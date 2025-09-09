#pragma once
#property strict

typedef uchar u8;
typedef uint  u32;
typedef ulong u64;
typedef int   i32;
typedef long  i64;
typedef int   ed_size;

#ifndef ED_INLINE
#define ED_INLINE static
#endif

#include "portable/ed25519_portable.inl"

/// \brief Generate Ed25519 keypair from seed.
/// \param sk Secret key output (32 bytes).
/// \param pk Public key output (32 bytes).
/// \param seed Input seed (32 bytes).
/// \return true on success, false on size mismatch.
bool ed25519_keypair_from_seed_mql(uchar &sk[], uchar &pk[], const uchar &seed[]) {
  if(ArraySize(sk) < 32 || ArraySize(pk) < 32 || ArraySize(seed) < 32)
    return false;
  return ed25519_keypair_from_seed(sk, pk, seed) == 1;
}

/// \brief Sign message with Ed25519.
/// \param msg Message bytes.
/// \param msg_len Message length.
/// \param sk Secret key (32 bytes).
/// \param pk Public key (32 bytes).
/// \param sig Signature output (64 bytes).
/// \return true on success, false on size mismatch.
bool ed25519_sign_mql(const uchar &msg[], int msg_len,
                      const uchar &sk[], const uchar &pk[], uchar &sig[]) {
  if(ArraySize(sk) < 32 || ArraySize(pk) < 32 || ArraySize(sig) < 64)
    return false;
  return ed25519_sign(msg, (ed_size)msg_len, sk, pk, sig) == 1;
}

/// \brief Verify Ed25519 signature.
/// \param msg Message bytes.
/// \param msg_len Message length.
/// \param pk Public key (32 bytes).
/// \param sig Signature (64 bytes).
/// \return true if signature is valid and sizes are OK.
bool ed25519_verify_mql(const uchar &msg[], int msg_len,
                        const uchar &pk[], const uchar &sig[]) {
  if(ArraySize(pk) < 32 || ArraySize(sig) < 64)
    return false;
  return ed25519_verify(msg, (ed_size)msg_len, pk, sig) == 1;
}
