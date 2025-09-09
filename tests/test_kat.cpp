#include "rfc8032_vectors.hpp"
#include "../include/ed25519_cpp11.hpp"
#include <cstring>
#include <cstdio>

int main() {
  for (int i = 0; i < rfc_vectors_len; ++i) {
    const rfc8032_vector& v = rfc_vectors[i];
    u8 sk[32];
    u8 pk[32];
    u8 sig[64];
    if (!ed25519::keypair_from_seed(sk, pk, v.seed)) return 1;
    if (std::memcmp(pk, v.pk, 32) != 0) return 2;
    if (!ed25519::sign(v.msg, v.msg_len, sk, pk, sig)) return 3;
    if (std::memcmp(sig, v.sig, 64) != 0) return 4;
    if (!ed25519::verify(v.msg, v.msg_len, pk, sig)) return 5;
    sig[0] ^= 0x80;
    if (ed25519::verify(v.msg, v.msg_len, pk, sig)) return 6;
  }
  puts("kat ok");
  return 0;
}
