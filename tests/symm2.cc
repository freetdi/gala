
#include <assert.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>
#include "../boost.h"
#include "../boost_copy.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>


template<class G>
struct uvv_config : gala::graph_cfg_default<G> {
//    static constexpr bool is_directed=false;
    static constexpr bool is_directed=true;
    static constexpr bool force_symmetric=true;
    static constexpr bool force_simple=true;
    static constexpr bool force_loopless=true;
};

template<class G>
struct dpvv_config : uvv_config<G> {
    static constexpr bool force_simple=false;
    static constexpr bool force_symmetric=false;
    static constexpr bool is_directed=true;
};

typedef gala::graph<std::vector, std::vector, uint16_t, dpvv_config> P;
typedef gala::graph<std::vector, std::vector, uint16_t, uvv_config> G;

using namespace std;

int main(int , char* [])
{
	P p(4);
	boost::add_edge(0,1,p);
	boost::add_edge(0,1,p);
	boost::add_edge(2,1,p);
	assert(!p.is_simple());
	assert(!p.is_symmetric());

	auto E=boost::edges(p);
	for(;E.first!=E.second; ++E.first){
		std::cout << boost::source(*E.first, p) << " ";
		std::cout << boost::target(*E.first, p) << "\n";
	}

	std::cout << p.num_edges() << "\n";
	assert(p.num_edges()==3);
	assert(!p.is_symmetric());
	assert(!p.is_simple());

	std::cout << "---\n";
	{
		G gc(p);
		G g(std::move(p));
		auto E=boost::edges(g);
		for(;E.first!=E.second; ++E.first){
			std::cout << boost::source(*E.first, g) << " ";
			std::cout << boost::target(*E.first, g) << "\n";
		}
		assert(g.num_edges()==4);
		assert(g.is_symmetric());
		assert(g.is_simple());
		std::cout << "---\n";
		auto F=boost::edges(gc);
		for(;F.first!=F.second; ++F.first){
			std::cout << boost::source(*F.first, gc) << " ";
			std::cout << boost::target(*F.first, gc) << "\n";
		}
		assert(gc.num_edges()==4);
		assert(gc.is_symmetric());
		assert(gc.is_simple());
	}

}
