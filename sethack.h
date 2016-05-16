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
 * set related hacks
 */
#ifndef SETHACK_H
#define SETHACK_H

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include <stx/btree_set.h>
#include <set>
#include <assert.h>

template <class S>
inline S& operator<<( S& o, const std::set<unsigned> &s)
{untested();
	o << "(";
	std::string comma;
	for(auto i : s){untested();
		o << comma << i;
		comma=", ";
	}
	o << ")";
	return o;
}
template <class S>
inline S& operator<<( S& o, const std::set<short unsigned> &s)
{untested();
	o << "(";
	std::string comma;
	for(auto i : s){untested();
		o << comma << i;
		comma=", ";
	}
	o << ")";
	return o;
}

namespace detail{ // ??
//inline std::basic_ostream<char>& operator<<( std::basic_ostream<char>& o, std::set<short unsigned int> s)
template <class S>
inline S& operator<<( S& o, const std::set<short unsigned> &s)
{untested();
	o << "(";
	std::string comma;
	for(auto i : s){untested();
		o << comma << i;
		comma=", ";
	}
	o << ")";
	return o;
}
}

namespace sethack{ //

template<class I, class ref>
class skip_it : public I{//
public:
//	skip_it(const skip_it& p) : I(p)
//	{ untested();
//	}
	skip_it(I& i, I const& e, const ref& what): I(i), _skip(what), _end(e)
	{
		skip();
	}
	skip_it& operator++()
	{
		if (I::operator==(_end)){ untested();
		}else{
			I::operator++();
			skip();
		}
		return *this;
	}
	bool operator==(const I& i)
	{
		return I::operator==(i);
	}
	bool operator!=(const I& i)
	{
		return I::operator!=(i);
	}

private:
	void skip()
	{
		if (I::operator==(_end)){
		}else if (I::operator*() != _skip){
		}else{
			I::operator++();
		}
	}
	ref _skip;
	I _end;
};

template<class I, class S>
inline skip_it<I,S> make_skip_it(I b, I e, S s)
{
	return skip_it<I,S>(b, e, s);
}

template<class C, class B>
inline skip_it<C,B> make_skip_it(C c, B b)
{untested();
	return make_skip_it(c.begin(), c.end() ,b);
}

} // sethack
/*--------------------------------------------------------------------------*/
template<class S>
struct sethack_{ //
	static typename S::iterator reverse(typename S::reverse_iterator& x)
	{untested();
		return x.base();
	}
};
/*--------------------------------------------------------------------------*/
template<>
struct sethack_<stx::btree_set<size_t> >{ //
	typedef stx::btree_set<size_t> S;
	static typename S::iterator reverse(typename S::reverse_iterator& x)
	{untested();
		return S::iterator(x);
	}
};

template<class S>
typename S::iterator reverse(typename S::reverse_iterator& x, S const& s)
{untested();
	return sethack_<S>::reverse(x);
}


// TODO: put generic implementation here (see below for unordered_sets),
// then specialize for ordered sets
template<class S, class S_iterator, class T_iterator, typename=void>
struct merge{ //
	// count elements in src range that are not in tgt
	static size_t count_missing(S const& tgt, S_iterator src_begin, T_iterator src_end)
	{ untested();
		size_t missing=0;
		while(src_begin!=src_end){untested();
			if(tgt.find(*src_begin)==tgt.end()){untested();
				++missing;
			}
			++src_begin;
		}
		return missing;
	}
// S_iterator and T_iterator must be comparable...
//template<class S, class S_iterator, class T_iterator>
// static void zipfwd(S& tgt, S_iterator src_begin, T_iterator src_end)
// { incomplete();
// }
	//typedef typename std::unordered_set<X> S;
	static void zipfwd(S& tgt, S_iterator src_begin, T_iterator src_end)
	{ untested();
		auto i=src_begin;
		for(; i!=src_end; ++i){untested();
			tgt.insert(*i);
		}
	}
}; // merge
/*--------------------------------------------------------------------------*/
template<class X>
struct is_ordered_set{ //
	typedef void type;
};
template<class X>
struct is_ordered_set<std::unordered_set<X> >{ };
/*--------------------------------------------------------------------------*/
template<class D>
class my_counter{ //
	size_t _cnt;
	D _dummy;
	public:
      typedef D value_type;
	public:
		my_counter() : _cnt(0){}
		void operator++(){
			++_cnt;
		}
		size_t count() const
		{itested();
			return _cnt;
		}
		D& operator*(){
			return _dummy;
		}
};
/*--------------------------------------------------------------------------*/
namespace std{ //
template<class D>
struct iterator_traits<my_counter<D> >{ //
	typedef D value_type;
};
}
/*--------------------------------------------------------------------------*/
template<class S, class S_iterator, class T_iterator>
struct merge<S, S_iterator, T_iterator, typename is_ordered_set<S>::type >{ //

	static size_t count_missing(S const& tgt, S_iterator src_begin, T_iterator src_end)
	{ itested();

		my_counter<typename S::value_type> counter;
		counter = std::set_difference (src_begin, src_end, tgt.begin(), tgt.end(), counter);
		size_t missing=counter.count();

#if 0 // essentially this...
		size_t missing2=0;
		while(src_begin!=src_end){untested();
			if(tgt.find(*src_begin)==tgt.end()){untested();
				++missing2;
			}
			++src_begin;
		}
		assert(missing==missing2);
#endif

		return missing;
	}

	template<class CB>
	static void zipfwd(S& tgt, S_iterator src_begin, T_iterator src_end, CB* cb=NULL)
	{ itested();

		S_iterator seek=src_begin;
		typename S::iterator hint;
		if(seek==src_end){
			return;
		}

#if 1
		// find twice? should be optimized out (hopefully).
		// turns out faster in FI2, could be the callback...
		hint = tgt.find(*seek);
		if(hint == tgt.end()){
			(*cb)(*seek);
			hint = tgt.insert(*seek).first;
		}
#else
		bool done;
		boost::tie(hint, done) = tgt.insert(*seek);
		if(!done){ itested();
			// no need to callback...
		}else if(cb){ itested();
			(*cb)(*seek);
		}
#endif

		++hint;
		++seek;

		if(seek==src_end){
			return;
		}

		if(unlikely(hint==tgt.end())){itested();
		}else{

		while(true){
			assert(seek!=src_end);
			assert(hint!=tgt.end());
			if(unlikely(*seek<*hint)){
				if(cb){
					(*cb)(*seek);
				}
				size_t l=tgt.size();
				tgt.insert(hint,*seek);
				(void) l; assert(l+1==tgt.size());
				++seek;
				if(seek==src_end){itested();
					return;
				}
			}else if(unlikely(*seek==*hint)){
				++hint;
				++seek;
				if(unlikely(seek==src_end)){
					return;
				}else if(unlikely(hint==tgt.end())){
					break; // h_at_end
				}
			}else if(unlikely(*seek>100+*hint)){ // TODO: fine tuning?
				// hint = tgt.insert(hint,*seek); // slower
#if 1
				// is this slower?
				// maybe if the second find is NOT being opimized out...
				hint = tgt.find(*seek);

				if(hint == tgt.end()){itested();
					(*cb)(*seek);
					hint = tgt.insert(*seek).first;
				}
#else
				bool done;
				boost::tie(hint, done) = tgt.insert(*seek);
				if(!done){ itested();
					// no need to callback...
				}else if(cb){ itested();
					(*cb)(*seek);
				}
#endif
				++seek;
				++hint;

				if(unlikely(seek==src_end)){itested();
					return;
				}else if(unlikely(hint==tgt.end())){itested();
					break; // h_at_end
				}
			}else{
				while(*seek>*hint){
					++hint;
					if(hint==tgt.end()){itested();
						goto h_at_end;
					}else{
					}
				}
			}
		}
		}

	h_at_end:
		assert(hint==tgt.end());
		while(seek!=src_end){
			if(cb){
				(*cb)(*seek);
			}

			size_t l=tgt.size();
			tgt.insert(hint,*seek);
			(void) l; assert(l+1==tgt.size());
			++seek;
		}
	}
};

template<class arg>
class dummy{ //
	public:
		void operator()(arg){}
};

// S_iterator and T_iterator must be comparable...
// call back BEFORE insertion (if possible?)
template<class S, class S_iterator, class T_iterator, class CB=dummy<typename S::value_type> >
void zipfwdb4(S& tgt, S_iterator src_begin, T_iterator src_end, CB* cb=nullptr)
{
	return merge<S, S_iterator,T_iterator>::zipfwd(tgt, src_begin, src_end, cb);
}

// S_iterator and T_iterator must be comparable...
template<class S, class S_iterator, class T_iterator>
size_t count_missing(S const& tgt, S_iterator src_begin, T_iterator src_end)
{
	return merge<S,S_iterator,T_iterator>::count_missing(tgt, src_begin, src_end);
}

template<class S>
void zipfwdb42(S& tgt, S const& src)
{untested();
	zipfwdb42(tgt, src.begin(), src.end());
}
template<class S>
void zipfwdb4(S& tgt, S const& src)
{untested();
	zipfwdb4(tgt, src.begin(), src.end());
}


// insert items reversed with hints preceding insertion.
// (does not really help)
template<class S>
void ziprevb4(S& tgt, S const& src)
{untested();
	auto s=src.rbegin();
	auto hint=tgt.rbegin();
	if(s==src.rend()){untested();
		return;
	}else if(hint==tgt.rend()){untested();
		goto h_at_rend;
	}

	while(true){untested();
		assert(s!=src.rend());
		assert(hint!=tgt.rend());
		if(unlikely(*s>*hint)){untested();
			tgt.insert(reverse(hint,tgt),*s);
			++s;
			if(s==src.rend()){untested();
				return;
			}
		}else if(*s==*hint){;
			++hint;
			++s;
			if(hint==tgt.rend()){untested();
				break;
			}
			if(s==src.rend()){untested();
				return;
			}
		}

		while(*hint>*s){untested();
			++hint;
			if(hint==tgt.rend()){untested();
				goto h_at_rend;
			}else{untested();
			}
		}
	}

h_at_rend:
	assert(hint==tgt.rend());
	while(s!=src.rend()){untested();
		tgt.insert(reverse(hint,tgt),*s);
		++s;
	}
}
#endif
