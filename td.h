// Felix Salfelder 2016
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
// Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//
// treedec routines specialized for gala backend.
//
#ifndef GALA_TREEDEC_H
#define GALA_TREEDEC_H

#include <gala/boost.h>

#include <tdlib/degree.hpp>
#include <tdlib/graph.hpp>
#include <tdlib/std.hpp>

#include <boost/graph/iteration_macros.hpp>
//hack
#include <stx/btree_set.h>

#include "sethack.h"
//#include "parallel.h"
#include "degs.h"

#define q_in_cb

namespace noboost{ // here??

template<class CB, class G>
class callback_proxy{ //
public:
	typedef typename boost::graph_traits<G>::vertex_descriptor vertex_descriptor;
public:
	callback_proxy(vertex_descriptor v, CB* cb, G const& g)
	    : _done(false), _v(v), _cb(cb), _g(g)
	{
		assert( g.is_valid(v) );
	}
	void operator()(vertex_descriptor w)
	{
//		BUG:: :w
		if(!_cb){ itested();
		}else if(_v<w){
			(*_cb)(boost::edge(_v,w,_g).first);
#ifdef q_in_cb
			(*_cb)(_v);
			(*_cb)(w);
#endif
			_done=true;
		}else{
		}
	}
	bool done() const
	{
		return _done;
	}
private:
	bool _done;
	vertex_descriptor _v;
	CB* _cb;
	const G& _g;
};

template< template<class T, class... > class ECT, \
          template<class T, class... > class VCT,
          class VDP,
          template<class G, class...> class DEG,
          class CB, typename=void /*required for sfinae*/ >
struct sghelp_hack{ //
static size_t mcah(
		typename boost::graph_traits<gala::graph<SGARGS> >::vertex_descriptor c,
		gala::graph<SGARGS>& g,
		typename outedge_set< gala::graph<SGARGS> >::type& bag,
		CB* cb)
{
	typedef gala::graph<SGARGS> G;
	typedef typename boost::graph_traits<G>::vertex_descriptor vertex_descriptor;
	typedef typename boost::graph_traits<G>::adjacency_iterator Iter;
	typedef typename boost::graph_traits<G>::vertex_descriptor VD;
#ifndef NDEBUG
	noboost::check(g);
#endif
	VD x; (void) x;
	Iter I, J, K, E;
	boost::tie(I, E) = boost::adjacent_vertices(c, g);
	long n=0;

	for(J=boost::adjacent_vertices(c, g).first; J!=E; ++J){
		vertex_descriptor N=*J;
#ifndef q_in_cb
		if(cb){untested();
			(*cb)(N);
		}
#endif

		assert(*J!=c);
		assert(g.is_valid(N));
		unsigned m=g.out_edges(N).erase(c);
		assert(m==1); (void) m;
		n += -g.out_edges(N).size();
		auto II(sethack::make_skip_it(I, E, N));

		callback_proxy<CB, G> cbp(N, cb, g);

		// merge II .. E into out_edges(N)
		zipfwdb4(g.out_edges(N), II, E, &cbp);
		if(cbp.done()){
			assert(cb);
		// 	(*cb)(N); not yet.
		}else{
		}
		n += g.out_edges(N).size();
	}

	assert(!(n%2));
	n /= 2; // edges added.
//	typedef typename outedge_set< gala::graph<SGARGS> >::type bag_type;
	assert(!bag.size());
	bag = std::move(g.out_edges(c));

	// FIXME: this might be completely unnecessary.
	g._num_edges += n;
	g._num_edges -= bag.size();
	g.out_edges(c).clear();
	return n;
}

/*----------------------------------*/
// contract edge. it better exists
// FIXME: pass EDGE, not pair of vertices.
typedef typename boost::graph_traits<gala::graph<SGARGS> >::vertex_descriptor vertex_descriptor;
	static void ce(vertex_descriptor vd, vertex_descriptor into,
	                   gala::graph<SGARGS> &g,
							 bool erase=true,
	                   vertex_callback<typename gala::graph<SGARGS>::vertex_type >* cb=NULL)
	{ itested();
		typedef gala::graph<SGARGS> G;
		(void) erase;
		assert(vd!=into);
		{ /// 	g.contract(vd,into);

			auto x=g.out_edges(vd).find(into);
			if(x!=g.out_edges(vd).end()){ itested();
				// delete edge in between
				g.out_edges(vd).erase(x); // can we avoid it? we will vd.clear() later on.
				--g._num_edges; // hack!!
				x = g.out_edges(into).find(vd);
				assert(x!=g.out_edges(into).end());
				g.out_edges(into).erase(x);
			}else{ untested();
				assert(false); // no edge to contract.
			}

			// merge outnodes.
			auto E = g.out_edges(vd).end();
#define OLD // slightly(!) slower. sometimes.
#ifdef OLD
			auto I = g.out_edges(vd).begin();
			zipfwdb4(g.out_edges(into), I, E);
#endif

			for(typename G::EL::iterator iI=g.out_edges(vd).begin();
			    iI!=E; ++iI){ // untested();
				typename G::vertex_type I = *iI;
				assert(I != into);
				bool inserted_new = g.out_edges(I).insert(into).second;
				if(inserted_new){ itested();
					// HERE: insert the other. with hint?
#ifndef OLD
					inserted_new = g.out_edges(into).insert(I).second;
					assert(inserted_new);
#endif
					// edge has moved.
				}else{itested();
					// there are two, one of which will be gone.
					if(cb){itested();
						(*cb)(I);
					}
					--g._num_edges; // hack!!
				}

				{itested();
				assert(g.out_edges(I).end() != g.out_edges(I).find(vd));
				g.out_edges(I).erase(vd);
				}
			} // for

			// urgs. might be unnecessary
			// case of erase=true (next function)
			g.out_edges(vd).clear();
		}
	}
}; // noboost:sghelp_hack

} //noboost

namespace noboost { //
	template<typename G>
	using vertex_iterator = typename boost::graph_traits<G>::vertex_iterator;
	template<typename G>
	using vertex_descriptor = typename boost::graph_traits<G>::vertex_descriptor;
	template<typename G>
	using adjacency_iterator = typename boost::graph_traits<G>::adjacency_iterator;

	VCTtemplate
	void remove_vertex(typename boost::graph_traits<gala::graph<SGARGS> >::vertex_iterator u,
	                   gala::graph<SGARGS> &g)
	{ // untested();
		g.remove_vertex(u.value());
	}

	VCTtemplate
	bool is_valid(typename boost::graph_traits<gala::graph<SGARGS> >::vertex_descriptor v,
	              gala::graph<SGARGS> const& g)
	{
		return g.is_valid(v);
	}

//	template<class G>
//	bool is_valid(typename boost::graph_traits<G>::vertex_descriptor,
//	              G const&)
//	{ untested();
//		return false;
//	}

	VCTtemplate
	void contract_edge(vertex_descriptor<gala::graph<SGARGS> > vd,
	                   vertex_descriptor<gala::graph<SGARGS> > into,
	                   gala::graph<SGARGS> &g,
							 bool erase=true,
	                   vertex_callback<typename gala::graph<SGARGS>::vertex_type >* cb=NULL)
	{ itested();
//		typedef typename boost::graph_traits<gala::graph<SGARGS> >::vertex_descriptor vertex_descriptor;
		typedef noboost::vertex_callback<typename gala::graph<SGARGS>::vertex_type> CB;
//		typedef gala::graph<SGARGS> G;
		(void) erase;
		assert(vd!=into);
		sghelp_hack<ECT, VCT, VDP, DEG, CB>::ce(vd, into, g, erase, cb);
	}

	// weird wrapper. maybe irrelevant.
	VCTtemplate
	void contract_edge(vertex_iterator<gala::graph<SGARGS> > v,
			vertex_descriptor<gala::graph<SGARGS> > into,
			gala::graph<SGARGS> &g,
			bool erase=true,
			vertex_callback<typename gala::graph<SGARGS>::vertex_type >* cb=NULL)
	{ untested();
		if(erase){ untested();
		}
		contract_edge(*v,into,g, erase, cb);

		//		boost::clear_vertex(*v, g);
		if(erase){ incomplete();
			// broken for vector!
			remove_vertex(v, g);
			// *v = into; does not work. for some reason
		}else{ untested();
			assert(g.degree(*v)==0);
			//			(*v)->n.clear();
		}
	}
} // noboost

namespace noboost { // BUG in graphviz?
	// required for _new algo...?
	VCTtemplate
	static inline void add_edge(typename gala::graph<SGARGS>::vertex v,
			typename gala::graph<SGARGS>::vertex w,
			gala::graph<SGARGS> &G){ untested();
		 G.add_edge(v, w);
	}

	// get a vertex identifier.
	VCTtemplate
	vertex_descriptor<gala::graph<SGARGS> > get_vd(const gala::graph<SGARGS>&,
			const vertex_descriptor<gala::graph<SGARGS> >& v )
	{ itested();
		return v;
	}

} // noboost

namespace treedec{ //
// FIXME: always hijack like this, if
// noboost::outedge_set<gala::graph<SGARGS> >::type is treedec_chooser::bag_type?
	VCTtemplate
	typename noboost::outedge_set<gala::graph<SGARGS> >::type detach_neighborhood(
			  typename boost::graph_traits<gala::graph<SGARGS> >::vertex_descriptor& c,
			  gala::graph<SGARGS> & g,
			  typename noboost::outedge_set<gala::graph<SGARGS> >::type* N)
	{ untested();
		assert(N==nullptr); // for now..
		incomplete(); //	unreachable...
		 typename noboost::outedge_set<gala::graph<SGARGS> >::type bag;
		 typename boost::graph_traits<gala::graph<SGARGS> >::adjacency_iterator nIt1, nIt2, nEnd;
		 // inefficient.
		 for(boost::tie(nIt1, nEnd) = boost::adjacent_vertices(c, g);
					nIt1 != nEnd; nIt1++){ // untested();
			  bag.insert(noboost::get_vd(g, *nIt1));
		 }
		 return bag;
	}

	//VCTtemplate // FIXME
	template< template<class T, class... > class ECT, \
				 template<class T, class... > class VCT,
				 class VDP,
				 template<class G, class...> class DEG>
	size_t make_clique_and_detach(
			typename boost::graph_traits<gala::graph<SGARGS> >::vertex_descriptor c,
			gala::graph<SGARGS>& g,
			typename outedge_set< gala::graph<SGARGS> >::type& bag,
			typename treedec::graph_callback<gala::graph<SGARGS> >* cb=NULL)
	{
		typedef typename treedec::graph_callback<gala::graph<SGARGS> > CB;
		return noboost::sghelp_hack<ECT, VCT, VDP, DEG, CB>::mcah(c, g, bag, cb);
	}

}//treedec
namespace noboost{ //

	// theres no Vertex. use the position instead
	template< template<class T, class... > class ECT, class VDP >
	unsigned get_id(const gala::graph<ECT,std::vector, void*>& g,
			const vertex_descriptor<gala::graph<ECT,std::vector, void*> >& v )
	{ untested();
		unsigned p=g.position(v);
		assert(p<g.num_vertices());
		assert(0); // for now (this is dangerous).
		return p;
	}

	VCTtemplate
	unsigned get_id(const gala::graph<SGARGS>& /*g*/,
			const vertex_descriptor<gala::graph<SGARGS> >& /*v*/ )
	{untested();
		incomplete();
		return 0;
	}

	VCTtemplate
	struct outedge_set< gala::graph<SGARGS> >{ //
		typedef typename gala::graph<SGARGS>::EL type;
	};

	VCTtemplate
	struct treedec_chooser< gala::graph<SGARGS> >{ //
		typedef vertex_descriptor< gala::graph<SGARGS> > value_type;
		typedef typename outedge_set< gala::graph<SGARGS> >::type bag_type;
		typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, bag_type> type;
	};

#if 0
VCTtemplate
struct treedec_traits< treedec_chooser<gala::graph<SGARGS> > >{ //
	typedef vertex_descriptor<gala::graph<SGARGS> > vd_type;
	typedef typename outedge_set< gala::graph<SGARGS> >::type bag_type;
};
#endif

//VCTtemplate
// FIXME: more sgs.
template<>
struct treedec_traits< boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,
	std::set<gala::graph<std::set, std::vector, void*>::vertex_*> > >{ //
	typedef typename gala::graph<std::set, std::vector, void*>::vertex_* vd_type;
 	typedef typename outedge_set< gala::graph<std::set, std::vector, void*> >::type bag_type;
};

#if 0
/// something like that...
template<class T>
struct treedec_traits { //
	typedef typename T::vertex_property_type::value_type;
 	typedef typename outedge_set< G::vertex_property_type >::type bag_type;
};
///
template<class U>
struct treedec_traits< typename boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,
	 std::set< typename gala::graph<std::set, std::vector, U>::vertex_*> > >{ //
	typedef typename gala::graph<std::set, std::vector, U>::vertex_* vd_type;
 	typedef typename outedge_set< gala::graph<std::set, std::vector, U> >::type bag_type;
};
#endif

VCTtemplate
inline typename treedec_traits<gala::graph<SGARGS> >::bag_type&
   bag(gala::graph<SGARGS>& /*t*/,
	const vertex_descriptor<gala::graph<SGARGS> >& /*v*/)
{ untested();
	incomplete();
   // return g[v].bag;
}


using boost::setS;
using boost::vecS;
using boost::undirectedS;
using boost::graph_traits;

typedef  boost::adjacency_list<setS, vecS, undirectedS, \
	 std::set< gala::graph<std::set, std::vector, void*>::vertex_*> > TDT;

//VCTtemplate //not yet.
inline typename treedec_traits<TDT>::bag_type&
   bag(TDT& t,
	const typename graph_traits<TDT>::vertex_descriptor& v)
{ itested();
   return t[v];
}

// does not work
// VCTtemplate
// std::set<typename gala::graph<SGARGS>::vertex_descriptor>&
//    bag( typename treedec_chooser< gala::graph<SGARGS> >::type&,
// 			vertex_descriptor< treedec_chooser<gala::graph<SGARGS> > >& )
// 
// {untested();
// 	incomplete();
// }
//
// TODO:: not here
template<class G>
size_t degree(G const& g)
{untested();
	size_t ret=0;
	auto i = boost::vertices(g).first;
	auto e = i;

	for(boost::tie(i,e) = boost::vertices(g); i!=e; ++i){untested();
		size_t d=boost::degree(*i,g);
		if(ret<d) ret = d;
	}
	return ret;
}

VCTtemplate
size_t degree(gala::graph<SGARGS> const& g)
{untested();
	return g.degree();
}

VCTtemplate
void check(gala::graph<SGARGS> const& g)
{itested();
	typedef gala::graph<SGARGS> G;

	unsigned edges=0;
	auto i = boost::vertices(g).first;
	auto e = i;
	auto I = i;
	auto E = i;

#ifdef MORE_DEBUG
	for(boost::tie(i,e) = boost::vertices(g); i!=e; ++i){untested();
//		assert(noboost::is_valid(iter::deref(i),g));
	}
#endif

	for(boost::tie(i,e) = boost::vertices(g); i!=e; ++i){
	//	assert(noboost::is_valid(*i,g));
		auto j = boost::out_edges(*i,g).first;
		auto f = j;
		for(boost::tie(j,f) = boost::out_edges(*i,g); j!=f; ++j){
			++edges;
		}
	}
	assert(!(edges%2));
	edges/=2;

//	std::cout << "OK\n";


	trace2("check edgecount", edges, boost::num_edges(g));
	if (edges > boost::num_edges(g)){untested();
		assert(false);
	}else if (edges < boost::num_edges(g)){untested();
		assert(false);
	}

//	std::cout << "OK2"<< *i <<"\n";

	using adjacency_iterator = typename boost::graph_traits<G>::adjacency_iterator;
	adjacency_iterator aI, aE;

	for(boost::tie(i,e) = boost::vertices(g); i!=e; ++i){ itested();
		for(boost::tie(aI,aE) = boost::adjacent_vertices(*i,g); aI!=aE; ++aI){ itested();
			assert(*aI!=*i);
		}
		for(boost::tie(I,E) = boost::vertices(g); I!=E; ++I){ itested();
			assert(boost::edge(*I,*i,g).second == boost::edge(*i,*I,g).second);
		}
	}
}

//} // noboost
//namespace misc{ // really?!

	VCTtemplate
	struct deg_chooser<gala::graph<SGARGS> >{ //
		typedef gala::graph<SGARGS> G;
		typedef DEG<G> type;
		typedef typename G::vertex_type vd_type;

#if __cplusplus < 201103L
		typedef std::set<vd_type> bag_type;
#else
		typedef std::unordered_set<vd_type> bag_type;
#endif
	};
}

namespace treedec{ //

	// namespace graph?!

VCTtemplate
inline size_t count_missing_edges(
		const typename boost::graph_traits<gala::graph<SGARGS> >::vertex_descriptor v,
		gala::graph<SGARGS> const &g)
{ itested();

	size_t missing_edges=0;

	auto V=v;
	auto e=g.out_edges(V);
	for(auto nIt1 = e.begin(); nIt1!=e.end(); ++nIt1){
		auto nIt2 = nIt1;
		++nIt2;
		auto x=*nIt1; // hack for svu. something about const.
		missing_edges += count_missing(g.out_edges(x), nIt2, e.end());
	}
	return missing_edges;
}

namespace detail{ //
	// iterate over edges adjacent to both v and s
	// implementation: iterate over outedges(v), skip non-outedges(s).
	// TODO: reuse generic set iterator stuff
	VCTtemplate
	class shared_adj_iter<gala::graph<SGARGS> >
	    : public boost::graph_traits<gala::graph<SGARGS> >::adjacency_iterator { //
	public:
		typedef typename gala::graph<SGARGS> G;
		typedef typename boost::graph_traits<G>::adjacency_iterator adjacency_iterator;
		typedef typename boost::graph_traits<G>::vertex_descriptor vertex_descriptor;
		shared_adj_iter(vertex_descriptor v,
		                vertex_descriptor w, G const& g)
		    : boost::graph_traits<G>::adjacency_iterator(g.out_edges(v).begin()),
		      _ve(g.out_edges(v).end()),
				_s(g.out_edges(w).begin()),
		      _w(g.out_edges(w)), _g(g)
		{ itested();
			adjacency_iterator::operator=(g.out_edges(v).lower_bound(*_s));
			_s = g.out_edges(w).lower_bound(**this);
			skip();
		}
		// inefficient. probably obsolete
		shared_adj_iter(adjacency_iterator v, adjacency_iterator ve,
		                vertex_descriptor w, G const& g)
		    : boost::graph_traits<G>::adjacency_iterator(v),
		      _ve(ve), _s(g.out_edges(w).begin()),
		      _w(g.out_edges(w)), _g(g)
		{
			_s = g.out_edges(w).lower_bound(**this);
			skip();
		}
	public: //ops
		shared_adj_iter& operator++()
		{
			// still inefficient...
			adjacency_iterator::operator++();
			if(unlikely(**this>10+*_s)){
				auto w=&_w; /// HACK
				_s = (const_cast<typename G::EL*>(w))->lower_bound(**this);
			}else{
				++_s;
			}
			skip();
			assert(adjacency_iterator::operator==(_ve)
					|| *_s == **this);
			return *this;
		}
		// inefficient
	private:
		void skip()
		{ itested();
			while(true){
				if(unlikely(adjacency_iterator::operator==(_ve))){
					return;
				}else if(unlikely(_s==_w.end())){itested();
					adjacency_iterator::operator=(_ve);
					return;
				}else if(**this<*_s){itested();
					adjacency_iterator::operator++();
				}else if(*_s<**this){
					++_s;
				}else{
					return;
				}
			}
		}
		adjacency_iterator _ve;
	  	adjacency_iterator _s;
		typename G::EL const& _w;
		G const& _g;
	};

}// detail

} // treedec


#endif
