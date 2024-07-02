// Felix Salfelder 2023
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, 51 Franklin Street - Suite 500, Boston, MA 02110-1335, USA.
//
//
// platform specific tweaks and nonsense
//
#ifndef GALA_PLATFORM_H
#define GALA_PLATFORM_H

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <cstdint>
#include "trace.h"
/*--------------------------------------------------------------------------*/
#ifdef __APPLE__
#ifdef howmany
#undef howmany
#endif
#endif
/*--------------------------------------------------------------------------*/
// count the number of ones. some platforms offer fast implementations
template<class T>
inline unsigned generic_popcount(T x)
{ untested();
  unsigned r = 0;
  while(x){ untested();
    r += x&1;
    x =x >>1;
  }
  return r;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <intrin.h>
#endif
/*--------------------------------------------------------------------------*/
#if __cplusplus >= 202002L
#include <bit>
using std::popcount;
#else
template<class T>
inline unsigned popcount(T x)
{
#ifdef __GNUC__
  {
    return __builtin_popcount(x);
  }
#else
  { untested();
    return generic_popcount(x);
  }
#endif
}
/*--------------------------------------------------------------------------*/
template<>
inline unsigned popcount(uint64_t x)
{
#ifdef __GNUC__
  {
    return __builtin_popcountl(x);
  }
#else
  { untested();
    return generic_popcount(x);
  }
#endif
}
# endif
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
// vim:ts=8:sw=2:noet
