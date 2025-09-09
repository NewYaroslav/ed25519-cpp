#include "../include/ed25519_cpp11.hpp"
#include <random>
#include <cstdio>

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  for (int i = 0; i < 10; ++i) {
    u8 seed[32];
    u8 sk[32];
    u8 pk[32];
    u8 sig[64];
    u8 msg[32];
    for (int j = 0; j < 32; ++j) {
      seed[j] = static_cast<u8>(gen());
      msg[j] = static_cast<u8>(gen());
    }
    if (!ed25519::keypair_from_seed(sk, pk, seed)) return 1;
    if (!ed25519::sign(msg, sizeof msg, sk, pk, sig)) return 2;
    if (!ed25519::verify(msg, sizeof msg, pk, sig)) return 3;
    sig[0] ^= 1;
    if (ed25519::verify(msg, sizeof msg, pk, sig)) return 4;
    sig[0] ^= 1;
    msg[0] ^= 1;
    if (ed25519::verify(msg, sizeof msg, pk, sig)) return 5;
  }
  puts("sign/verify ok");
  return 0;
}
