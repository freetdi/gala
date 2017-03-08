
#include <assert.h>
#include <boost/functional/hash.hpp> // BUG?!
#include <boost/graph/adjacency_matrix.hpp>
#include "../boost.h"
#include <boost/graph/graph_traits.hpp>

template<class G>
struct dvv_config : public gala::graph_cfg_default<G> {
	static constexpr bool is_directed=true;
};

typedef uint32_t unsignedType;
typedef gala::graph<std::vector, std::vector, unsignedType, dvv_config> sg_dvv;
typedef gala::graph<std::vector, std::vector, unsignedType> sg_uvv;

using namespace std;

int main(int , char* [])
{
	sg_uvv ug;
	boost::add_vertex(ug);
	boost::add_vertex(ug);

	auto V = boost::vertices(ug);
	auto next=V.first; ++next;
	bool added = boost::add_edge(*V.first, *next, ug).second;
	assert(added);
	sg_uvv ug2(ug);

	assert(boost::num_edges(ug)==1);

	assert(!sg_uvv::is_directed_v);
	assert(sg_dvv::is_directed_v);
	sg_dvv dg(std::move(ug));

	assert(boost::num_edges(dg)==2);
	sg_dvv dg2(std::move(dg));
	assert(boost::num_edges(dg2)==2);

	typename sg_uvv::directed_type dg3(std::move(ug2));
	assert(boost::num_edges(dg3)==2);
}
