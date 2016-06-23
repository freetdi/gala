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
// prototype implementation for deg tracking.
//
#ifndef DEGS_H
#define DEGS_H

#include "trace.h"
#include <assert.h>

namespace gala{

template<class G>
struct sgdeg_config{
//    typedef typename boost::graph_traits<G>::vertex_descriptor vd_type; <= ouch?
// #if __cplusplus < 201103L
//     typedef std::set<vd_type> bag_type;
// #else
//     typedef std::unordered_set<vd_type> bag_type;
// #endif
    // typedef stx::btree_set<vd_type> bag_type;
    typedef typename G::EL bag_type;
    static void alloc_init(size_t)
	 {
    }
    static unsigned num_threads(){return 1;}
};


template<class G, class CFG=sgdeg_config<G> >
class default_DEGS{
    default_DEGS(const default_DEGS&){}

public: // types
    typedef typename G::vertex_type vertex_descriptor;
    typedef typename G::iterator vertex_iterator;
    //typedef typename G::EL bag_type;
    typedef typename CFG::bag_type bag_type;
    typedef typename bag_type::iterator bag_iterator;
    typedef std::vector<bag_type> container_type;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;
    typedef typename G::vertex_index_type degree_t;

public: // construct
    default_DEGS(G& g): _degs(g.num_vertices()), _g(g)
    {
		 CFG::alloc_init(g.num_vertices());
		 for(vertex_iterator vIt=g.begin(); vIt!=g.end(); ++vIt){
			  vertex_descriptor v=G::iter::deref(vIt);
			 _degs[g.degree(v)].insert(v);
        }
    }

public: // queueing
    void unlink(const vertex_descriptor& v, size_t d)
    {
        int n=_degs[d].erase(v);
        (void)n;
        assert(n==1);
    }
    void unlink(const vertex_descriptor& v)
    {
        size_t d=_g.degree(v);
        unlink(v,d);
    }

    void reg(const vertex_descriptor& v)
    {
        size_t d=_g.degree(v);
        reg(v,d);
    }
    void reg(const vertex_descriptor& v, size_t d)
    {
        bool n=_degs[d].insert(v).second;
        assert(n); (void)n;
    }

    void flush() const
    {
    }

public: // picking
    vertex_descriptor pick(unsigned degree)
    {
        return *_degs[degree].begin();
    }
    // pick a minimum degree vertex within degree range [lower, upper]
    std::pair<vertex_descriptor,degree_t> pick_min(unsigned lower=0, unsigned upper=-1) const
    {
        while(_degs[lower].empty()){
            ++lower;
            // min_ntd==num_vert contradicts the outer loop condition
            // (this loop should be safe)
            assert(lower != upper+1);
        }
        vertex_descriptor min_nv=*_degs[lower].begin();
        return std::make_pair(min_nv, lower);
    }

    size_t num_nodes() const{ untested();
        unsigned N=0;
        for(const_iterator i=_degs.begin(); i!=_degs.end(); ++i) { itested();
            N+=i->size();
        }
        return N;
    }

    void check()
    { // sometimes required when debugging fancy callbacks :/
#ifdef EXCESSIVE_DEG_DEBUG
            default_DEGS degs(_g);
            assert(_degs.size()==degs.size());
            assert(size()==boost::num_vertices(_g));
            assert(num_nodes()==degs.num_nodes());

            iterator j=degs._degs.begin();
            unsigned N=0;
            for(iterator i=_degs.begin(); i!=_degs.end();) {
                assert(N<boost::num_vertices(_g));
                unsigned I=i->size();
                unsigned J=j->size(); //actual _g

                if(I>J){
                    std::cerr<<"mismatch " << I << " " << J << "\n";
                    std::cerr<<"extra node " << *i->begin() << " of deg " << N << " in degs\n";
                }else if(I<J){
                    std::cerr<<"mismatch " << I << " " << J << " in " << N << "\n";
                    std::cerr<<"extra node " << *j->begin() << " of deg " << N << " in g\n";
                }
                assert(I==J);
                ++i;
                ++j;
                ++N;
            }
            assert(N==boost::num_vertices(_g));
#endif
    } //void check()

    bag_type const& operator[](size_t x) const
    {
        return _degs[x];
    }
    size_t size() const
    {
        return _degs.size();
    }
private:
    bag_type& operator[](size_t x)
    {
        return _degs[x];
    }

//private: // later.
    container_type _degs;
private:
    G& _g;
}; // default_DEGS

}

#endif
