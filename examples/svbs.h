/* Copyright (C) 2015-2016 Felix Salfelder
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
 *
 */

template<class G>
struct m_deg_config{
	typedef typename boost::graph_traits<G>::vertex_descriptor vd_type;
	typedef typename G::EL bag_type;
	static void alloc_init(size_t)
	{
	}
	static unsigned num_threads()
	{ untested();
		unsigned n=1;
		if(const char* nt=getenv("TD_NUM_DEGWORKER")){ untested();
			n = atoi(nt);
			trace1("num_threads", n);
		}else{ untested();
		}
		return n;
	}
};

template<class T, class...>
using DEX = misc::DEGS<T, m_deg_config<T> >;

template<class G>
struct svbs_config{
	typedef DEX<G> degs_type;
};

typedef gala::graph< stx::btree_set, std::vector, void*, svbs_config> simplegraph_vector_bs;
