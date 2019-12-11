// Felix Salfelder 2016
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, 51 Franklin Street - Suite 500, Boston, MA 02110-1335, USA.
//
//
// prototype implementation for deg tracking.
// TODO: much simpler prototype.
// TODO: use from treedec.
//
#ifndef DEGS_H
#define DEGS_H

#include "trace.h"
#include <assert.h>
#include <boost/graph/graph_traits.hpp>
#include <stack>

namespace gala{

template<class G>
struct sgdeg_config {
    typedef typename boost::graph_traits<G>::vertex_descriptor vertex_descriptor;
    typedef typename G::EL bag_type;
    static void alloc_init(size_t)
	 {
    }
    static unsigned num_threads(){return 1;}
	 template<class container_t>
    static vertex_descriptor pick(unsigned degree, container_t &c){ untested();
        return *c[degree].begin();
    }
};

}// gala

#endif
