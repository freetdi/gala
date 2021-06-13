
#include <assert.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>
#include "../boost.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/container/flat_set.hpp>

template<class T, class...>
using myset=boost::container::flat_set<T>;
static_assert(gala::sfinae::is_set_tpl<myset>::value);

template<class T, class...>
using myset1=boost::container::flat_set<T, std::less<T>, boost::container::new_allocator<T> >;
static_assert(gala::sfinae::is_set_tpl<myset1>::value); // BUG

template<class T, class...>
using myset2=boost::container::flat_set<T, std::less<T> >;
static_assert(gala::sfinae::is_set_tpl<myset2>::value);

template<class T, class...>
using myset3=boost::container::flat_set<T, std::less<T>, std::vector<T> >;
static_assert(gala::sfinae::is_set_tpl<myset3>::value);


typedef gala::graph<myset, std::vector, uint16_t> sg_fbs;
typedef gala::graph<myset2, std::vector, uint16_t> sg_fbs2;
typedef gala::graph<myset3, std::vector, uint16_t> sg_fbs3;


using namespace std;

int main(int , char* [])
{
	sg_fbs g;
	sg_fbs2 g2;
	sg_fbs3 g3;
	assert(g3.is_ordered());

	assert(boost::num_edges(g)==0);
	assert(boost::num_vertices(g)==0);
	assert(g.is_ordered());
	assert(g2.is_ordered());

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
