
#include <assert.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>
#include "../boost.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/container/flat_set.hpp>

template<class T, class...>
using myset=boost::container::flat_set<T>;

typedef gala::graph<myset, std::vector, uint16_t> sg_fbs;

using namespace std;

int main(int , char* [])
{
	sg_fbs g;
	assert(boost::num_edges(g)==0);
	assert(boost::num_vertices(g)==0);
	assert(g.is_ordered());

	boost::add_vertex(g);
	boost::add_vertex(g);
	boost::add_vertex(g);
	boost::add_vertex(g);
	auto x=boost::add_vertex(g);
	assert(x==4);
	assert(boost::num_vertices(g)==5);

	sg_fbs h(5);
	assert(boost::num_vertices(h)==5);

	boost::add_edge(1, 2, h);
	assert(boost::num_edges(h)==1);

	boost::add_edge(1, 4, h);
	boost::add_edge(1, 3, h);
	boost::add_edge(1, 2, h);
	boost::add_edge(1, 0, h);

	auto b=boost::adjacent_vertices(1, h);
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

}
