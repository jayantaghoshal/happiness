
#ifndef MESSAGE_API_H
#define MESSAGE_API_H

#include <reuse.h>

typedef struct Message_Send_Tag {
    uint32_t data_size;
    uint8_t *data_ptr;
} Message_Send_T;

// Use this function to send messages from common components into the vehicle data driver module.
void messageSend(Message_Send_T *msg_data);

// Use this function to inject AVMP messages into the vehicle signals manager module.
void avmpMessageInject(uint8_t *msg_data, const uint32_t data_size);

#endif  // MESSAGE_API_H
