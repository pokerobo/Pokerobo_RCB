#include "Message_Exchange.h"

//-------------------------------------------------------------------------------------------------

MessagePacket::MessagePacket(MessageInterface* action, MessageInterface* command) {
  _action = action;
  _command = command;
}

uint8_t MessagePacket::length() {
  uint8_t len = 2; // 2 bytes header
  if (_action != NULL) {
    len += _action->length();
  }
  if (_command != NULL) {
    len += _command->length();
  }
  return len;
}

uint8_t* MessagePacket::serialize(uint8_t* buf, uint8_t len) {
  if (len < length()) {
    return NULL;
  }

  if (_action == NULL) {
    return NULL;
  }

  buf[0] = _signature[0];
  buf[1] = _signature[1];

  _action->serialize(&buf[2], _action->length());

  if (_command != NULL) {
    _command->serialize(&buf[_action->length()], _command->length());
  }

  return buf;
}

//-------------------------------------------------------------------------------------------------

uint8_t* encodeInteger(uint8_t* store, uint16_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
  return store;
}

uint8_t* encodeInteger(uint8_t* store, uint32_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
  store[2] = (value >> 16) & 0xff;
  store[3] = (value >> 24) & 0xff;
  return store;
}

uint8_t* encodeMessage(uint8_t* msg, char* cmd, uint16_t pressed, uint16_t x, uint16_t y, uint32_t extras) {
  uint8_t *buf = msg;
  if (cmd != NULL) {
    buf[0] = cmd[0];
    buf[1] = cmd[1];
    buf += 2;
  }
  encodeInteger(&buf[2], pressed);
  encodeInteger(&buf[4], x);
  encodeInteger(&buf[6], y);
  encodeInteger(&buf[8], extras);
  return msg;
}

uint8_t* encodeMessage_old(uint8_t* buf, char* cmd, uint16_t pressed, uint16_t x, uint16_t y, uint32_t extras) {
  if (cmd == NULL) {
    encodeInteger(&buf[0], pressed);
    encodeInteger(&buf[2], x);
    encodeInteger(&buf[4], y);
    encodeInteger(&buf[6], extras);
    return buf;
  }
  buf[0] = cmd[0];
  buf[1] = cmd[1];
  encodeInteger(&buf[2], pressed);
  encodeInteger(&buf[4], x);
  encodeInteger(&buf[6], y);
  encodeInteger(&buf[8], extras);
  return buf;
}

#if __SPACE_SAVING_MODE__
uint32_t decodeInteger(uint8_t* arr, int length) {
  if (length == 2) {
    uint16_t a1 = arr[1];
    return (a1 << 8) | arr[0];
  }
  if (length == 4) {
    uint16_t a1 = arr[1];
    uint32_t a2 = arr[2];
    uint32_t a3 = arr[3];
    return (a3 << 24) | (a2 << 16) | (a1 << 8) | arr[0];
  }
  return 0;
}
#else
uint32_t decodeInteger(uint8_t* arr, int length) {
  uint32_t a0 = arr[0];
  uint32_t a1 = arr[1];
  uint32_t a2 = arr[2];
  uint32_t a3 = arr[3];
  if (length == 2) {
    return (a1 << 8) | a0;
  }
  if (length == 4) {
    return (a3 << 24) | (a2 << 16) | (a1 << 8) | a0;
  }
  return 0;
}
#endif

bool decodeMessage(uint8_t* msg, char* cmd, uint16_t* buttons, uint16_t* x, uint16_t* y, uint32_t* extras) {
  if (cmd != NULL) {
    if (msg[0] != cmd[0] || msg[1] != cmd[1]) {
      return false;
    }
    msg += 2;
  }
  *buttons = decodeInteger(&msg[0], 2);
  *x = decodeInteger(&msg[2], 2);
  *y = decodeInteger(&msg[4], 2);
  *extras = decodeInteger(&msg[6], 4);
  return true;
}

bool decodeMessage_old(uint8_t* msg, char* cmd, uint16_t* buttons, uint16_t* x, uint16_t* y, uint32_t* extras) {
  if (cmd == NULL) {
    *buttons = decodeInteger(&msg[0], 2);
    *x = decodeInteger(&msg[2], 2);
    *y = decodeInteger(&msg[4], 2);
    *extras = decodeInteger(&msg[6], 4);
    return true;
  }
  if (msg[0] == cmd[0] && msg[1] == cmd[1]) {
    *buttons = decodeInteger(&msg[2], 2);
    *x = decodeInteger(&msg[4], 2);
    *y = decodeInteger(&msg[6], 2);
    *extras = decodeInteger(&msg[8], 4);
    return true;
  }
  return false;
}
