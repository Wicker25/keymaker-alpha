/*!
 * Title ---- km/ql/config.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_QL_CONFIG_HPP__
#define __KM_QL_CONFIG_HPP__

#include <km.hpp>

#include <km/ql/error_handler.hpp>

#include <boost/spirit/home/x3.hpp>

namespace km { // Begin main namespace

namespace ql { // Begin query language namespace

namespace grammar
{
    namespace x3 = boost::spirit::x3;

    typedef std::string::const_iterator iterator_type;

    typedef x3::phrase_parse_context<
        x3::ascii::space_type
    >::type phrase_context_type;

    typedef error_handler<iterator_type> error_handler_type;

    typedef const x3::with_context<
        error_handler_tag,
        const std::reference_wrapper<error_handler_type>,
        phrase_context_type
    >::type context_type;
}

} // End of query language namespace

} // End of main namespace

#endif /* __KM_QL_CONFIG_HPP__ */