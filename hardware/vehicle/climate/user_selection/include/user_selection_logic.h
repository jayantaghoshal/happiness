/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include "notifiable_property.h"
#include <v0/org/volvocars/climate/UserSelection.hpp>
#include "logging_context.h"
#include "settings_proxy.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

namespace Selection
{
LOG_SET_DEFAULT_CONTEXT(UserSelectionContext)

template <typename SharedType,
          typename SelectionStateType = typename std::tuple_element<0, decltype(SharedType::Struct::values_)>::type,
          typename SelectionType      = typename std::tuple_element<1, decltype(SharedType::Struct::values_)>::type>
class UserSelectionLogic
{
public:
    template <typename... Ts>
    UserSelectionLogic(NotifiableProperty<SharedType>&     selection,
                       std::unique_ptr<SettingsProxyInterface<int>> settingSelection,
                       Ts... carConfigParams)
        : UserSelectionLogic{ selection, std::move(settingSelection), CarConfigIsValid(carConfigParams...) }
    {
    }

    // Used when no carconfig is required
    UserSelectionLogic(NotifiableProperty<SharedType>& selection, std::unique_ptr<SettingsProxyInterface<int>> settingSelection)
        : UserSelectionLogic{ selection, std::move(settingSelection), true }
    {
    }

    ~UserSelectionLogic()
    {
        settingSelection_->unSubscribe();
    }

    void request(SelectionType value)
    {
        settingSelection_->set(
            static_cast<typename std::underlying_type<typename SelectionType::Literal>::type>(value));
        selection_.set({ SelectionStateType::Literal::AVAILABLE, value });
    }

private:
    UserSelectionLogic(NotifiableProperty<SharedType>&     selection,
                       std::unique_ptr<SettingsProxyInterface<int>> settingSelection,
                       bool                                carConfigValid)
        : selection_{ selection }
        , settingSelection_{ std::move(settingSelection) }
        , carConfigValid_{ carConfigValid }
    {

        auto handleSelection = [this] {
            if (carConfigValid_)
            {
                selection_.set({ SelectionStateType::Literal::AVAILABLE,
                                  static_cast<typename SelectionType::Literal>(settingSelection_->get()) });
            }
            else
            {
                selection_.set(
                    { SelectionStateType::Literal::NOT_PRESENT, static_cast<typename SelectionType::Literal>(0) });
            }

            log_debug() << "User Selection state: " << std::get<0>(selection_.get().values_)
                        << "User Selection value: " << std::get<1>(selection_.get().values_);

        };

        settingSelection_->subscribe(handleSelection);
        handleSelection();
    }

    template <typename T>
    bool CarConfigIsValid(T param)
    {
        using Enum = decltype(param);
        return param == carconfig::getValue<Enum>();
    }

    template <typename T, typename... Ts>
    bool CarConfigIsValid(T param, Ts... params)
    {
        using Enum = decltype(param);
        bool valid = (param == carconfig::getValue<Enum>());
        return valid && CarConfigIsValid(params...);
    }

private:
    NotifiableProperty<SharedType>&     selection_;
    std::unique_ptr<SettingsProxyInterface<int>> settingSelection_;
    bool const                          carConfigValid_;
};
}
