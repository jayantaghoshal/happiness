/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <functional>

/*
 * Wrapper class to hold two versions of callback lamdas:
 * 1) one without argument
 * 2) one with argument
 */
template <typename S>
struct CallbackWrapper {
    void set(const std::function<void()>& callback) {
        _callbackValue = nullptr;
        _callbackEmpty = callback;
    }

    void set(const std::function<void(S)>& callback) {
        _callbackEmpty = nullptr;
        _callbackValue = callback;
    }

    void operator()(const S& arg) {
        if (_callbackValue) {
            _callbackValue(arg);
        } else if (_callbackEmpty) {
            _callbackEmpty();
        }
    }

    std::function<void(S)> _callbackValue;
    std::function<void()> _callbackEmpty;
};
