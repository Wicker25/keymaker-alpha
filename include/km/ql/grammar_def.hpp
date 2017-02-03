/*!
 * Title ---- km/ql/grammar_def.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_QL_GRAMMAR_DEF_HPP__
#define __KM_QL_GRAMMAR_DEF_HPP__

#include <km.hpp>

#include <km/ql/ast.hpp>
#include <km/ql/ast_adapted.hpp>
#include <km/ql/error_handler.hpp>
#include <km/ql/grammar.hpp>

#include <boost/spirit/home/x3.hpp>

namespace km { // Begin main namespace

namespace ql { // Begin query language namespace

namespace grammar
{
    DEFINE_GRAMMAR_RULE(string       )
    DEFINE_GRAMMAR_RULE(repository   )
    DEFINE_GRAMMAR_RULE(entry        )
    DEFINE_GRAMMAR_RULE(property_list)
    DEFINE_GRAMMAR_RULE(list_stmt    )
    DEFINE_GRAMMAR_RULE(add_stmt     )
    DEFINE_GRAMMAR_RULE(show_stmt    )
    DEFINE_GRAMMAR_RULE(select_stmt  )
    DEFINE_GRAMMAR_RULE(update_stmt  )
    DEFINE_GRAMMAR_RULE(run_stmt     )
    DEFINE_GRAMMAR_RULE(copy_stmt    )
    DEFINE_GRAMMAR_RULE(commit_stmt  )
    DEFINE_GRAMMAR_RULE(push_stmt    )
    DEFINE_GRAMMAR_RULE(pull_stmt    )
    DEFINE_GRAMMAR_RULE(remove_stmt  )
    DEFINE_GRAMMAR_RULE(stmt         )
    DEFINE_GRAMMAR_RULE(query        )


    using x3::lit;
    using x3::char_;
    using x3::uint_;
    using x3::lexeme;
    using x3::eps;
    using x3::eoi;
    using x3::expect;

    /*!
     * The arguments
     */
    auto const single_quoted_string =
        lexeme['\'' > *(char_ - '\'') > '\'']
    ;

    auto const double_quoted_string =
        lexeme['"' > *(char_ - '"') > '"']
    ;

    auto const string_def =
        single_quoted_string | double_quoted_string
    ;

    auto const name =
        string
    ;

    auto const uuid =
        string
    ;

    auto const integer_index =
        lexeme[':' >> uint_]
    ;

    auto const repository_def =
        integer_index | uuid
    ;

    auto const entry_def =
        integer_index | uuid
    ;

    auto const property =
        string
    ;

    auto const property_list_def =
        lit("*") | property % ','
    ;

    auto const action =
        string
    ;


    /*!
     * The statements
     */
    auto const list_stmt_def =
        lit("list") > eps
    ;

    auto const add_stmt_def =
        lit("add") > name > eps
    ;

    auto const show_stmt_def =
        lit("show") > entry > eps
    ;

    auto const select_stmt_def =
        lit("select") > property_list > lit("from") > entry > eps
    ;

    auto const update_stmt_def =
        lit("update") > entry > lit("set") > property_list > eps
    ;

    auto const run_stmt_def =
        lit("run") > action > lit("on") > entry > eps
    ;

    auto const copy_stmt_def =
        lit("copy") > property_list > lit("from") > entry > eps
    ;

    auto const commit_stmt_def =
        lit("commit") > entry > eps
    ;

    auto const push_stmt_def =
        lit("push") > eps
    ;

    auto const pull_stmt_def =
        lit("pull") > eps
    ;

    auto const remove_stmt_def =
        lit("remove") > property_list > lit("from") > entry > eps
    ;

    auto const stmt_def =
        add_stmt
        | list_stmt
        | show_stmt
        | select_stmt
        | update_stmt
        | run_stmt
        | copy_stmt
        | commit_stmt
        | push_stmt
        | pull_stmt
        | remove_stmt
    ;

    /*!
     * The query
     */
    auto const query_def =
        expect[stmt] > eoi
    ;


    BOOST_SPIRIT_DEFINE(string       )
    BOOST_SPIRIT_DEFINE(repository   )
    BOOST_SPIRIT_DEFINE(entry        )
    BOOST_SPIRIT_DEFINE(property_list)
    BOOST_SPIRIT_DEFINE(list_stmt    )
    BOOST_SPIRIT_DEFINE(add_stmt     )
    BOOST_SPIRIT_DEFINE(show_stmt    )
    BOOST_SPIRIT_DEFINE(select_stmt  )
    BOOST_SPIRIT_DEFINE(update_stmt  )
    BOOST_SPIRIT_DEFINE(run_stmt     )
    BOOST_SPIRIT_DEFINE(copy_stmt    )
    BOOST_SPIRIT_DEFINE(commit_stmt  )
    BOOST_SPIRIT_DEFINE(push_stmt    )
    BOOST_SPIRIT_DEFINE(pull_stmt    )
    BOOST_SPIRIT_DEFINE(remove_stmt  )
    BOOST_SPIRIT_DEFINE(stmt         )
    BOOST_SPIRIT_DEFINE(query        )
}


inline const grammar::repository_type &
repository()
{
    return grammar::repository;
}

inline const grammar::entry_type &
entry()
{
    return grammar::entry;
}

inline const grammar::query_type &
query()
{
    return grammar::query;
}

} // End of query language namespace

} // End of main namespace

#endif /* __KM_QL_GRAMMAR_DEF_HPP__ */