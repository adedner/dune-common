// -*- tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set ts=8 sw=2 et sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_ASSUME_HH
#define DUNE_COMMON_ASSUME_HH

// define the macro DUNE_ASSUME(...) that is a portable assume expression
// as a hint for the compiler, e.g., optimizer. If no equivalent to
// the assume attribute is available, it falls back to an `assert()` macro.

// The implementation is inspired by
// https://stackoverflow.com/questions/63493968/reproducing-clangs-builtin-assume-for-gcc

// If the C++ standard attribute assume is available
#ifdef __has_cpp_attribute
  #if __has_cpp_attribute(assume) >= 202207L
    #define DUNE_ASSUME(...) [[assume(__VA_ARGS__)]]
  #endif
#endif

// if ompiler intrinsics/attributes for assumptions are available
#ifndef DUNE_ASSUME
  #if defined(__clang__)
    #define DUNE_ASSUME(...) do { __builtin_assume(__VA_ARGS__); } while(0)
  #elif defined(_MSC_VER)
    #define DUNE_ASSUME(...) do { __assume(__VA_ARGS__); } while(0)
  #elif defined(__GNUC__)
    #if __GNUC__ >= 13
      #define DUNE_ASSUME(...) __attribute__((__assume__(__VA_ARGS__)))
    #endif
  #endif
#endif

// if we are in release mode, use undefined behavior as a way to enforce an assumption
#if !defined(DUNE_ASSUME) && defined(NDEBUG)
  #include <utility>
  #if defined(__GNUC__)
    #define DUNE_ASSUME(...) do { if (!bool(__VA_ARGS__)) __builtin_unreachable(); } while(0)
  #elif __cpp_lib_unreachable >= 202202L
    #define DUNE_ASSUME(...) do { if (!bool(__VA_ARGS__)) ::std::unreachable(); } while(0)
  #endif
#endif

// in debug mode and if not defined before, use the assert macro
#ifndef DUNE_ASSUME
  #define DUNE_ASSUME(...) assert(bool(__VA_ARGS__))
#endif

#endif // DUNE_COMMON_ASSUME_HH