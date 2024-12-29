// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_ASSERT_HH
#define DUNE_ASSERT_HH

#include <cassert>

/**
 * @addtogroup Common
 *
 * @{
 */

#ifndef DUNE_ASSERT
#if defined(DUNE_ENABLE_DEBUG) || defined(DOXYGEN)
  #define DUNE_ASSERT(...) assert((__VA_ARGS__))
  #define DUNE_ASSERT_MSG(cond, msg) assert((cond) && msg)
#else
  #define DUNE_ASSERT(...)
  #define DUNE_ASSERT_MSG(cond, msg)
#endif
#endif

/** @} */

#endif // DUNE_ASSERT_HH
