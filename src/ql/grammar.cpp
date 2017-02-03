/*!
 * Title ---- km/ql/grammar.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/ql/grammar.hpp>
#include <km/ql/grammar_def.hpp>

namespace km { // Begin main namespace

namespace ql { // Begin query language namespace

namespace grammar
{
    BOOST_SPIRIT_INSTANTIATE(query_type, iterator_type, context_type)
}

} // End of query language namespace

} // End of main namespace