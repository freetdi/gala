
#include <assert.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>
#include "../boost.h"
#include <boost/graph/graph_traits.hpp>

#include "../cbset.h"

template<class G>
struct directed_config : public gala::graph_cfg_default<G> {
	static constexpr bool is_directed=true;
};

//typedef cbset::BSET_DYNAMIC<4> set_type;

template<class T, class...>
// class myset : public cbset::BSET_DYNAMIC<4>{};
using myset=cbset::BSET_DYNAMIC<4>;

typedef gala::graph<myset, std::vector, uint16_t> sg_cbs;

using namespace std;

int main(int , char* [])
{
	sg_cbs g;
	assert(boost::num_edges(g)==0);
	assert(boost::num_vertices(g)==0);

	boost::add_vertex(g);
	boost::add_vertex(g);
	boost::add_vertex(g);
	boost::add_vertex(g);
	auto x=boost::add_vertex(g);
	assert(x==4);
	assert(boost::num_vertices(g)==5);

	sg_cbs h(5);
	assert(boost::num_vertices(h)==5);

	boost::add_edge(1, 2, h);
	assert(boost::num_edges(h)==1);

}
