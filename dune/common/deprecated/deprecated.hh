// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_DEPRECATED_DEPRECATED_HH
#define DUNE_DEPRECATED_DEPRECATED_HH

#warning "The file common/deprecated/deprecated.hh is deprecated. Replace the macros DUNE_DEPRECATED and DUNE_DEPRECATED_MSG by the corresponding C++ attribute [[deprecated]]"

//! Mark some entity as deprecated
#define DUNE_DEPRECATED __attribute__((deprecated))
//! Mark some entity as deprecated and show a deprecation message
#define DUNE_DEPRECATED_MSG(text) __attribute__((deprecated(# text)))

#endif // DUNE_DEPRECATED_DEPRECATED_HH
