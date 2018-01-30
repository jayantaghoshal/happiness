package com.volvocars.userswitch;

/**
 * Binder interface for {@link android.volvocars.userswitch}.
 * Check {@link android.volvocars.userswitch} APIs for expected behavior of each calls.
 *
 * @hide
 */

import com.volvocars.userswitch.VolvoUser;

interface IUserSwitchService {
    void switchUser(in VolvoUser volvoUser) = 0;
    void createUser(in VolvoUser volvoUser) = 1;
    void pairUserTokey() = 2;
    void deleteUser(in VolvoUser volvoUser) = 3;
}