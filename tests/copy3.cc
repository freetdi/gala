
#include <assert.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>
#include "../boost.h"
#include <boost/graph/graph_traits.hpp>
#include <gala/examples/ssg16i.h>

template<class G>
struct dvv_config : public gala::graph_cfg_default<G>
{
	 typedef boost::mpl::true_ is_directed_t;
};

typedef gala::graph<std::vector, std::vector, uint16_t, dvv_config> sg_dvv16;
typedef gala::graph<std::set, std::vector, uint16_t> ssg16;
typedef gala::graph<std::vector, std::vector, uint16_t> sg_dvu;
typedef gala::graph<std::set, std::vector, uint16_t> SSG_16i;


using namespace std;

int main(int , char* [])
{
	using namespace boost;

	sg_dvv16 G(5);

	add_edge(0, 1, G);
	add_edge(1, 0, G);
	add_edge(1, 2, G);
	add_edge(2, 0, G);
	assert(boost::num_vertices(G)==5);
	assert(boost::num_edges(G)==4);

	ssg16 G2(G);

	assert(boost::num_vertices(G2)==5);
	trace1("",boost::num_edges(G2));
	assert(boost::num_edges(G2)==3);

}
