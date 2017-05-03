
#include <assert.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>
#include "../boost.h"
#include "../boost_copy.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>

template<class G>
struct dvv_config : public gala::graph_cfg_default<G> {
	static constexpr bool is_directed=true;
};
template<class G>
struct dvvm_config : public gala::graph_cfg_default<G> {
	static constexpr bool force_simple=false;
};

typedef gala::graph<std::set, std::vector, uint16_t> SSG_16i;
typedef gala::graph<std::vector, std::vector, uint16_t> vvg;
typedef gala::graph<std::vector, std::vector, uint16_t, dvvm_config> vvmg;
typedef boost::adjacency_list <boost::setS, boost::vecS, boost::directedS> BALD_t;
typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::undirectedS> balu_t;

using namespace std;

int main(int , char* [])
{
	assert(boost::is_multigraph<balu_t>());
	assert(!boost::is_multigraph<SSG_16i>());
	assert(!boost::is_multigraph<vvg>());
	assert(boost::is_multigraph<vvmg>());

	using namespace boost;

	BALD_t G;

	add_vertex(G);
	add_vertex(G);
	add_vertex(G);
	add_vertex(G);

	add_edge(0, 1, G);
	add_edge(1, 0, G);
	add_edge(1, 2, G);
	add_edge(2, 1, G);

	vvg h;
	boost::copy_graph(G, h);
	assert(boost::num_vertices(G)==boost::num_vertices(h));

	// boost to boost, for reference
	balu_t H;
	std::cout << "boost2boost\n";
	boost::copy_graph(G, H);
	std::cout << boost::num_edges(G) << " " << boost::num_edges(H) << "\n";
	assert(boost::num_edges(G)==boost::num_edges(H));

	add_edge(2, 1, G);
	add_edge(3, 1, G);

	SSG_16i G2;
	assert(!boost::is_multigraph<SSG_16i>());
	assert(!boost::is_directed(G2));
	std::cout << "boost vv to sv\n";
	boost::copy_graph(G, G2);

	assert(boost::num_vertices(G)==boost::num_vertices(G2));
	std::cout << boost::num_edges(G) << " " << boost::num_edges(G2) << "\n";
	assert(3==boost::num_edges(G2));
}
