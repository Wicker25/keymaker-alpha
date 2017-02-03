/*!
 * Title ---- km/ql/grammar.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_QL_GRAMMAR_HPP__
#define __KM_QL_GRAMMAR_HPP__

#include <km.hpp>

#include <km/ql/config.hpp>
#include <km/ql/ast.hpp>

#include <boost/spirit/home/x3.hpp>


#define DECLARE_GRAMMAR_RULE(name, attribute) \
    struct name##_class; \
    typedef x3::rule<name##_class, attribute> name##_type;

#define DEFINE_GRAMMAR_RULE(name) \
    struct name##_class : error_handler_base {}; \
    const name##_type name = #name;


namespace km { // Begin main namespace

namespace ql { // Begin query language namespace

namespace grammar
{
    namespace x3 = boost::spirit::x3;

    DECLARE_GRAMMAR_RULE(string,        std::string         )
    DECLARE_GRAMMAR_RULE(repository,    ast::repository_t   )
    DECLARE_GRAMMAR_RULE(entry,         ast::entry_t        )
    DECLARE_GRAMMAR_RULE(property_list, ast::property_list_t)
    DECLARE_GRAMMAR_RULE(list_stmt,     ast::list_stmt      )
    DECLARE_GRAMMAR_RULE(add_stmt,      ast::add_stmt       )
    DECLARE_GRAMMAR_RULE(show_stmt,     ast::show_stmt      )
    DECLARE_GRAMMAR_RULE(select_stmt,   ast::select_stmt    )
    DECLARE_GRAMMAR_RULE(update_stmt,   ast::update_stmt    )
    DECLARE_GRAMMAR_RULE(run_stmt,      ast::run_stmt       )
    DECLARE_GRAMMAR_RULE(copy_stmt,     ast::copy_stmt      )
    DECLARE_GRAMMAR_RULE(commit_stmt,   ast::commit_stmt    )
    DECLARE_GRAMMAR_RULE(push_stmt,     ast::push_stmt      )
    DECLARE_GRAMMAR_RULE(pull_stmt,     ast::pull_stmt      )
    DECLARE_GRAMMAR_RULE(remove_stmt,   ast::remove_stmt    )
    DECLARE_GRAMMAR_RULE(stmt,          ast::stmt           )
    DECLARE_GRAMMAR_RULE(query,         ast::query          )

    BOOST_SPIRIT_DECLARE(repository_type)
    BOOST_SPIRIT_DECLARE(entry_type     )
    BOOST_SPIRIT_DECLARE(query_type     )
}

} // End of query language namespace

} // End of main namespace

#endif /* __KM_QL_GRAMMAR_HPP__ */