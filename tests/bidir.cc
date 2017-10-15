#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/copy.hpp>

typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::undirectedS> Gu;
typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS> Gd;
typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::bidirectionalS> Gb;


int main()
{
	Gb a;

	boost::add_vertex(a);
	boost::add_vertex(a);
	boost::add_vertex(a);

	boost::add_edge(0,1,a);
	boost::add_edge(1,0,a);
	boost::add_edge(1,2,a);

	boost::print_graph(a);
	std::cout << "====\n";
	auto x=boost::adjacent_vertices(2,a);
	for(;x.first!=x.second; ++x.first){
		std::cout << *x.first << "\n";
	}
	size_t i=0;
	auto xx=boost::in_edges(2,a);
	for(;xx.first!=xx.second; ++xx.first){
		std::cout << *xx.first << "\n";
		++i;
	}
	assert(i==1);

	{
		Gd b;
		boost::copy_graph(a,b);
		std::cout << "====\n";
		boost::print_graph(b);
		std::cout << "====\n";
		auto y=boost::adjacent_vertices(2,b);
		i=0;
		for(;y.first!=y.second; ++y.first){
			std::cout << *y.first << "\n";
			++i;
		}
		assert(!i);
	}
	{
		Gu b;
		boost::copy_graph(a,b);
		std::cout << "====\n";
		boost::print_graph(b);
		std::cout << "====\n";
		auto y=boost::adjacent_vertices(2,b);
		i=0;
		for(;y.first!=y.second; ++y.first){
			std::cout << *y.first << "\n";
			++i;
		}
		assert(1==i);
	}

	// not possible
	//auto xxx=boost::in_edges(2,b);

}
