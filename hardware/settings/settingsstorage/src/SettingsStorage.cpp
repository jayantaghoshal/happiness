/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "SettingsStorage.h"

#include <cutils/log.h>
#include <gsl/span>
#undef LOG_TAG
#define LOG_TAG "SettingsStorage"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace settings {
namespace V1_0 {
namespace implementation {

constexpr int64_t IPROFILES_DEATH_COOKIE = 0xDEAD;
constexpr int64_t ISETTINGSLISTENER_DEATH_COOKIE = 0xDEAD;

ProfileListener::ProfileListener(std::function<void(profileHidl::ProfileIdentifier)> onProfileChange)
    : on_profile_change_{std::move(onProfileChange)} {
    profileHidl::ICarProfileManager::registerForNotifications("default", this);
}

andrHw::Return<void> ProfileListener::onRegistration(const andrHw::hidl_string& fqName,
                                                     const andrHw::hidl_string& name,
                                                     bool preexisting) {
    ALOGV("onRegistration, fqName=%s, name=%s, preexisting=%d", fqName.c_str(), name.c_str(), preexisting);

    car_profile_manager_ = profileHidl::ICarProfileManager::getService();
    if (car_profile_manager_ == nullptr) {
        ALOGE("Could not contact ICarProfileManager HAL");
        return andrHw::Return<void>();
    }
    auto result = car_profile_manager_->linkToDeath(this, IPROFILES_DEATH_COOKIE);
    if (!result.isOk()) {
        ALOGE("ICarProfileManager link to death error: %s", result.description().c_str());
        return andrHw::Return<void>();
    }

    car_profile_manager_->subscribeUserChange(this);

    return andrHw::Return<void>();
}

void ProfileListener::serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) {
    (void)who;
    if (cookie != IPROFILES_DEATH_COOKIE) {
        return;
    }
    ALOGE("ICarProfileManager disconnected");
    car_profile_manager_ = nullptr;
}

Return<void> ProfileListener::profileChanged(const andrHw::hidl_string& android_user_id,
                                             profileHidl::ProfileIdentifier new_profile_id) {
    (void)android_user_id;
    ALOGD("Profile changed");
    on_profile_change_(new_profile_id);
    return andrHw::Return<void>();
}

std::string build_sqllite_error_string(const std::string& msg, int result_code, sqlite3* db, char* err_msg) {
    std::string err_msg_str;
    if (err_msg != nullptr) {
        err_msg_str = std::string(err_msg);
        sqlite3_free(err_msg);
    }
    return msg + " rc=" + std::to_string(result_code) +  //
           " errstr=" + sqlite3_errstr(result_code) +    //
           " errmsgdb=" + sqlite3_errmsg(db) +           //
           " errmsg=" + err_msg_str;
}

using RowCallbackFunction = std::function<void(gsl::span<char*>, gsl::span<char*>)>;
int exec_row_callback(void* user_data, int nr_of_columns, char** column_values, char** column_names) {
    auto callback = static_cast<RowCallbackFunction*>(user_data);
    (*callback)(gsl::span<char*>(column_values, column_values + nr_of_columns),  // NOLINT C->CPP boundary
                gsl::span<char*>(column_names, column_names + nr_of_columns));   // NOLINT C->CPP boundary
    return 0;
}

struct SqliteException : public std::logic_error {
    SqliteException(const std::string& msg, int result_code, sqlite3* db, char* err_msg = nullptr)
        : std::logic_error(build_sqllite_error_string(msg, result_code, db, err_msg)) {}
};

SettingsStorage::SettingsStorage() {
    {
        // NOTE: Since we run WAL mode, you also have to copy the xxx-shm and xxx-wal file
        //      in case you want to debug the contents on host side.
        const int status = sqlite3_open("/data/vendor/vehiclesettings.db", &db_);
        if (status != 0) {
            ALOGW("Can't open database: %s", sqlite3_errmsg(db_));
            throw SqliteException("Can't open database", status, db_);
        } else {
            ALOGI("Opened database successfully");
        }
        // Enable more detailed result codes
        (void)sqlite3_extended_result_codes(db_, 1);

        // Auto retry in case DB is locked, this way we don't have to handle
        // error code SQLITE_BUSY.
        // If db is locked more than this time we just bail out and log error.
        // This should not be necessary because only one process SHOULD work with the db simultaneously, but just in
        // case..
        (void)sqlite3_busy_timeout(db_, 3000);
    }
    {
        const std::string sql =
                "CREATE TABLE IF NOT EXISTS settings("    //
                "settings_key INTEGER NOT NULL, "         //
                "profile_id  INTEGER NOT NULL, "          //
                "setting_value TEXT NOT NULL, "           //
                "PRIMARY KEY (settings_key, profile_id)"  //
                ") WITHOUT ROWID;";
        char* errMsg;
        const int status = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg);
        if (status != SQLITE_OK) {
            throw SqliteException("Can't create table", status, db_, errMsg);
        }
    }
    {
        // Enable Write Ahead Log to save on flash wear
        // Without this option each transaction forces a fflush() of journal to OS before acking.
        // TODO(PSS370-12695): Investigate if this is really the optimal solution
        // TODO(PSS370-12695): Configure checkpointing threshold, autocheckpoint, time triggered, etc
        //
        // See //https://sqlite.org/wal.html for more details
        RowCallbackFunction rowCallback = [](gsl::span<char*> column_values, gsl::span<char*> column_names) {
            (void)column_names;
            const char* const col1 = column_values[0];
            if (strncmp(col1, "wal", 3) == 0) {
                ALOGV("WAL mode set to %s", col1);
            } else {
                ALOGW("WAL mode not, mode=%s", col1);
            }
        };

        char* errMsg;
        const int status = sqlite3_exec(db_, "PRAGMA journal_mode=WAL;", exec_row_callback, &rowCallback, &errMsg);
        if (status != SQLITE_OK) {
            throw SqliteException("Can't set journal_mode", status, db_, errMsg);
        }
    }

    {
        const std::string sql =
                "INSERT OR REPLACE INTO settings (settings_key, profile_id, setting_value) VALUES(?1, ?2, ?3);";
        const int status = sqlite3_prepare_v2(db_, sql.c_str(), sql.size(), &insert_stmt_, nullptr);
        if (status != SQLITE_OK) {
            throw SqliteException("Can't prepare insert statement", status, db_, nullptr);
        }
    }
    {
        const std::string sql =
                "SELECT setting_value, profile_id "         //
                "FROM settings "                            //
                "WHERE settings_key=?1 AND profile_id=?2 "  //
                "LIMIT 1;";
        const int status = sqlite3_prepare_v2(db_, sql.c_str(), sql.size(), &select_setting_stmt_, nullptr);
        if (status != SQLITE_OK) {
            throw SqliteException("Can't prepare select statement", status, db_, nullptr);
        }
    }

    android::wp<SettingsStorage> weakThis{this};
    profile_listener_ = android::sp<ProfileListener>(new ProfileListener([weakThis](profileHidl::ProfileIdentifier newProfileId) {
        android::sp<SettingsStorage> s = weakThis.promote();
        if (s != nullptr) {
            s->onProfileChange(newProfileId);
        }
    }));
}
SettingsStorage::~SettingsStorage() {
    sqlite3_free(select_setting_stmt_);
    sqlite3_free(insert_stmt_);
    sqlite3_free(db_);
}

void SettingsStorage::onProfileChange(profileHidl::ProfileIdentifier profileId) {
    activeProfileId = profileId;
    for (auto& sl : settings_listeners_) {
        if (sl.user_scope_ == UserScope::NOT_USER_RELATED) {
            continue;
        }
        const unsigned char* data = getData(sl.key_, activeProfileId);

        SettingsChangeReason reason = SettingsChangeReason::ProfileChange;
        if (data == nullptr) {
            reason = SettingsChangeReason::Reset;
        }
        sl.listener_->settingsForCurrentUserChanged(sl.key_, reason, activeProfileId);
    }
}

Return<void> SettingsStorage::set(const SettingsIdHidl key,
                                  profileHidl::ProfileIdentifier profileId,
                                  const hidl_string& data) {
    ALOGD("set %d = %s", key, data.c_str());

    {
        (void)sqlite3_reset(insert_stmt_);
        sqlite3_bind_int(insert_stmt_, 1, key);
        sqlite3_bind_int(insert_stmt_, 2, static_cast<int32_t>(profileId));
        sqlite3_bind_text(insert_stmt_, 3, data.c_str(), data.size(), SQLITE_STATIC);
        int status = sqlite3_step(insert_stmt_);
        if (status != SQLITE_DONE) {
            ALOGE("SQL error insert. Step: %d, %s", status, sqlite3_errmsg(db_));
            return Return<void>(andrHw::Status::fromExceptionCode(andrHw::Status::Exception::EX_ILLEGAL_STATE));
        }
        ALOGD("Settings inserted successfully, key=%d", key);
    }
    return andrHw::Return<void>();
}

const unsigned char* SettingsStorage::getData(const SettingsIdHidl key, profileHidl::ProfileIdentifier profileId) {
    (void)sqlite3_reset(select_setting_stmt_);
    sqlite3_bind_int(select_setting_stmt_, 1, key);
    sqlite3_bind_int(select_setting_stmt_, 2, static_cast<int32_t>(profileId));
    int status = sqlite3_step(select_setting_stmt_);
    if (status == SQLITE_DONE) {
        ALOGV("getData no data found for key %d", key);
        // No row found
        return nullptr;
    }
    if (status != SQLITE_ROW) {
        ALOGW("SQL error get. Step: %d, %s", status, sqlite3_errmsg(db_));
        return nullptr;
    }

    // https://sqlite.org/c3ref/column_blob.html
    //    The pointers returned are valid until a type conversion occurs as described above,
    //    or until sqlite3_step() or sqlite3_reset() or sqlite3_finalize() is called
    // sqlite guarantees these always null terminated (or null for empy)
    const unsigned char* data = sqlite3_column_text(select_setting_stmt_, 0);

    if (data == nullptr) {
        ALOGV("getData key=%d found data with empty/null content", key);
    } else {
        ALOGV("getData key=%d found data %s", key, data);
    }
    return data;
}

Return<void> SettingsStorage::get(const SettingsIdHidl key, profileHidl::ProfileIdentifier profileId, get_cb _hidl_cb) {
    ALOGD("get %d", key);
    const unsigned char* data = getData(key, profileId);
    // TODO(ee): How to handle NULL on get vs settingsReset
    hidl_string returnValue{reinterpret_cast<const char*>(data)};  // NOLINT
    _hidl_cb(returnValue);
    return andrHw::Return<void>();
}

Return<void> SettingsStorage::subscribe(const SettingsIdHidl key,
                                        UserScope userScope,
                                        const sp<ISettingsListener>& listener) {
    ALOGD("subscribe key=%d, userScope=%hu", key, userScope);

    settings_listeners_.emplace_back(key, userScope, listener);
    listener->linkToDeath(this, ISETTINGSLISTENER_DEATH_COOKIE);

    profileHidl::ProfileIdentifier profileToGet =
            ((userScope == UserScope::NOT_USER_RELATED) ? profileHidl::ProfileIdentifier::None : activeProfileId);
    const unsigned char* data = getData(key, profileToGet);

    SettingsChangeReason reason = SettingsChangeReason::ProfileChange;
    if (data == nullptr) {
        reason = SettingsChangeReason::Reset;
    }
    listener->settingsForCurrentUserChanged(key, reason, profileToGet);
    return andrHw::Return<void>();
}

Return<void> SettingsStorage::unsubscribe(const SettingsIdHidl key, const sp<ISettingsListener>& listener) {
    ALOGD("unsubscribe %d", key);
    settings_listeners_.remove_if([&](const auto& x) { return x.key_ == key && ((x.listener_) == (listener)); });
    return andrHw::Return<void>();
}

void SettingsStorage::serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) {
    if (cookie != ISETTINGSLISTENER_DEATH_COOKIE) {
        return;
    }

    android::sp<::android::hidl::base::V1_0::IBase> whoPromoted = who.promote();
    if (whoPromoted == nullptr) {
        // In case we cant promote the wp it means the sp from settings_listeners_ is already gone
        return;
    }

    ALOGE("ISettingsListener disconnected");
    settings_listeners_.remove_if([&](const auto& x) { return ((x.listener_) == (whoPromoted)); });
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace settings
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
