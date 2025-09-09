## Purpose

Provide a **portable Ed25519 implementation** (RFC 8032) that compiles in both **C++11** and **MQL5**:

* **Core**: shared portable code in a C++11/MQL5 subset (no STL, no exceptions, no namespaces).
* **C++11 wrapper**: reference build with unit tests and benchmarks.
* **MQL5 wrapper**: usable inside MetaTrader 5 advisors and scripts without DLLs.

## Repository Layout

```
/include/
  ed25519_cpp11.hpp        # C++11 wrapper
/portable/
  ed25519_portable.inl     # shared portable core
ed25519_mql5.mqh           # MQL5 wrapper
/tests/
  rfc8032_vectors.hpp/.cpp # RFC 8032 test vectors
  test_kat.cpp             # Known Answer Tests
  test_sign_verify.cpp
/bench/
  bench_sign_verify.cpp
LICENSE
README.md
AGENTS.md
```

## Core Principles

1. **Strict RFC 8032 compliance** (Ed25519).
2. **Security-oriented implementation**:

   * Constant-time where possible (no secret-dependent branches).
   * No undefined behavior.
   * Clear buffer boundaries, caller-allocated.
3. **Portable subset**:

   * Core `.inl`: no STL, no exceptions, no templates, no namespaces.
   * Only primitive typedefs: `u8`, `u32`, `u64`.
   * No `memcpy`/`memset`: local inline helpers.
4. **Raw byte API**: fixed-size inputs/outputs, explicit `len`.
5. **Verification with RFC 8032 test vectors** and Known Answer Tests.

## Core API Contract

The same signatures for both C++11 and MQL5:

```c
/* portable/ed25519_portable.inl */
/* Types must be defined before including:
   typedef unsigned char u8;
   typedef unsigned int u32;
   typedef unsigned long long u64; */
#ifndef ED_INLINE
#define ED_INLINE static
#endif

ED_INLINE int ed25519_keypair_from_seed(u8 sk[32], u8 pk[32], const u8 seed[32]);
ED_INLINE int ed25519_sign(const u8* msg, int msg_len,
                           const u8 sk[32], const u8 pk[32],
                           u8 sig[64]);
ED_INLINE int ed25519_verify(const u8* msg, int msg_len,
                             const u8 pk[32], const u8 sig[64]);
```

Invariants:

* `seed`: 32 bytes → `sk`, `pk`: 32 bytes each.
* `sig`: 64 bytes (`R||S`).
* Functions return `1` for success, `0` for failure. No exceptions, no exits.

## C++11 Wrapper

* Allowed: `namespace ed25519`, `inline`, `<cstdint>`.
* Errors signaled via `bool`/`int`, no exceptions.

```cpp
#pragma once
#include <cstdint>
typedef uint8_t  u8; typedef uint32_t u32; typedef uint64_t u64;
#define ED_INLINE inline
#include "../portable/ed25519_portable.inl"

namespace ed25519 {
inline bool keypair_from_seed(u8 sk[32], u8 pk[32], const u8 seed[32]) { return ed25519_keypair_from_seed(sk, pk, seed)==1; }
inline bool sign(const u8* m, int n, const u8 sk[32], const u8 pk[32], u8 sig[64]) { return ed25519_sign(m,n,sk,pk,sig)==1; }
inline bool verify(const u8* m, int n, const u8 pk[32], const u8 sig[64]) { return ed25519_verify(m,n,pk,sig)==1; }
}
```

### Example (C++11)

```cpp
#include "include/ed25519_cpp11.hpp"
#include <cstdio>

int main() {
  u8 seed[32]={0}, sk[32], pk[32], sig[64];
  const char* msg="hello";
  if(!ed25519::keypair_from_seed(sk,pk,seed)) return 1;
  if(!ed25519::sign((const u8*)msg,5,sk,pk,sig)) return 2;
  if(!ed25519::verify((const u8*)msg,5,pk,sig)) return 3;
  puts("ok");
  return 0;
}
```

## MQL5 Wrapper

* Types: `typedef uchar u8; typedef uint u32; typedef ulong u64;`
* Same core `.inl` included.
* Public API checks array sizes before calling core.

```mql5
#pragma once
#property strict
typedef uchar u8; typedef uint u32; typedef ulong u64;
#define ED_INLINE static
#include "../portable/ed25519_portable.inl"

bool ed25519_keypair_from_seed_mql(uchar &sk[], uchar &pk[], const uchar &seed[]) {
  if(ArraySize(sk)<32 || ArraySize(pk)<32 || ArraySize(seed)<32) return false;
  return ed25519_keypair_from_seed(sk, pk, seed) == 1;
}
```

## Coding Style

* **Code comments**: English.
* **Naming**: `snake_case` for functions/variables. Constants in core also `snake_case`.
* **Formatting**:

  * Indent: 2 spaces, no tabs.
  * Braces: same line (`if (...) {`).
  * Max line length: 100 (soft).
* **Doxygen**: concise `/// \brief`, `@param`, `@return`.
* **Comments**: explain math invariants and non-trivial steps:

  ```c
  /// \brief Reduce modulo 2^255-19 using 64-bit limbs.
  /// Invariant: input in [0, 2^260), output < p.
  ```

## Security

* Constant-time for secrets (avoid branching on secret values).
* Intermediate operations in `u64`, documented reductions.
* Zero-out sensitive buffers where feasible.
* No trust in inputs: always check lengths and array bounds.

## Testing

* **RFC 8032 vectors**:

  * Positive: deterministic keypair, sign, verify.
  * Negative: bit flips in sig/pk/msg must fail verify.
* **Coverage**: edge cases in modular reduction, point operations.
* **Benchmarks**: sign/verify on messages of length 1B/64B/1KB.

Run example:

```
g++ -std=c++11 -O3 tests/test_kat.cpp -o test_kat
./test_kat
```

## API Invariants

* All public functions return `int` (1=success, 0=failure).
* Buffers pre-allocated by caller.
* No global mutable state in the core.

## Commits and PRs

* Commit messages must follow **Conventional Commits**.
  * Format: `type(scope): summary`.
  * The header must be written in English.
  * Include a body that describes the change.
  * Examples:
    * `feat(core): add scalar multiplication (ref10 port)`
    * `fix(verify): correct carry propagation`
    * `test(rfc): add RFC 8032 vectors`
    * `docs(readme): add MQL5 usage`
* One PR = one logical change. Tests required.
* CI: build + KAT (later).

## Review Checklist

* [ ] Core in portable subset (no STL/exceptions/namespaces).
* [ ] Buffer bounds validated.
* [ ] No secret-dependent branches or documented exceptions.
* [ ] New code covered by tests.
* [ ] Doxygen comments precise.
