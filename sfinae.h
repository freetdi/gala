/* Copyright (C) 2015-2016 Felix Salfelder
 * Author: Felix Salfelder
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *------------------------------------------------------------------
 * identify container types ins template specialization.
 */

#ifndef GALA_SFINAE_H
#define GALA_SFINAE_H

#include <stx/btree_set.h>
#include <set>
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
namespace gala{
namespace sfinae{
/*--------------------------------------------------------------------------*/
struct any{int dummy;};
/*--------------------------------------------------------------------------*/
template<class A, class B=any, class T=void>
struct is_set{
	static constexpr bool value = true;
};
/*--------------------------------------------------------------------------*/

template<class S, class T>
struct is_set<S, typename std::enable_if < std::is_same<
std::set<any, typename S::key_compare, typename S::allocator_type >, S
>::value, any >::type , T>{

	typedef T type;
};
template<class S, class T>
struct is_set<S, typename std::enable_if < std::is_same<
stx::btree_set<any, typename S::key_compare, typename S::allocator_type >, S
>::value, any >::type , T>{

	typedef T type;
};
/*--------------------------------------------------------------------------*/
template<class A, class B=any, class T=void>
struct is_seq{
	static constexpr bool value = true;
};
template<class S, class T>
struct is_seq<S, typename std::enable_if < std::is_same<
std::vector<any, typename S::allocator_type >, S
>::value, any >::type , T>{

	typedef T type;
};
template<class S, class T>
struct is_seq<S, typename std::enable_if < std::is_same<
std::deque<any, typename S::allocator_type >, S
>::value, any >::type , T>{

	typedef T type;
};

}//sfinae
} // namespace gala

#endif
