#pragma once
#include <cstdint>
typedef uint8_t u8;

struct rfc8032_vector {
  u8 seed[32];
  u8 pk[32];
  const u8* msg;
  int msg_len;
  u8 sig[64];
};

extern const rfc8032_vector rfc_vectors[];
extern const int rfc_vectors_len;
