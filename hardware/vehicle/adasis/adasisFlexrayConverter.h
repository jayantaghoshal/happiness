/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
class adasisFlexrayConverter {
  public:
    virtual bool sendShortHorizonMessage() = 0;
    virtual bool sendLongHorizonMessage() = 0;
    virtual bool sendHorizonMetaData() = 0;
    virtual bool sendStubMessage() = 0;
    virtual bool sendSegmentMessage() = 0;
    virtual bool sendPositionMessage() = 0;
    virtual ~adasisFlexrayConverter() = 0;
};
