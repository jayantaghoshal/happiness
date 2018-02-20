package com.volvocars.userswitch;

import com.volvocars.userswitch.UserSwitchResult;

interface IUserSwitchCallBack {
    void handleResponse(in UserSwitchResult result);
}