
#include <assert.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>
#include "../boost.h"
#include <boost/graph/graph_traits.hpp>

#include "../cbset.h"
#include "../boost_copy.h"
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_concepts.hpp>

#include <boost/graph/graph_utility.hpp>

template<class G>
struct directed_config : public gala::graph_cfg_default<G> {
	static constexpr bool is_directed=true;
};

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> balu_t;
typedef typename boost::graph_traits<balu_t>::vertex_descriptor tc_vertex;
typedef typename boost::property_map<balu_t, boost::vertex_index_t>::const_type VertexIndexMap;

struct permute_edge {
	boost::iterator_property_map < tc_vertex *, VertexIndexMap, tc_vertex, tc_vertex&> g_to_tc_map;
	    const balu_t& g;
			permute_edge(boost::iterator_property_map < tc_vertex *,
					VertexIndexMap, tc_vertex, tc_vertex&> g_to_tc_map, const
					balu_t& g): g_to_tc_map(g_to_tc_map), g(g) {}

	std::pair<tc_vertex, tc_vertex>
		operator()(typename boost::graph_traits<balu_t>::edge_descriptor e) const {
			auto p=std::make_pair(
					boost::get(g_to_tc_map, boost::source(e, g)),
					boost::get(g_to_tc_map, boost::target(e, g)));
			std::cout << "perm " << p.first << ":" << p.second << "\n";
			return p;
		}
};


template<class T, class...>
// class myset : public cbset::BSET_DYNAMIC<4>{};
using myset=cbset::BSET_DYNAMIC<4>;

typedef gala::graph<myset, std::vector, uint16_t> sg_cbs;


using namespace std;

int main(int , char* [])
{
	balu_t g(3);
	balu_t g1(3);
	boost::concepts::ReadablePropertyGraphConcept<balu_t, unsigned, boost::vertex_index_t> Xv;
	// boost::concepts::PropertyGraphConcept<balu_t, unsigned, boost::vertex_index_t>  no!
   // boost::concepts::ReadablePropertyGraphConcept<balu_t, unsigned, boost::edge_index_t> Xee;

	sg_cbs h;
 	boost::concepts::ReadablePropertyGraphConcept<sg_cbs, unsigned, boost::vertex_index_t> Xvg;
 	// boost::concepts::PropertyGraphConcept<sg_cbs, unsigned, boost::vertex_index_t> no!

	auto x=boost::add_vertex(g); (void)x;
	auto y=boost::add_vertex(h); (void)y;


	boost::add_edge(0,1,g);
	typedef balu_t::vertex_descriptor vertex_t;
	std::vector<vertex_t> v={0,2,1,3,4};

	//auto m=boost::make_iterator_property_map(v.begin(), boost::get(boost::vertex_index, g), v[0]);
	//boost::copy_graph(g, g1, boost::orig_to_copy(m));
	auto g_edges=boost::edges(g);


	boost::iterator_property_map<tc_vertex*, VertexIndexMap, tc_vertex, tc_vertex&>
			    g_to_tc_map(&v[0], boost::get(boost::vertex_index, g));
	
	permute_edge perm(g_to_tc_map, g);
	balu_t g2(boost::make_transform_iterator(g_edges.first, perm),
	          boost::make_transform_iterator(g_edges.second, perm),
									            boost::num_vertices(g), boost::num_edges(g));

	boost::print_graph(g);
	std::cout << "--\n";
	boost::print_graph(g2);
	std::cout << " " << boost::num_vertices(g) << " --\n";

	sg_cbs g3(boost::make_transform_iterator(g_edges.first, perm),
	          boost::make_transform_iterator(g_edges.second, perm),
									            boost::num_vertices(g), boost::num_edges(g));

	std::cout << "same again?--\n";
	boost::print_graph(g3);
	std::cout << " " << boost::num_vertices(g3) << " --\n";
	std::cout << " " << boost::num_edges(g3) << " --\n";

	boost::copy_graph(h, g);
	boost::copy_graph(g, h);
}

#if 0
{
	std::vector<unsigned> v;
	auto m=boost::make_iterator_property_map(v.begin(), boost::get(boost::vertex_index, g), v[0]);

	boost::copy_graph(g, h, boost::orig_to_copy(m));
}
#endif
