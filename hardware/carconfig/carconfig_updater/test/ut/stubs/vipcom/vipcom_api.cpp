/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <vipcom/vipcom_api.h>

VipCom::VipCom(Application appIndex, IVipComClient* vipcom_client) {}
VipCom::~VipCom() {}
int VipCom::sendMessage(const uint8_t& _fid, const std::vector<uint8_t>& _payload) {
    // Return happy case when all bytes are sent.
    return _payload.size() + sizeof _fid;
}
