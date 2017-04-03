
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
struct ovv_config : public gala::graph_cfg_default<G> {
	static constexpr bool force_ordering=true;
};

typedef gala::graph<std::vector, std::vector, uint16_t> vvg;
typedef gala::graph<std::vector, std::vector, uint16_t, ovv_config> vvgo;
typedef gala::graph<std::vector, std::vector, uint16_t, dvv_config> vvgd;

using namespace std;

int main(int , char* [])
{
	assert(boost::is_multigraph<vvg>());

	vvg g(5);
	vvgo h(5);

	assert(h.is_ordered());
	assert(!g.is_ordered());

	boost::add_edge(0, 4, g);
	boost::add_edge(0, 3, g);
	boost::add_edge(0, 2, g);
	boost::add_edge(0, 1, g);

	assert(boost::edge(0, 1, g).second);
	assert(boost::edge(0, 2, g).second);
	assert(boost::edge(0, 3, g).second);
	assert(boost::edge(0, 4, g).second);

#if 0 // incomplete();
	boost::add_edge(0,4,h);
	boost::add_edge(0,3,h);
	boost::add_edge(0,2,h);
	boost::add_edge(0,1,h);
#endif

	{
		h = g;
	}

#if 1
	auto b=boost::adjacent_vertices(0, h);
	for(;;){
		std::cout << *b.first << "\n";
		unsigned x=*b.first;
		++b.first;

		if(b.first==b.second){
			break;
		}else{
			assert(x<*b.first);
		}
	}
#endif

	{
		h = std::move(g);
	}

	b = boost::adjacent_vertices(0, h);
	for(;;){
		std::cout << *b.first << "\n";
		unsigned x=*b.first;
		++b.first;

		if(b.first==b.second){
			break;
		}else{
			assert(x<*b.first);
		}
	}

	assert(boost::edge(0, 2, h).second);

}
