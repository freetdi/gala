
#include <assert.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>
#include "../boost.h"
#include "../boost_copy.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>

template<class G>
struct multig_config : public gala::graph_cfg_default<G> {
	static constexpr bool is_directed=true;
	static constexpr bool force_simple=false;
};

template<class G>
struct dsvv_config : public gala::graph_cfg_default<G> {
	static constexpr bool is_directed=true;
	static constexpr bool force_symmetric=true;
	static constexpr bool force_simple=true;
};

typedef gala::graph<std::vector, std::vector, uint16_t, multig_config> P;
typedef gala::graph<std::vector, std::vector, uint16_t, dsvv_config> G;

typedef gala::graph<std::set, std::vector, uint16_t, multig_config> Ps;
typedef gala::graph<std::set, std::vector, uint16_t, dsvv_config> Gs;

using namespace std;

int main(int , char* [])
{
	P p(2);
	G g;
	Ps ps(2);
	Gs gs;

	assert(!p.is_simple());
	assert(g.is_simple());

	assert(boost::is_multigraph<P>());
	assert(!boost::is_multigraph<G>());

	boost::add_edge(0,1,p);
	boost::add_edge(0,1,p);
	boost::add_edge(0,1,p);
	boost::add_edge(0,1,ps);
	boost::add_edge(0,1,ps);
	boost::add_edge(0,1,ps);

	G h=std::move(p);
	std::cout << h.num_edges() << "\n";
	Gs hs=std::move(ps);
	std::cout << hs.num_edges() << "\n";

	assert(h.num_edges()==2);
	assert(hs.num_edges()==2);
}
