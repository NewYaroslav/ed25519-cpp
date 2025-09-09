#pragma once
#include <cstdint>
typedef uint8_t  u8; typedef uint32_t u32; typedef uint64_t u64;
#define ED_INLINE inline
#include "../portable/ed25519_portable.inl"

namespace ed25519 {
inline bool keypair_from_seed(u8 sk[32], u8 pk[32], const u8 seed[32]) { return ed25519_keypair_from_seed(sk, pk, seed) == 1; }
inline bool sign(const u8* m, int n, const u8 sk[32], const u8 pk[32], u8 sig[64]) { return ed25519_sign(m, n, sk, pk, sig) == 1; }
inline bool verify(const u8* m, int n, const u8 pk[32], const u8 sig[64]) { return ed25519_verify(m, n, pk, sig) == 1; }
}
