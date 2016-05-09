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
 * graph abstraction with low-level access
 */
#ifndef GALA_GRAPH_H
#define GALA_GRAPH_H

#include <cstdlib>
#include <vector>
#include <map>
#include <forward_list>

#include <boost/iterator/counting_iterator.hpp>
#include <boost/tuple/tuple.hpp>

#include "degs.h"
#include "trace.h"
#include <assert.h>

/*--------------------------------------------------------------------------*/
#ifndef GALA_DEFAULT_OVERRIDE
# include <set>
# include <vector>
# define GALA_DEFAULT_SET std::set
# define GALA_DEFAULT_VECTOR std::vector
#endif
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
namespace gala{ //
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#define VCTtemplate template< \
                     template<class T, typename... > class ECT, \
                     template<class T, typename... > class VCT, \
                     class VDP, \
                     template<class G, class ... > class DEG \
                    >
#define SGARGS ECT,VCT,VDP,DEG
/*--------------------------------------------------------------------------*/
#define VCTtemplateP template< \
                     template<class T, typename... > class ECT, \
                     template<class T, typename... > class VCT \
                     template<class G, class ... > class DEG \
                     >
#define SGARGSP ECT,VCT,DEG
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
namespace bits{ //
/*--------------------------------------------------------------------------*/
template<class VDP>
struct vertex_helper{ //
	template<class T, class V, class VC>
	static void insert(T& v, V& w, VC* wp)
	{ itested();
		v.insert(w);
	}
	template<class VL>
	static bool is_valid(VDP const& v, VL const& _v)
	{ untested();
		return v<_v.size();
	}
	template<class E, class S>
	static void rebase(E& e, S const& s, intptr_t delta)
	{ untested();
		e=s;
	}
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template<>
struct vertex_helper<void*>{ //
	template<class T, class V, class VC>
	static void insert(T& v, V& w, VC* wp)
	{ itested();
		v.n.insert(wp);
	}
	template<class VL>
	static bool is_valid(typename VL::value_type const* v, VL const& _v)
	{ itested();
		intptr_t B= intptr_t(v) - intptr_t(&*_v.begin());
		if(!std::is_same<std::random_access_iterator_tag,
		                 typename VL::iterator::iterator_category > :: value) { untested();
			return true; // don't know...
		}else if(B<0){untested();
			return false;
		}else if(B>=intptr_t(&*_v.end())) {untested();
			return false;
		}else if(B%sizeof(typename VL::value_type)) { untested();
			return false;
		}else{ itested();
			return true;
		}
	}
	template<class T, class S>
	static void rebase(T& tgt, S const& src, intptr_t delta)
	{
		tgt.clear();
		for(auto i : src) {
#ifdef NOHINT_REBASE
			tgt.insert( typename T::value_type((intptr_t(i)+delta)));
#else // faster. check applicability
			auto e=tgt.end();
			tgt.insert(e, (typename T::value_type)((intptr_t(i)+delta)));
#endif
		}
	}
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template<class c_iter_tag, class VDP>
struct iter_helper{ //

	template<class iter, class VL>
	static size_t fill_pos(iter first, iter last, size_t nv, VL& _v)
	{
		size_t c=0;
		for(;first!=last; ++first){
			unsigned v=first->first;
			unsigned w=first->second;
			assert(v<nv);
			assert(w<nv);
			vertex_helper<VDP>::insert(_v[v], w, &_v[w]);
			vertex_helper<VDP>::insert(_v[w], v, &_v[v]);
			++c;
		}
		return c;
	}

};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template<class VDP>
struct iter_helper<std::bidirectional_iterator_tag, VDP>{ //

template<class iter, class VL>
static size_t fill_pos(iter first, iter last, size_t nv, VL& _v)
{
	typedef typename VL::value_type v_t;
	GALA_DEFAULT_VECTOR<v_t*> vmap(nv);
	assert(nv==_v.size());
	size_t index=0;
	for(auto& i : _v){
		vmap[index++] = &i;
	}
	assert(index==_v.size());
	size_t c=0;
	for(;first!=last; ++first){
		unsigned v=first->first;
		unsigned w=first->second;
		assert(v<nv);
		assert(w<nv);
		v_t* _V = vmap[v];
		v_t* _W = vmap[w];
		v_t& V = *_V;
		v_t& W = *_W;
		vertex_helper<VDP>::insert(V, W, _W);
		vertex_helper<VDP>::insert(W, V, _V);
		++c;
	}
	return c;
}
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
// select vertex type depending on 3rd template parameter
template<template<class T, typename... > class ECT, class VDP>
struct vertex_selector{ //
	typedef VDP type;
	typedef const VDP const_type;
	typedef ECT<type> stype;
	typedef VDP vertices_size_type;
	typedef size_t edges_size_type; // TODO.
	typedef unsigned vertex_index_type; // INCOMPLETE
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template< template<class T, typename... > class ECT >
struct vertex_selector<ECT, void*>{ //
	typedef struct vertex_{ //
		ECT<vertex_*> n;
		size_t size() const { return n.size(); }
	} vertex_;
	typedef vertex_* type;
	typedef vertex_ const* const_type;
	typedef vertex_ stype; // storage type
	typedef uintptr_t vertices_size_type;
	typedef size_t edges_size_type; // TODO.
	typedef unsigned vertex_index_type; // INCOMPLETE
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#define STtemplate template< \
                   template<class T, typename... > class ECT, \
                   template<class T, typename... > class VCT, \
                   class VDP \
                   >
#define STARGS ECT, VCT, VDP
/*--------------------------------------------------------------------------*/
STtemplate
struct storage_base{ //
	typedef vertex_selector<ECT,VDP> vs;
	typedef typename vs::stype vertex_;
	typedef VCT<vertex_> container_type;
	typedef typename vs::vertex_index_type vertex_index_type;
	static vertex_index_type num_edges(vertex_index_type n, container_type const&)
	{
		return n;
	}
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
STtemplate
struct storage : storage_base<STARGS>{ //
	using typename storage_base<STARGS>::vs;
	using typename storage_base<STARGS>::vertex_;
	using typename storage_base<STARGS>::container_type;
	typedef typename vs::type vertex_type;
	typedef typename vs::vertex_index_type vertex_index_type;
	typedef typename vs::const_type const_vertex_type;
	typedef ECT<vertex_type> edge_container_type;
	typedef VCT<vertex_> VL;
	static bool need_rewire()
	{
		return false;
	}
	static vertex_type new_node(container_type& _v)
	{
		vertex_ n;
		auto s=_v.size();
		_v.push_back(n);
		return s;
	}
	static VDP degree(const VDP v, container_type const& _v)
	{
		return _v[v].size();
	}
	static VDP degree(container_type const& _v)
	{
		vertex_index_type ret=0;
		for(auto const& e : _v){
			vertex_index_type d = e.size();
			if(d>ret) ret=d;
		}
		return ret;
	}
	static void clear_vertex(const VDP v, container_type& _v)
	{
		_v[v].clear();
	}
	static edge_container_type& out_edges(vertex_type& v, container_type& _v)
	{
		return _v[v];
	}
	static edge_container_type& out_edges(const_vertex_type& v, container_type& _v)
	{
		return _v[v];
	}
	static const edge_container_type& out_edges(const_vertex_type& v, const container_type& _v)
	{
		return _v[v];
	}
	static void add_pos_edge(vertex_index_type v, vertex_index_type w,
	                         container_type& _v)
	{
		assert(v<w); // for now
		_v[v].insert(w);
		_v[w].insert(v);
	}
	template<class E, class S>
	static void rebase(E& e, S const& s, intptr_t delta)
	{ incomplete();
		e = s;
	}
}; // storage<VDP>
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template<class A, class B>
static void rewire_nodes(A& new_vl, B offset)
{ untested();
	for(auto& node : new_vl){ incomplete();
		node.n.insert(NULL);
		auto& N = node.n;
		for(auto& neigh : N) {
			*(const_cast<typename A::value_type**>(&neigh)) =
			      (typename A::value_type*) ( uintptr_t(neigh) + uintptr_t(offset));
		}
	}
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template<template<class T, typename... > class ECT,
         template<class T, typename... > class VCT>
struct storage<ECT, VCT, void*> : public storage_base<ECT, VCT, void*>{ //
	typedef void* VDP;
	typedef typename bits::vertex_selector<ECT,VDP>::stype vertex_;
	typedef typename bits::vertex_selector<ECT,VDP>::type vertex_type;
	typedef typename bits::vertex_selector<ECT,VDP>::vertex_index_type vertex_index_type;
	typedef typename bits::vertex_selector<ECT,VDP>::const_type const_vertex_type;
	typedef VCT<vertex_> container_type;
	typedef ECT<vertex_type> edge_container_type;
	typedef VCT<vertex_> VL;
	static bool need_rewire()
	{
		return true;
	}
	static vertex_type new_node(container_type& _v)
	{ incomplete();
		unsigned int s = _v.size();
		auto old_begin=_v.begin();
		_v.resize(s+1);
		vertex_type offset = (vertex_type) (uintptr_t(&*_v.begin()) - uintptr_t(&*old_begin));
		if(!offset){
		}else{ incomplete();
			std::cerr << "hmm " << _v.size() << "\n";
			rewire_nodes(_v, offset);
		}
		incomplete();
		return &_v.back();
	}
	static vertex_index_type degree(const vertex_type& v, container_type const&){ // ??
		return v->n.size();
	}
	static vertex_index_type degree(const_vertex_type& v, container_type const&){
		return v->n.size();
	}
	static vertex_index_type degree(container_type const& _v)
	{
		vertex_index_type ret=0;
		typename container_type::const_iterator v = _v.begin();
		typename container_type::const_iterator e = _v.end();
		for(; v!=e ; ++v){
			vertex_index_type d = v->n.size();
			if(d>ret) ret=d;
		}
		return ret;
	}
	static void clear_vertex(const vertex_type v, container_type&)
	{
		v->n.clear();
	}
	static edge_container_type& out_edges(vertex_& v, container_type&)
	{
		return v.n;
	}
	static const edge_container_type& out_edges(vertex_& v, const container_type&)
	{
		return v.n;
	}
	static edge_container_type& out_edges(vertex_type& v, container_type&)
	{
		return v->n;
	}
	static const edge_container_type& out_edges(const_vertex_type& v, const container_type&)
	{
		return v->n;
	}
public:
	static void add_pos_edge(vertex_index_type v, vertex_index_type w,
	                         container_type& _v)
	{
		assert(v<w); // for now
		_v[v].n.insert(&_v[w]);
		_v[w].n.insert(&_v[v]);
	}
	// copy over outedges.
	template<class T, class S>
	static void rebase(T& tgt, S const& src, intptr_t delta)
	{ incomplete();
		tgt.clear();
		auto e=tgt.end();
		for(auto i : src) {
			tgt.insert( (vertex_*) (intptr_t(i)+delta), e);
		}
	}
}; //storage<void*>
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
STtemplate
struct iter{ //
	typedef boost::counting_iterator<VDP> vertex_iterator;
	typedef boost::counting_iterator<VDP> const_vertex_iterator; // ?
	static vertex_iterator vbegin(typename storage<STARGS>::VL&)
	{
		return vertex_iterator(0);
	}
	static const_vertex_iterator vbegin(const typename storage<STARGS>::VL&)
	{
		return vertex_iterator(0);
	}
	static vertex_iterator vend(typename storage<STARGS>::VL& v)
	{
		return vertex_iterator(v.size());
	}
	static const vertex_iterator vend(const typename storage<STARGS>::VL& v)
	{
		return vertex_iterator(v.size());
	}
	static VDP deref(vertex_iterator v)
	{
		return *v;
	}
	// FIXME: is this "iter"?
	typedef typename bits::vertex_selector<ECT,VDP>::type vertex_type;
	typedef typename bits::vertex_selector<ECT,VDP>::stype vertex_;
	typedef VCT<vertex_> container_type;
	static unsigned pos(const vertex_type v, container_type const& _v)
	{
		return v;
	}
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template<template<class T, typename... > class ECT,
         template<class T, typename... > class VCT>
struct iter<ECT, VCT, void*>{ //
	typedef void* VDP;
	typedef typename storage<STARGS>::VL::iterator vertex_iterator;
	typedef typename storage<STARGS>::VL::const_iterator const_vertex_iterator;
	typedef typename vertex_selector<ECT,VDP>::type vertex_type;
	typedef typename vertex_selector<ECT,VDP>::const_type const_vertex;

	static const_vertex_iterator vbegin(typename storage<STARGS>::VL const& v)
	{
		return v.begin();
	}
	static vertex_iterator vbegin(typename storage<STARGS>::VL& v)
	{
		return v.begin();
	}
	static const_vertex_iterator vend(typename storage<STARGS>::VL const& v)
	{
		return v.end();
	}
	static vertex_iterator vend(typename storage<STARGS>::VL& v)
	{
		return v.end();
	}
	static vertex_type deref(vertex_iterator& v){
		return &*v;
	}
	// needed?
	static const vertex_type deref(const vertex_iterator& v){
		return &*v;
	}
	static const_vertex deref(const_vertex_iterator& v){
		return &*v;
	}

// FIXME: is this "iter"?
// BUG:: type?
// fixme: VDP!=vector?!
	typedef typename bits::vertex_selector<ECT,VDP>::stype vertex_;

	typedef VCT<vertex_> container_type;
	static unsigned /*hmmm*/ pos(const vertex_type v, container_type const& _v)
	{
		vertex_ const* B = &*_v.begin();
		vertex_ * const V = v;
		intptr_t delta = intptr_t(V) - intptr_t(B);
		assert(delta>=0); // fails for sl. (incomplete).
		unsigned r=delta/sizeof(vertex_);
		assert(r<_v.size());
		return r;
	}
}; // iter<void*>
}// bits
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template< template<class T, typename... > class ECT=GALA_DEFAULT_SET,
          template<class T, typename... > class VCT=GALA_DEFAULT_VECTOR,
          typename VDP=void*,
          template<class G, class... > class DEG=default_DEGS >
class graph{ //
public: // types
	template<class T>
	using vertex_container_t = VCT<T>;

	template<class T>
	using edgecontainer = ECT<T>;
	
	using vs = bits::vertex_selector<ECT,VDP>;

	typedef typename vs::type vertex_type;
	typedef typename vs::const_type const_vertex_type;
	typedef typename vs::stype vertex_;
	typedef typename vs::vertices_size_type vertices_size_type;
	typedef typename vs::edges_size_type edges_size_type;
	typedef typename vs::vertex_index_type vertex_index_type;
// private: hmm not yet.
	typedef typename bits::storage<STARGS> storage;
	typedef typename bits::iter<STARGS> iter;
	typedef edgecontainer<vertex_type> EL;
public:
	typedef typename storage::container_type VL;
	typedef typename storage::container_type vertex_container_type;

	typedef typename iter::vertex_iterator iterator;
	typedef typename iter::const_vertex_iterator const_iterator;

	typedef typename EL::iterator out_vertex_iterator;
	typedef typename EL::const_iterator out_vertex_const_iterator;

	typedef std::pair<vertex_type, vertex_type> edge; // FIXME: remove.
	typedef std::pair<vertex_type, vertex_type> edge_type;

	typedef std::pair<iterator, out_vertex_iterator> edge_iterator;
public: // construct
	graph(const graph& x)
	{
		assign_(x);
		assert(num_vertices()==x.num_vertices());
		assert(_num_edges==x._num_edges);
#ifndef NDEBUG
		for(auto i = begin(); i!=end(); ++i){
			assert(is_valid(iter::deref(i)));
			//   		for(auto& j : vertices()){
			//   //			assert(is_edge(&j,&i) == is_edge(&i,&j));
			//   		}
		}
#endif
	}
public: // move
	graph(graph&& x)
	    : _v(std::move(x._v)),
	      _num_edges(x._num_edges)
	{
		assert(num_vertices()==x.num_vertices());
		for(auto i = begin(); i!=end(); ++i){
			assert(is_valid(iter::deref(i)));
#ifdef MORE_DEBUG
			for(auto& j : vertices()){
				assert(is_edge(&j,&i) == is_edge(&i,&j));
			}
#endif
		}
	}
	graph(vertices_size_type n=0, edges_size_type m=0)
	    : _v(n), _num_edges(0)
	{
		if(m){
			// incomplete();
			// prealloc...
		}
	}
public: // Required by Iterator Constructible Graph
	template <class EdgeIterator>
	graph(EdgeIterator first, EdgeIterator last,
	      vertices_size_type nv, edges_size_type ne=0);
public: //assign
	graph& operator=(graph<SGARGS> const& x);
	// VCTtemplate?
	// graph& operator=(graph<...??...> const&)
	graph& operator=(graph&& x)
	{
#ifndef NDEBUG
		unsigned c=0;
		for(auto& i : x._v){
			c += i.size();
		}
		assert(2*x.num_edges() == c);
#endif
		if (&x!=this){
			_num_edges = x._num_edges;
			_v = std::move(x._v);
//			x._v.clear(); // hmm, does not work for sl
			x._num_edges = 0;
		}else{
		}
		trace2("move assign", size_t(num_vertices()), size_t(num_edges()));
#ifndef NDEBUG
		c=0;
		for(auto& i : _v){
			c += i.size();
		}
		assert(2*num_edges() == c);
#endif
		return *this;
	}
private:
	void assign_(graph const& G);
public: // construct
	// construct a graph from a boost graph
	// warning: assuming vertex_descriptors are 0,1,2 ...
	template <typename G_t>
	graph(G_t const &G)
	    : _num_edges(0)
	{
		assign(G);
	}
	template <typename G_t>
	void assign(G_t const& G);
public: // iterators
	iterator begin()
	{
		return iter::vbegin(_v);
	}
	iterator end()
	{
		return iter::vend(_v);
	}
	const_iterator begin() const
	{
		return iter::vbegin(_v);
	}
	const_iterator end() const
	{
		return iter::vend(_v);
	}
	// will not work for non-vector VCT...
	// only for sv*
	unsigned position(const vertex_type v) const
	{
		return iter::pos(v, _v);
	}
public: // BUG: should not expose this...?
	vertex_container_type& vertices()
	{ // incomplete(); later
		return _v;
	}
	const vertex_container_type& vertices() const
	{ // incomplete(); later
		return _v;
	}
public:
	void clear()
	{
		_v.resize(0);
		_num_edges = 0;
	}
	bool is_edge(const_vertex_type a, const_vertex_type b) const
	{
		return a->n.find(const_cast<vertex_type>(b)) != a->n.end();
	}
private:
	//O(log d), where d is the degree of a
	void remove_edge_single(vertex_type a, vertex_type b)
	{
		assert(a!=b);
		out_edges(a).erase(b);
		--_num_edges;
	}
private: // required by contruct from iterator
	// add edge from vertex positions \in [0, nv)
	void add_pos_edge(vertex_index_type a, vertex_index_type b)
	{
		storage::add_pos_edge(a, b, _v);
	}
	// same, but from strings.
	void add_pos_edge(const char* a, const char* b)
	{
		storage::add_pos_edge(a, b, _v);
	}
public:
	//O(n) due to (possible) vector resizing
	//amortized O(log n), possibly.
	vertex_type new_node();
	//O(1)
	EL& out_edges(vertex_type& v);
	EL& out_edges(vertex_type& v) const;
	EL& out_edges(const_vertex_type& /*v*/);
	const EL& out_edges(const_vertex_type& /*v*/) const;
	//O(1)
	vertex_index_type num_vertices() const
	{
		return _v.size();
	}
	//O(1)
	edges_size_type num_edges() const
	{
		return storage::num_edges(_num_edges, _v);
	}
public:
	//O(log max{d_1, d_2}), where d_1 is the degree of a and d_2 is the degree of b
	std::pair<edge,bool> add_edge(vertex_type a, vertex_type b)
	{
		vertex_type* A=&a;
		vertex_type* B=&b;
#ifdef ADDEDGESWAP
		// does not really help...
		if(out_edges(a).size()>out_edges(b).size()){ untested();
			std::swap(A,B);
		}else{untested();
		}
#endif
		size_t s = out_edges(*A).size();
		out_edges(*A).insert(*B);
		// since the graph is undirected,
		// if (a, b) already exists, then (b, a) does too
		bool added=false;
		if(s == out_edges(*A).size()){
		}else{
			out_edges(*B).insert(*A);
			++_num_edges;
			added = true;
		}
		return std::make_pair(edge(*A, *B), added);
	}
	// O(log max{d_1, d_2}), where d_1 is the degree of a and d_2 is the degree of b
	void remove_edge(vertex_type a, vertex_type b)
	{
		// must exist, for now.
		assert(exists_edge(a,b));
		assert(exists_edge(b,a));
		out_edges(a).erase(b);
		out_edges(b).erase(a);
		assert(_num_edges);
		--_num_edges;
	}
	void contract(vertex_type& vd, vertex_type into);
	vertex_index_type degree(const_vertex_type who) const
	{
		return storage::degree(who, _v);
	}
	// degree of the graph, O(V)
	vertex_index_type degree() const
	{
		return storage::degree(_v);
	}
public:
	// O(n + deg(v)*log(D)), where D is the maximum of the degrees of the
	// neighbours of v
	void clear_vertex(vertex_type v)
	{
		unsigned c = 0;
		for(auto& nIt : out_edges(v)){
			remove_edge_single(nIt, v);
			++c;
		}
		assert(c==out_edges(v).size());
		storage::clear_vertex(v, _v);
	}
	void remove_vertex(vertex_type)
	{ incomplete();
//			_v.erase(*v);
	}
	bool is_valid(const_vertex_type v) const
	{ itested();
		return bits::vertex_helper<VDP>::is_valid(v,_v);
	}
	void remove_vertex(iterator v);
	// O(log d), where d is the degree of v
	bool exists_edge(vertex_type v, vertex_type w)
	{
		assert((out_edges(v).find(w) == out_edges(v).end())
				== (out_edges(w).find(v) == out_edges(w).end()));
		return (out_edges(v).find(w) != out_edges(v).end());
	}

		/*
			template<class S>
			void print(S& s){
			for(unsigned int i = 0; i < num_vertices(); i++){
			for(edgecontainer<size_t>::iterator nIt = _v[i].begin(); nIt != _v[i].end(); nIt++) {
			s << i << " -- " << *nIt << " ";
			}
			s << std::endl;
			}
			}
			*/
	bool graphviz_parse() const;
private:
	void rewire_nodes(vertex_container_type& new_vl, vertex_type offset);
public: // BUG: private. use "friend"...
	vertex_container_type _v;
	size_t _num_edges;
};
/*--------------------------------------------------------------------------*/
VCTtemplate
template <class EdgeIterator>
graph<SGARGS>::graph(EdgeIterator first, EdgeIterator last,
                     vertices_size_type nv, edges_size_type ne)
    : graph(nv, ne)
{
	unsigned c;
	typedef typename iter::vertex_iterator::iterator_category iterator_category;
	c = bits::iter_helper<iterator_category, VDP>::fill_pos(first, last, nv, _v);
	trace2("EdgeIterator init", ne, c);
	assert(!ne || ne==c); // unique edges? for now.
	_num_edges = c;

#ifndef NDEBUG
	c = 0;
	for(auto& i : _v){
		c += i.size();
	}
	assert(2*num_edges() == c);
#endif
}
/*--------------------------------------------------------------------------*/
VCTtemplate
typename graph<SGARGS>::EL const&
    graph<SGARGS>::out_edges(const_vertex_type& v) const
{
	return storage::out_edges(v, _v);
}
/*--------------------------------------------------------------------------*/
VCTtemplate
typename graph<SGARGS>::EL&
	graph<SGARGS>::out_edges(const_vertex_type& v)
{
	return storage::out_edges(v, _v);
}
/*--------------------------------------------------------------------------*/
VCTtemplate
typename graph<SGARGS>::EL&
	graph<SGARGS>::out_edges(vertex_type& v)
{
	return storage::out_edges(v, _v);
}
/*--------------------------------------------------------------------------*/
VCTtemplate
typename graph<SGARGS>::EL& // const?!
	graph<SGARGS>::out_edges(vertex_type& v) const
{
	auto&_V = const_cast<graph<SGARGS>*>(this)->_v;
	return storage::out_edges(v, _V);
}
/*--------------------------------------------------------------------------*/
// VCTtemplate
// const typename graph<SGARGS>::EL&
// 	graph<SGARGS>::out_edges(const typename graph<SGARGS>::vertex_type& v) const
// {
// 	return storage::out_edges(v);
// }
/*--------------------------------------------------------------------------*/
VCTtemplate
graph<SGARGS>& graph<SGARGS>::operator=(graph<SGARGS> const& x)
{
	if (&x == this) {
	}else if (num_vertices()==0){ itested();
		assign_(x);
	}else if (num_vertices()!=x.num_vertices()){ incomplete();
	}else{
		// why not assign_?
		const_iterator b = begin();
		const_iterator s = x.begin();
		intptr_t delta = intptr_t(&*b) - intptr_t(&*s);
		trace1("op=", delta);
		iterator v = begin();
		iterator e = end();
		_num_edges = x._num_edges;
		for(; v!=e ; ++v){
			vertex_type vd = iter::deref(v);
			const_vertex_type sd = iter::deref(s);
			EL& E = out_edges(vd); // ?!
			EL const& S = x.out_edges(sd); // ?!
			bits::vertex_helper<VDP>::rebase(E, S, delta);
#if 0
			incomplete();// does not work for stx

			EL& MS = const_cast<EL&>(S); // HACK!?
			E = MS;
			//for(vertex& h : out_edges(vd))
			// works for std::set only...
			for(typename EL::iterator i=vd->n.begin(); i!=vd->n.end(); ++i){
				vertex* j= const_cast<vertex*>(&(*i));
				assert(*j >= &*x.begin());
				assert(*j < &*x.end());
				assert(*i >= &*x.begin());
				assert(*i < &*x.end());

				*j = vertex(intptr_t(*j) + delta);

				assert(*j >= &*begin());
				assert(*j < &*end());
			}
			for(typename EL::iterator i=vd->n.begin(); i!=vd->n.end(); ++i){
				assert(*i >= &*begin());
				assert(*i < &*end());
			}
#endif
			++s;
		}
	}
	return *this;
}
/*--------------------------------------------------------------------------*/
// FIXME: likely simpler for ssg
// FIXME: operator= ...
VCTtemplate
void graph<SGARGS>::assign_(graph<SGARGS> const& G)
{
	size_t nv = G.num_vertices();
	size_t ne = G.num_edges();
	trace4("assign_",nv, ne, num_vertices(), num_edges());
	_v.resize(nv);
	assert(_v.size()==nv);
	vertex_type map[nv];
	std::map<vertex_type, size_t> reverse_map;
	size_t i=0;
	_num_edges = 0;

	// not necessary for vector...
	for(iterator v=begin(); v!=end(); ++v){
		// for(auto& v : vertices())
		map[i] = iter::deref(v);
		++i;
		assert(i<=nv);
	}
	trace2("resize v", i, nv);
	assert(i==nv);

	graph<SGARGS>* GG = const_cast< graph<SGARGS>* >(&G);
	i = 0;
	// FIXME: use const_iter
	for( iterator v=GG->begin(); v!=GG->end(); ++v)
	{
		reverse_map[iter::deref(v)] = i;
		++i;
	}

	assert(_num_edges == 0);
	assert(i==nv);

	assert(_num_edges == 0);
	for(iterator V=GG->begin(); V!=GG->end(); ++V){
		vertex_type v=iter::deref(V);
		for(vertex_type w : G.out_edges(v)) {
			assert(w!=v);
			assert(reverse_map[v]<num_vertices());
			assert(reverse_map[w]<num_vertices());
			add_edge(map[reverse_map[v]],
			         map[reverse_map[w]]);
//			++_num_edges;
		}
	}
	if(_num_edges == 2*ne){
	}else if(_num_edges == ne){
	}else{
		std::cerr << "assign_ oops " << _num_edges << ":" << ne << "\n";
	}
	assert(_num_edges == ne || _num_edges == 2*ne);
	assert(num_vertices() == nv);
	_num_edges = ne;


#ifndef NDEBUG
	// incomplete...
	for(auto i = begin(); i!=end(); ++i){
		assert(is_valid(iter::deref(i)));
		//   		for(auto& j : vertices()){
		//   //			assert(is_edge(&j,&i) == is_edge(&i,&j));
		//   		}
	}
#endif
	trace4("assign_ done",nv, ne, num_vertices(), num_edges());
}
/*--------------------------------------------------------------------------*/
// TODO: runtime info?
VCTtemplate
void graph<SGARGS>::contract(vertex_type& vd, vertex_type into)
{
	assert(vd);
	assert(into);

	auto x=vd->n.find(into);
	if(x!=vd->n.end()){
		// delete edge in between
		vd->n.erase(x); // can we avoid it? we will vd.clear() later on.
		--_num_edges;
		x = into->n.find(vd);
		assert(x!=into->n.end());
		into->n.erase(x);
	}else{
	}

	// merge outnodes.
	auto I = vd->n.begin();
	auto E = vd->n.end();
	into->n.insert(I,E);

	for(auto& I : vd->n){ // untested();
		assert(&*I != &*into);
		bool inserted_new = I->n.insert(into).second;
		if(inserted_new){
			// edge has moved.
		}else{
			// there are two, one of which will be gone.
			--_num_edges;
		}

		x = I->n.find(vd);
		assert(x!=I->n.end());
		I->n.erase(x);
	}

	// urgs. could be unnecessary
	// case of erase=true (next function)
	vd->n.clear();
}
/*--------------------------------------------------------------------------*/
VCTtemplate
inline void graph<SGARGS>::remove_vertex(iterator v)
{ untested();
	if(storage::need_rewire()){
		incomplete();
	}
	_v.erase(v);
}
/*--------------------------------------------------------------------------*/
VCTtemplate
inline typename graph<SGARGS>::vertex_type graph<SGARGS>::new_node()
{
	return storage::new_node(_v);
}
/*--------------------------------------------------------------------------*/
#if 0
VCTtemplate
static inline size_t num_edges(graph<SGARGS> &G)
{
    return G.num_edges();
}
/*--------------------------------------------------------------------------*/
VCTtemplate
static inline size_t num_edges(graph<SGARGS> const &G)
{
    return G.num_edges();
}
/*--------------------------------------------------------------------------*/
// VCTtemplate
// static inline void remove_edge(vertex_type v,
// 		typename graph<SGARGS>::vertex w, graph<SGARGS> &G)
// {
//     G.remove_edge(v, w);
// }
/*--------------------------------------------------------------------------*/
// VCTtemplate
// static inline size_t out_degree(const /*_*/ vertex_type v, graph<SGARGS> const& G)
// {
//     return G.degree(v);
// }
#endif
/*--------------------------------------------------------------------------*/
VCTtemplate
static inline boost::tuple<typename graph<SGARGS>::out_iterator,
                           typename graph<SGARGS>::out_iterator>
adjacent_vertices(typename graph<SGARGS>::vertex v, graph<SGARGS> &G)
{
	typedef typename graph<SGARGS>::out_iterator Iter;
	typedef boost::tuple<Iter, Iter> P;
	auto& o=G.out_edges(v);
	return P(o.begin(), o.end());
}
/*--------------------------------------------------------------------------*/
VCTtemplate
static inline void clear_vertex(typename graph<SGARGS>::vertex v, graph<SGARGS> &G)
{
    G.clear_vertex(v);
}
/*--------------------------------------------------------------------------*/
VCTtemplate
static inline std::pair<bool, bool> edge(typename graph<SGARGS>::vertex v,
		typename graph<SGARGS>::vertex w, graph<SGARGS> &G)
{
    return std::pair<bool, bool>(true, G.exists_edge(v, w));
}
/*--------------------------------------------------------------------------*/
namespace bits{
template<>
inline void storage<GALA_DEFAULT_SET, GALA_DEFAULT_VECTOR, void*>::add_pos_edge(
      typename storage<GALA_DEFAULT_SET, GALA_DEFAULT_VECTOR, void*>::vertex_index_type v,
      typename storage<GALA_DEFAULT_SET, GALA_DEFAULT_VECTOR, void*>::vertex_index_type w,
      typename storage<GALA_DEFAULT_SET, GALA_DEFAULT_VECTOR, void*>::container_type& _v)
{
	trace2("pos edge", v, w);
	assert(v<w); // for now
	_v[v].n.insert(&_v[w]);
	_v[w].n.insert(&_v[v]);
}
/*--------------------------------------------------------------------------*/
// FIXME: implement for all ECTs...
// template< template<class T, class... > class ECT >
// inline void storage<ECT, std::vector, void*>::add_pos_edge(
//       typename storage<ECT, std::vector, void*>::vertex_index_type v,
//       typename storage<ECT, std::vector, void*>::vertex_index_type w,
//       typename storage<ECT, std::vector, void*>::container_type& _v)
// {
// 	_v[v].insert(&_v[w]);
// 	_v[w].insert(&_v[v]);
// }
/*--------------------------------------------------------------------------*/
} // bits
/*--------------------------------------------------------------------------*/
} // gala
/*--------------------------------------------------------------------------*/
#endif
