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
 *
 */
#include <iostream>
#include <iomanip>
#include <map>
#include <gala/graph.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>

typedef gala::graph<std::set, std::vector, uint16_t> SSG_16i;

// HACK HACK HACK
#include <gala/td.h>
#ifndef TD_DEFS_NETWORK_FLOW
#define TD_DEFS_NETWORK_FLOW
namespace treedec{

struct Vertex_NF{
    bool visited;
    int predecessor;
};

struct Edge_NF{
    bool path; //true if a path uses the edge
};
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, Vertex_NF, Edge_NF> digraph_t;

}
#endif

#include <gala/digraph.h>
#include <gala/immutable.h>

namespace treedec{
template<>
struct graph_traits<SSG_16i>{ //
	typedef typename treedec_chooser<SSG_16i>::type treedec_type;
	typedef typename outedge_set<SSG_16i>::type outedge_set_type;
	typedef idgwel<SSG_16i> directed_overlay;
	typedef idgwel<SSG_16i> immutable_directed_type;
#if 1
	typedef immvecgraph<SSG_16i> immutable_type;
	typedef immvecgraph<SSG_16i> immutable_undirected_type;
#else
	typedef typename boost::adjacency_matrix<boost::undirectedS> immutable_type;
#endif
};

using ssg16i_traits=graph_traits<SSG_16i>;

std::pair<typename boost::graph_traits<typename ssg16i_traits::directed_overlay>::vertex_descriptor,
          typename boost::graph_traits<typename ssg16i_traits::directed_overlay>::vertex_descriptor>
    make_digraph_with_source_and_sink(SSG_16i const &G, std::vector<bool> const &disabled,
                 unsigned num_dis,
                 typename graph_traits<SSG_16i>::directed_overlay& dg,
                 std::vector<typename boost::graph_traits<SSG_16i>::vertex_descriptor> &idxMap,
                 typename std::set<typename boost::graph_traits<SSG_16i>::vertex_descriptor> const &SRC,
                 typename std::set<typename boost::graph_traits<SSG_16i>::vertex_descriptor> const &SNK)
{ untested();
	dg = std::move(idgwel<SSG_16i>(G, disabled, num_dis, idxMap, SRC, SNK));
	return std::make_pair(dg.source(), dg.sink());
}

} // treedec
