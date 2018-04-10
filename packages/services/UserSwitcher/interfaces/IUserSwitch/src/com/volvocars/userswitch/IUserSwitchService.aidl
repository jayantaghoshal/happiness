package com.volvocars.userswitch;

/**
 * Binder interface for {@link android.volvocars.userswitch}.
 * Check {@link android.volvocars.userswitch} APIs for expected behavior of each calls.
 *
 * @hide
 */

import com.volvocars.userswitch.VolvoUser;
import com.volvocars.userswitch.IUserSwitchCallBack;

interface IUserSwitchService {
    void switchUser(String androidUserId, IUserSwitchCallBack callback) = 0;
    void createUser(in VolvoUser volvoUser, IUserSwitchCallBack callback) = 1;
    void pairUserTokey(IUserSwitchCallBack callback) = 2;
    void deleteUser(String userName, IUserSwitchCallBack callback) = 3;
}