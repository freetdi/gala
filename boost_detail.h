/* Copyright (C) 2016 Felix Salfelder
 * Author: Felix Salfelder
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *------------------------------------------------------------------
 */

#if 0
namespace std{
 	template<class E>
 	size_t source(E const& e )
	{
		incomplete();
	}
 }
#endif
namespace boost{

namespace detail {
	using boost::source;
	template<class G, galaPARMS, class X=void>
	struct copy_helper{ //
		template<class GG, class BEI=typename graph_traits<G>::edge_iterator >
		class edge_iw : public BEI{ //
//			BEI& _b;
			GG const& _g;

		public:
			edge_iw(BEI& b, const GG& g ) : BEI(b), _g(g) {}
			std::pair<size_t, size_t> operator*()
			{ untested();
				typedef typename graph_traits<G>::edge_descriptor edge_descriptor;
				typedef typename graph_traits<G>::vertex_descriptor vertex_descriptor;
				edge_descriptor e=BEI::operator*();
				vertex_descriptor s=source(e,_g);
				vertex_descriptor t=target(e,_g);
				return std::pair<size_t, size_t>(s,t);
			}
		};

		static void boost_to_gala(const G& g, gala::graph<SGARGS>& h)
		{ untested();
			typedef typename graph_traits<G>::edge_iterator BEI;
			typedef edge_iw<G, BEI> bw_t;
			h.reshape(num_vertices(g), num_edges(g), is_directed(g));
			auto E=edges(g);

			if(is_directed(g)){ untested();
				h.fill_in_edges(bw_t(E.first, g), bw_t(E.second, g), false);
			}
		}
		static void copy_from_gg(const gala::graph<SGARGS>& h, G& g)
		{ untested();
			incomplete();
		}
	};

}

#if 0
	template<class G, galaPARMS>
	struct copy_helper<G, SGARGS, boost::graph_traits<G> > { //
		static void boost_to_gala(const G& g, gala::graph<SGARGS>& h)
		{
			incomplete();
		}
		static void copy_from_gg(const gala::graph<SGARGS>& h, G& g)
		{
			incomplete();
		}
	};
#endif
}
