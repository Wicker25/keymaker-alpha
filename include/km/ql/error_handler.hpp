/*!
 * Title ---- km/ql/error_handler.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_QL_ERROR_HANDLER_HPP__
#define __KM_QL_ERROR_HANDLER_HPP__

#include <km.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

#include <stdexcept>
#include <map>

namespace km { // Begin main namespace

namespace ql { // Begin query language namespace

namespace grammar
{
    /*!
     * The parser exception.
     */
    class parser_exception : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };


    namespace x3 = boost::spirit::x3;

    template <typename Iterator>
    using error_handler = x3::error_handler<Iterator>;

    using error_handler_tag = x3::error_handler_tag;

    struct error_handler_base
    {
        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result
        on_error(Iterator &, const Iterator &last, const Exception &x, const Context &context)
        {
            // TODO: improve this shit

            std::string which = x.which();

            auto iter = mRuleNames.find(which);

            if (iter != mRuleNames.end()) {
                which = iter->second;
            }

            std::string message;

            if (which == "stmt") {
                message = "Unrecognized command here:";
            } else {
                message = "Syntax Error! Expecting " + which + " here:";
            }

            auto &error_handler = x3::get<error_handler_tag>(context).get();
            error_handler(x.where(), message);

            throw parser_exception("");
        }

        std::map<std::string, std::string> mRuleNames =
        {
            { "'''",           "quote"                  },
            { "'\"'",          "quote"                  },
            { "string",        "string"                 },
            { "repository",    "repository ID or index" },
            { "entry",         "entry ID or index"      },
            { "property_list", "list of properties"     },
            { "eoi",           "EOI"                    }
        };
    };
}

} // End of query language namespace

} // End of main namespace

#endif /* __KM_QL_ERROR_HANDLER_HPP__ */