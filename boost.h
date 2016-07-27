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
 * boost interface for gala graph
 */
#ifndef GALA_BOOST_H
#define GALA_BOOST_H

#include <gala/graph.h>
#include <boost/graph/graph_traits.hpp> // bidirectional_graph_tag
#include <boost/graph/properties.hpp>

namespace boost { //
	struct simplegraph_traversal_category :
		public virtual bidirectional_graph_tag, // ?!
		public virtual adjacency_graph_tag,
		public virtual edge_list_graph_tag,
		public virtual vertex_list_graph_tag { };

	template<class VDP>
	struct myVDP{
		typedef VDP type;
	};

	template<>
	struct myVDP<void*>{
		typedef uintptr_t type;
	};

	VCTtemplate
	struct graph_traits<gala::graph<SGARGS> > { //
		typedef typename myVDP<VDP>::type vertices_size_type;
		typedef size_t edges_size_type; // FIXME: use v^2
		typedef typename myVDP<VDP>::type degree_size_type;

		typedef typename gala::graph<SGARGS>::vertex_type vertex_descriptor;
		typedef typename gala::graph<SGARGS>::edge_type edge_descriptor;

		typedef undirected_tag directed_category;
		typedef simplegraph_traversal_category traversal_category;
		typedef disallow_parallel_edge_tag edge_parallel_category;

		class vertex_iterator
		    : public iterator_facade<
		        vertex_iterator, //  <= Derived
		        typename gala::graph<SGARGS>::iterator, // <= value_type
		        typename gala::graph<SGARGS>::iterator::iterator_category,
		        typename gala::graph<SGARGS>::vertex_type, // <= value&
		        ptrdiff_t /*Difference*/
		    >{
		public:
			typedef gala::graph<SGARGS> G;
			typedef typename gala::graph<SGARGS>::vertex_type reference;
			typedef typename gala::graph<SGARGS>::iterator base_type;
			typedef ptrdiff_t difference_type; // in iterator_facade?

			void advance(size_t n)
			{
			// base+=n;
				std::advance(base,n);
			}
			vertex_iterator(typename gala::graph<SGARGS>::iterator
			    e=typename gala::graph<SGARGS>::iterator(),
			    const gala::graph<SGARGS>*g=NULL)
			        : base(e)
			{
				(void)g;
			}
			vertex_iterator(const vertex_iterator& p)
			    : base(p.base) /*,g(p.g)*/
			{
			}

			typename gala::graph<SGARGS>::iterator value(){return base;}

#if 0 // ?!
				vertex_iterator&& operator+(size_t n)
				{ untested();
					vertex_iterator a=*this;
					a.advance(n);
					return std::move(a);
				}
#else
			vertex_iterator operator+(size_t n)
			{
				vertex_iterator a=*this;
				a.advance(n);
				return a;
			}
#endif

		private:
			// reference
			reference dereference()
			{ untested();
				return &*base;
			}
			const reference dereference() const
			{ // untested();
				// BUG. don't use detail...
				return G::iter::deref(base);
			}

			bool equal(const vertex_iterator& other) const
			{
				return base == other.base;
			}
			void increment()
			{
				++base;
			}
			void decrement()
			{
				--base;
			}
		private:
			//value_type
			base_type base;
#ifdef EXTRA_G
			const ::gala::graph<SGARGS>* _g; //needed for vertex descriptor??
#endif
			friend class iterator_core_access;
		};

		class adjacency_iterator
		    : public iterator_facade<
		        adjacency_iterator,
		        typename gala::graph<SGARGS>::out_vertex_iterator, // value_type
//		        bidirectional_traversal_tag,
				  std::input_iterator_tag, // for now.
		        /*const*/ typename ::gala::graph<SGARGS>::vertex_type, // <= reference
		        typename ::gala::graph<SGARGS>::vertex_type* // difference_type
		    >{ //
		public:
			typedef typename gala::graph<SGARGS>::out_vertex_iterator value_type;
			typedef typename gala::graph<SGARGS>::out_vertex_const_iterator const_value_type;
			typedef typename gala::graph<SGARGS>::vertex_type reference;
//		   typedef typename gala::graph<SGARGS>::vertex_type* difference_type; ??
		   typedef intptr_t difference_type; // why?
		public:
			adjacency_iterator(typename ::gala::graph<SGARGS>::out_vertex_iterator
			    e=typename gala::graph<SGARGS>::out_vertex_iterator()) : _base(e)
			{
			}
			bool operator==(const_value_type other) const
			{
				return const_value_type(_base) == other;
			}
			bool operator==(const adjacency_iterator& other) const
			{
				return _base == other._base;
			}
			bool operator!=(const adjacency_iterator& other) const
			{
				return _base != other._base;
			}
		private: // reference
			reference dereference() const
			{
				return *_base;
			}

			bool equal(const adjacency_iterator& other) const
			{
				return _base == other._base;
			}
			void increment()
			{
				++_base;
			}
			void decrement()
			{ untested();
				--_base;
			}
		public: // bug? use friends...
			const value_type base(){return _base;}
		private:
			value_type _base;
			friend class iterator_core_access;
		};

		class edge_iterator
			: public iterator_facade<
			             edge_iterator,
			             typename gala::graph<SGARGS>::edge_iterator, // value_type
			             bidirectional_traversal_tag,
			             const typename gala::graph<SGARGS>::edge_type, // reference (?)
			             const typename gala::graph<SGARGS>::out_vertex_iterator*>
		{ //
		public: // types
			typedef gala::graph<SGARGS> G;
			typedef typename G::edge_iterator base_type;
			typedef typename G::edge_type reference;
			typedef ptrdiff_t difference_type; // is this correct?
			typedef std::input_iterator_tag iterator_category; // makes sense!?
		public:
			edge_iterator()
			{
			}
			edge_iterator(typename gala::graph<SGARGS>::iterator v,
			              typename gala::graph<SGARGS>::out_vertex_iterator e,
					const ::gala::graph<SGARGS>* g)
				: base(std::make_pair(v,e)), _g(g)
			{
				typename gala::graph<SGARGS>::iterator &f = base.first;
				typename gala::graph<SGARGS>::out_vertex_iterator &s = base.second;

				if(f==_g->end()){
				}else if(*s > G::iter::deref(f)){
					increment();
				}else{untested();
				}
			}

		private:
			const reference dereference() const
			{
				typename G::iterator i = base.first;
				typename G::vertex_type s = G::iter::deref(i);
				typename G::vertex_type t = *base.second;

				return std::make_pair(s,t);
			}
			bool equal(const edge_iterator& other) const
			{
				if(base.first==_g->end() &&
					    other.base.first==_g->end()){
					return true;
				}else {
					return base.first == other.base.first
					    && base.second == other.base.second;
				}
			}
			// fixme: more efficient for ordered edgelist
			void increment()
			{
				typedef gala::graph<SGARGS> G;
				typedef typename G::storage storage;
				typedef typename G::iterator vertex_iterator_type;
				typedef typename G::out_vertex_iterator out_vertex_iterator;

				G& gg = *const_cast<gala::graph<SGARGS>*>(_g); // HACK
				typename G::vertex_container_type& VC=gg._v;
				vertex_iterator_type &f=base.first;
				out_vertex_iterator &s=base.second;
				out_vertex_iterator e = storage::out_edges(*f, VC).end();
				++s;

				if(e==s){
				}

				while(true){
					if(f==_g->end()){
						break;
					}else if(s==storage::out_edges(*f, VC).end()) {
						++f;
						if(f==_g->end()){
							break;
						}
						e = storage::out_edges(*f, VC).end();
						s = storage::out_edges(*f, VC).begin();
					}else if(*s == G::iter::deref(f)){
						unreachable(); // self loop
						assert(false);
					}else if(*s > G::iter::deref(f)){
						++f;
						e = storage::out_edges(*f, VC).end();
						s = storage::out_edges(*f, VC).begin();
//					}else if(*(base.second) <
//							G::iter::deref(base.first)){ untested();
//						++(base.second);
					}else if(s!=storage::out_edges(*f, VC).end()){
						break;
					}else{ untested();
						// next row.
						++f;
						e = storage::out_edges(*f, VC).end();
						s = storage::out_edges(*f, VC).begin();
					}
				}
			}

			void decrement() { untested();
				if(base.second == base.first.begin()) {untested();
					--(base.first);
					incomplete();
					base.second = base.first.begin();
				}
				--(base.second);
			}

		private:
		// typedef typename gala::graph<SGARGS>::edge_iterator value_type;
			typename gala::graph<SGARGS>::edge_iterator base;
					  const gala::graph<SGARGS>* _g;
			friend class iterator_core_access;
		};

		class out_edge_iterator
			: public iterator_facade<
			   out_edge_iterator,
			   std::pair<typename gala::graph<SGARGS>::vertex_type,
			             typename gala::graph<SGARGS>::out_vertex_iterator>,
			   // bidirectional_traversal_tag, // breaks InputIterator (why?)
				std::input_iterator_tag,
			   const typename gala::graph<SGARGS>::edge_type, // <= reference
			   const typename gala::graph<SGARGS>::out_vertex_iterator* // difference_type
			>{ //
		public: // types
			typedef typename std::pair<
			    typename gala::graph<SGARGS>::vertex_type,
			    typename gala::graph<SGARGS>::out_vertex_iterator> value_type;
		   typedef intptr_t difference_type; // why?
			typedef typename gala::graph<SGARGS>::edge_type reference;

		public: // construct
			out_edge_iterator()
			{
			}
			out_edge_iterator(
			    typename boost::graph_traits<gala::graph<SGARGS> >::vertex_descriptor v,
			    typename gala::graph<SGARGS>::out_vertex_iterator w)
			{ itested();
				base.first = v;
				base.second = w;
			}
		private:
			reference dereference() const
			{
				return std::make_pair(base.first, *base.second);
			}
			bool equal(const out_edge_iterator& other) const
			{
				return base.second == other.base.second;
			}
			void increment()
			{ itested();
				++(base.second);
			}
			void decrement()
			{ itested();
				--(base.second);
			}
//			bool operator==(const out_edge_iterator& other) const
//			{ incomplete();
//				return false;
//			}
//			bool operator!=(const out_edge_iterator& other) const
//			{ incomplete();
//				return false;
//			}

		private:
			value_type base;
			friend class iterator_core_access;
		};

	}; // traits<sgVCT>
	// =============================================================================//

	VCTtemplate
	typename graph_traits<gala::graph<SGARGS> >::vertex_descriptor
	    add_vertex(gala::graph<SGARGS>& g)
	{
		return g.new_node();
	}

	VCTtemplate
	inline std::pair<typename graph_traits<::gala::graph<SGARGS> >::adjacency_iterator,
	                 typename graph_traits<::gala::graph<SGARGS> >::adjacency_iterator>
	      adjacent_vertices(typename graph_traits<::gala::graph<SGARGS> >::vertex_descriptor u,
	                        const gala::graph<SGARGS>& g)
	{
		typedef typename graph_traits<::gala::graph<SGARGS> >::adjacency_iterator Iter;
		auto& o = g.out_edges(u);
		return std::make_pair( Iter(o.begin()), Iter(o.end()));
	}

	VCTtemplate
	inline std::pair<typename graph_traits<::gala::graph<SGARGS> >::adjacency_iterator,
	                 typename graph_traits<::gala::graph<SGARGS> >::adjacency_iterator>
	      adjacent_vertices(typename graph_traits<::gala::graph<SGARGS> >::vertex_descriptor u,
	                        gala::graph<SGARGS>& g)
	{
		typedef typename graph_traits<::gala::graph<SGARGS> >::adjacency_iterator Iter;
		return std::make_pair( Iter(g.out_edges(u).begin()),
		                       Iter(g.out_edges(u).end()));
	}

	VCTtemplate
	std::pair<typename graph_traits<::gala::graph<SGARGS> >::edge_descriptor,
	    bool> add_edge(
         typename graph_traits<gala::graph<SGARGS> >::vertex_descriptor u,
         typename graph_traits<gala::graph<SGARGS> >::vertex_descriptor v,
         gala::graph<SGARGS>& g)
	{
		assert(v!=u);

		return g.add_edge(u, v);
//		return std::make_pair(n, /*FIXME*/ true);
	}

	VCTtemplate
	void remove_edge(
    typename graph_traits<gala::graph<SGARGS> >::vertex_descriptor u,
    typename graph_traits<gala::graph<SGARGS> >::vertex_descriptor v,
    gala::graph<SGARGS>& g)
	{ untested();
		g.remove_edge(u, v);
	}

	VCTtemplate
	void clear_vertex(typename graph_traits<::gala::graph<SGARGS>
			>::vertex_descriptor u, ::gala::graph<SGARGS> &g)
	{ itested();
		g.clear_vertex(u);
	}

	VCTtemplate
	void remove_vertex(typename graph_traits< ::gala::graph<SGARGS>
			>::vertex_descriptor u, ::gala::graph<SGARGS> &g)
	{ untested();
		g.remove_vertex(u);
	}

	// doesnt work
//	VCTtemplate
//	void* operator[]( ::gala::graph<SGARGS>&,
//			const typename ::gala::graph<SGARGS>::vertex& v) { // untested();
//		incomplete();
//		return NULL;
//	}

	VCTtemplate
	typename graph_traits<::gala::graph<SGARGS> >::edges_size_type
	num_edges(const ::gala::graph<SGARGS>& g)
	{ // untested();
		return g.num_edges();
	}

	VCTtemplate
   std::pair<typename graph_traits<::gala::graph<SGARGS> >::edge_descriptor, bool>
	   edge(/*const*/ typename graph_traits< ::gala::graph<SGARGS> >::vertex_descriptor u,
	        /*const*/ typename graph_traits< ::gala::graph<SGARGS> >::vertex_descriptor v,
		     const ::gala::graph<SGARGS>& g)
	{
		assert(g.is_valid(v));
		assert(g.is_valid(u));

		bool is_edge;
#if 0
		if(g.degree(u)<g.degree(v)){ itested();
		//	std::swap(u,v); // does it really help?
			auto i = g.out_edges(u).find(v);
			is_edge = (i!=g.out_edges(u).end());
		}else{
			auto i = g.out_edges(v).find(u);
			is_edge = (i!=g.out_edges(v).end());
		}
#else
		auto i = g.out_edges(u).find(v);
		is_edge = (i!=g.out_edges(u).end());
#endif


		assert((!is_edge) || (u!=v));
		
		//i = g.out_edges(u).find(v);
		//assert(is_edge == (i!=g.out_edges(u).end()));

		if(is_edge){
			auto e = typename graph_traits<gala::graph<SGARGS> >::edge_descriptor(u,v);
			return std::make_pair(e, is_edge);
		}else{
			auto e = typename graph_traits<gala::graph<SGARGS> >::edge_descriptor();
			return std::make_pair(e, is_edge);
		}
	}

	VCTtemplate
	typename graph_traits<::gala::graph<SGARGS> >::degree_size_type
	out_degree(typename graph_traits< ::gala::graph<SGARGS> >::vertex_descriptor u,
			const ::gala::graph<SGARGS>&g)
	{ itested();
	   return g.degree(u);
	}

	VCTtemplate
	typename graph_traits<::gala::graph<SGARGS> >::degree_size_type
	degree(typename graph_traits< ::gala::graph<SGARGS> >::vertex_descriptor u,
			const ::gala::graph<SGARGS>&g)
	{ itested();
		return boost::out_degree(u,g);
	}

#if 0
	VCTtemplate
	typename graph_traits< ::gala::graph<SGARGS> >::vertex_descriptor source(
	  typename graph_traits< ::gala::graph<SGARGS> >::edge_descriptor e,
	  const ::gala::graph<SGARGS>&)
	{
		return e.first;
	}

	VCTtemplate
	typename graph_traits<gala::graph<SGARGS> >::vertex_descriptor target(
	  typename graph_traits<gala::graph<SGARGS> >::edge_descriptor e,
	  const ::gala::graph<SGARGS>&)
	{
		return e.second;
	}
#else
	using std::source;
	using std::target;
#endif

	VCTtemplate
	inline std::pair<typename graph_traits<gala::graph<SGARGS> >::edge_iterator,
	                 typename graph_traits<gala::graph<SGARGS> >::edge_iterator>
	             edges(const ::gala::graph<SGARGS>& g)
	{
		typedef gala::graph<SGARGS> G;
		typedef typename graph_traits<G>::edge_iterator Iter;
		typedef typename G::iterator vi;
		typedef typename G::vertex_type vertex_type;
		typedef typename G::out_vertex_iterator ovi;
		gala::graph<SGARGS>* gg = const_cast<G*>(&g); // HACK
		vi u = gg->begin();
		vi e = gg->end();

		while(true){
			vertex_type U=G::iter::deref(u);
			if(!gg->out_edges(U).empty()){
				break;
			}else if(u==e){ untested();
				break;
			}
			++u;
		}
		auto deref=G::iter::deref(u);

		if(u!=e){
		}else{untested();
			// mpty
			ovi E;
			return std::make_pair(Iter(e, E, &g),
		                         Iter(e, E, &g));
		}

		ovi U = gg->out_edges(deref).begin();
		assert((deref<*U) || u==e);

		return std::make_pair(Iter(u, U, &g),
		                      Iter(e, U, &g));
	}

	VCTtemplate
	inline std::pair<typename graph_traits<gala::graph<SGARGS> >::vertex_iterator,
	                 typename graph_traits<gala::graph<SGARGS> >::vertex_iterator>
	             vertices(gala::graph<SGARGS> const& g)
	{ itested();
		typedef typename graph_traits<gala::graph<SGARGS> >::vertex_iterator Iter;
		::gala::graph<SGARGS>* G = const_cast<::gala::graph<SGARGS>*>(&g); // HACK
		return std::make_pair( Iter(G->begin()), Iter(G->end()));
	}

	VCTtemplate
	inline std::pair<typename graph_traits<gala::graph<SGARGS> >::vertex_iterator,
	                 typename graph_traits<gala::graph<SGARGS> >::vertex_iterator >
						vertices(gala::graph<SGARGS>& g)
	{ // untested();
		typedef typename graph_traits<gala::graph<SGARGS> >::vertex_iterator Iter;
		return std::make_pair( Iter(g.begin(),&g), Iter(g.end(),&g) );
	}

	VCTtemplate
	inline void copy_graph(const gala::graph<SGARGS>& g, gala::graph<SGARGS>& h)
	{ untested();
		h = g;
	}

   template<galaPARMS,
	         template<class T, typename... > class ECT2,
	         template<class T, typename... > class VCT2,
	         class VDP2,
	         template<class G> class CFG2>
	inline void copy_graph(const gala::graph<SGARGS>& g, gala::graph<ECT2, VCT2, VDP2, CFG2>& h)
	{ untested();
		h = g;
	}

//	VCTtemplate
//	typename graph_traits<::gala::graph<SGARGS> >::edges_size_type
//	num_edges(const gala::graph<SGARGS>& g)
//	{ untested();
//		return g.num_edges();
//	}

	/// collides with...
	VCTtemplate
	inline typename graph_traits<gala::graph<SGARGS> >::vertices_size_type
	num_vertices(const gala::graph<SGARGS>& g)
	{
		return g.num_vertices();
	}


//	VCTtemplate
//	typename graph_traits<::gala::graph<SGARGS> >::vertices_size_type
//	num_vertices(const gala::graph<SGARGS>& g) { // untested();
//		return g.num_vertices();
//	}
//
	VCTtemplate
	struct vertex_property_type<::gala::graph<SGARGS> >{
		typedef void type;
	};
	VCTtemplate
	struct edge_property_type<::gala::graph<SGARGS> >{
		typedef void type;
	};

	// map stuff
	VCTtemplate
	class simplegraph_graph_index_map
	    : public put_get_helper<
	        typename gala::bits::vertex_selector<ECT, VDP>::vertex_index_type,
	        simplegraph_graph_index_map<SGARGS> >
	{ //
		public:
			typedef readable_property_map_tag category;
			typedef int value_type; // FIXME: vertex_index_type;
			typedef int reference;
			typedef unsigned long key_type; // FIXME: vertex_index_type
			simplegraph_graph_index_map(gala::graph<SGARGS>const & g)
			    : _g(g)
			{
			}
			template <class T>
			long operator[](T x) const
			{
				return _g.position(x);
			}
		private:
			gala::graph<SGARGS> const&_g;
	};

	// map stuff
	class simplegraph_graph_eid_map
		: public put_get_helper<int, simplegraph_graph_eid_map>
	{ //
		public:
			typedef readable_property_map_tag category;
			typedef int value_type;
			typedef int reference;
			typedef std::pair<unsigned long, unsigned long> key_type; // ouch
			simplegraph_graph_eid_map() { incomplete(); }
			template <class T>
			long operator[](T x) const { untested(); return x->id(); }
	};

	// property stuff.
	template <class Tag>
	struct simplegraph_property_map { };

	template <>
	struct simplegraph_property_map<vertex_index_t> {
		VCTtemplate
		struct bind_ {
			typedef simplegraph_graph_index_map<SGARGS> type;
			typedef simplegraph_graph_index_map<SGARGS> const_type;
		};
	};

	template <>
	struct simplegraph_property_map<edge_index_t> {
		VCTtemplate
		struct bind_ {
			typedef simplegraph_graph_eid_map type;
			typedef simplegraph_graph_eid_map const_type;
		};
	};

	template <class Data, class DataRef, class GraphPtr>
	class simplegraph_graph_data_map
	: public put_get_helper<DataRef, simplegraph_graph_data_map<Data,DataRef,GraphPtr> >
	{
		public:
			typedef Data value_type;
			typedef DataRef reference;
			typedef unsigned long key_type; // ouch
			typedef lvalue_property_map_tag category;
			simplegraph_graph_data_map(GraphPtr g) : m_g(g) { incomplete(); }
			template <class NodeOrEdge>
				DataRef operator[](NodeOrEdge x) const { incomplete(); return (*m_g)[x]; }
		protected:
			GraphPtr m_g;
	};

	template <>
	struct simplegraph_property_map<vertex_all_t> {
		VCTtemplate
		struct bind_ {
			typedef typename gala::graph<SGARGS>::vertex_type vtype;
			typedef simplegraph_graph_data_map<vtype, vtype&, gala::graph<SGARGS>*> type;
			typedef simplegraph_graph_data_map<vtype, const vtype&,
					  const gala::graph<SGARGS>*> const_type;
		};
	};
	template <>
	struct simplegraph_property_map<edge_all_t> {
		VCTtemplate
		struct bind_ {
			typedef typename gala::graph<SGARGS>::edge_type etype;
			typedef simplegraph_graph_data_map<etype, etype&, gala::graph<SGARGS>*> type;
			typedef simplegraph_graph_data_map<etype, const etype&,
					  const gala::graph<SGARGS>*> const_type;
		};
	};

  // g++ 'enumeral_type' in template unification not implemented workaround
  // // FIXME: SGARGS?!
	template< template<class T, class=std::less<T>, class=std::allocator<T> > class ECT,
	          template<class T, class=std::allocator<T> > class VCT,
				 class VDP,
	          template<class G> class CFG,
				 class Tag>
	struct property_map<gala::graph<SGARGS>, Tag> {
		typedef typename simplegraph_property_map<Tag>::template bind_<SGARGS> map_gen;
		typedef typename map_gen::type type;
		typedef typename map_gen::const_type const_type;
	};

	template< template<class T, class=std::less<T>, class=std::allocator<T> > class ECT,
	          template<class T, class=std::allocator<T> > class VCT,
				 class VDP,
	          template<class G> class CFG>
	struct property_map<gala::graph<SGARGS>,
	                    typename gala::graph<SGARGS>::vertex_index_type> {
//		typedef typename simplegraph_property_map<Tag>::template bind_<SGARGS> map_gen;
		typedef void* type;
		typedef void* const_type;
	};

	VCTtemplate
	inline typename property_map<gala::graph<SGARGS>, edge_all_t>::type
	get(edge_all_t, gala::graph<SGARGS> & g) { incomplete();
		typedef typename property_map<gala::graph<SGARGS>, edge_all_t>::type
			pmap_type;
		return pmap_type(&g);
	}

	VCTtemplate
	inline typename property_map<gala::graph<SGARGS>, edge_all_t>::const_type
	get(edge_all_t, const gala::graph<SGARGS>& g) { incomplete();
		typedef typename property_map<::gala::graph<SGARGS>, edge_all_t>::const_type
			pmap_type;
		incomplete();
		return pmap_type(&g);
	}

	VCTtemplate
	inline typename property_map<gala::graph<SGARGS>, vertex_all_t>::type
	get(vertex_all_t, gala::graph<SGARGS> & g) { incomplete();
		typedef typename property_map<::gala::graph<SGARGS>, vertex_all_t>::type
			pmap_type;
		return pmap_type(&g);
	}

	VCTtemplate
	inline typename property_map<::gala::graph<SGARGS>, vertex_all_t>::const_type
	get(vertex_all_t, const gala::graph<SGARGS>& g) { incomplete();
		typedef typename property_map<::gala::graph<SGARGS>, vertex_all_t>::const_type
			pmap_type;
		incomplete();
		return pmap_type(&g);
	}

	VCTtemplate
	inline simplegraph_graph_index_map<SGARGS>
	get(vertex_index_t, gala::graph<SGARGS> & g)
	{ incomplete();
		typedef typename property_map< gala::graph<SGARGS>, vertex_all_t>::type
			pmap_type;
		return pmap_type(&g);
	}

	VCTtemplate
	inline simplegraph_graph_index_map<SGARGS>
	get(vertex_index_t, const gala::graph<SGARGS> &g)
	{
//		typedef typename property_map< ::gala::graph<SGARGS>, vertex_all_t>::const_type
//			pmap_type;
//		return pmap_type(&g);
		return simplegraph_graph_index_map<SGARGS>(g);
	}

	VCTtemplate
	typename boost::graph_traits<gala::graph<SGARGS > >::vertices_size_type
	get(boost::vertex_index_t t, const gala::graph<SGARGS>& g,
			typename boost::graph_traits< gala::graph<SGARGS> >::vertex_descriptor v)
	{
		return get(get(t, g), v);
	}

	VCTtemplate
	inline std::pair<typename boost::graph_traits< gala::graph<SGARGS> >::out_edge_iterator,
						  typename boost::graph_traits< gala::graph<SGARGS> >::out_edge_iterator >
						out_edges(typename boost::graph_traits< gala::graph<SGARGS> >::vertex_descriptor v,
								gala::graph<SGARGS> const& g)
	{ itested();
		typedef typename boost::graph_traits<gala::graph<SGARGS> >::out_edge_iterator Iter;
		::gala::graph<SGARGS>* G = const_cast<::gala::graph<SGARGS>*>(&g); // HACK
		return std::make_pair(Iter(v, G->out_edges(v).begin()), Iter(v, G->out_edges(v).end()));
	}



} // namespace boost


// HERE? requires boost... (bug?)
#include <boost/graph/adjacency_list.hpp>
// does not work if vertex_descriptor is not contiguous
// should be obsolete with boost::copy_graph
VCTtemplate template<class H>
void gala::graph<SGARGS>::assign(H const& g)
{ // untested();
//	size_t nv = boost::graph_traits<H>::num_vertices(g);
	typedef gala::graph<SGARGS> G;
	size_t nv = boost::num_vertices(g);
	size_t ne = boost::num_edges(g);

	typedef typename boost::graph_traits<gala::graph<SGARGS > >::vertices_size_type st;
	size_t L = std::numeric_limits<st>::max();
	assert(boost::num_vertices(g) <= L); (void) L;

	if(_v.size()){ untested();
		_v.resize(0);
	}else{
	}
	_v.resize(nv);
	vertex_type map[nv];
	size_t i=0;
	_num_edges = 0;

	// not necessary for vector...
//	vertex_iterator v=begin();
	// for(unsigned i=0; i<nv; ++i){ //  v=begin(); v!=end(); ++v)
	for(iterator v=begin(); v!=end(); ++v) {
		// for(auto& v : vertices())
		map[i] = G::iter::deref(v);
		++i;
	}
#ifndef NDEBUG
 	if(i!=nv){
		std::cerr << "oops2 " << i << " " << nv << "\n";
	}
	assert(i==nv);
#endif

	typename boost::graph_traits<H>::edge_iterator eIt, eEnd;
	for(boost::tie(eIt, eEnd) = boost::edges(g); eIt != eEnd; ++eIt){ // untested();
		if(boost::source(*eIt, g) >= num_vertices()){
			assert(false);
			// unreachable?
		}else if(boost::target(*eIt, g) >= num_vertices()) {
			assert(false);
			// unreachable?
		}else{
// -Wno-uninitialized!?
			add_edge(map[boost::source(*eIt, g)],
						map[boost::target(*eIt, g)]);
		}
		// ++_num_edges;
	}
	if(_num_edges == 2*ne){
		_num_edges = ne; // ??1
	}else if(_num_edges < ne){ untested();
//		possibly assigning directed graph.
//		getting fewer edges
		assert(_num_edges<ne);
	}else{
		assert(_num_edges == ne);
	}
	assert(num_vertices() == nv);


#if 0 // not yet
	for(vertex_& i : vertices()){
		assert(is_valid(&i));
		for(auto& j : vertices()){
			assert(is_edge(&j,&i) == is_edge(&i,&j));
		}
	}
#endif
}


#endif
