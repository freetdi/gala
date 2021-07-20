
#include <gala/cbset.h>
#include <gala/graph.h>
#include <gala/boost.h>

typedef cbset::BSET_DYNAMIC<10, uint16_t, cbset::nohowmany_t, cbset::nooffset_t, cbset::nosize_t> myset;
template<class A, class...>
using myset_=myset;
typedef gala::graph<myset_, std::vector, unsigned> graph_t;

int main(int argc, char** argv) {
	int m = 6;

	if(argc>1){
		m = atoi(argv[1]);
	}

	for(int n=m; n--;){

		graph_t g(n*n);

		std::vector<std::pair<int, int>> edges;
		// making edges of graph representing grid n by n
		for (int x = 0; x < n; x++) {
			for (int y = 0; y < n; y++) {
				if (x < n - 1){
					g.add_edge(x + y * n, x + 1 + y * n);
				}else{
				}
				if (y < n - 1){
					g.add_edge(x + y * n, x + (y + 1) * n);
				}else{
				}
			}
		}

		auto i = boost::edges(g);
		auto k=0;
		for(; i.first!=i.second; ++i.first){
			++k;
			// std::cout << boost::source(*i.first, g) <<":"<< boost::target(*i.first, g) << "\n";
		}
		assert(k == 2 * (n-1) * n);


		if(n>2){
			auto j = boost::adjacent_vertices(4, g);
			int nn = 0;
			for(; j.first!=j.second; ++j.first){
				++nn;
			}
			assert(n==7-nn);
		}else{
		}

		//	Tree t;
		// Graph g(edges.begin(), edges.end(), n * n);
		// treedec::exact_decomposition_cutset(g, t);

		// std::cout << treedec::get_bagsize(t) << "\n";
	}
	return 0;
}
