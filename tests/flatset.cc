#include <assert.h>
#include "../boost.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/container/flat_set.hpp>
#include "../trace.h"

using boost::container::flat_set;


template<class G>
struct uvv_config : gala::graph_cfg_default<G> {
//    static constexpr bool is_directed=false;
    static constexpr bool is_directed=false;
    static constexpr bool force_simple=true;
    static constexpr bool force_loopless=true;
};
template<class G>
struct dpvv_config : uvv_config<G> {
    static constexpr bool force_simple=false;
    static constexpr bool force_symmetric=false;
    static constexpr bool is_directed=true;
};
typedef gala::graph<std::vector, std::vector, unsigned, uvv_config> graph_t;
typedef gala::graph<flat_set, std::vector, unsigned, dpvv_config> graph_in_t;


int main()
{
	graph_in_t g_in(4);
	assert(!g_in.is_symmetric());
	assert(g_in.is_simple());

	boost::add_edge(0,1,g_in);
	boost::add_edge(0,1,g_in);
	boost::add_edge(0,1,g_in);

	boost::add_edge(3,2,g_in);
	assert(boost::num_edges(g_in)==2);

	graph_t g(g_in);
	graph_in_t g2(g_in);
	graph_t gm(std::move(g_in));

	assert(g.is_simple());
	assert(!g.is_directed());

	auto E=boost::edges(g);
	for(; E.first!=E.second; ++E.first){
		std::cout << "E" << boost::target(*E.first, g) <<  boost::source(*E.first, g) << "\n";
	}
	assert(boost::num_edges(g)==2);
	assert(boost::num_edges(gm)==2);

}
