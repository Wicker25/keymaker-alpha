/*!
 * Title ---- km/ql/ast.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_QL_AST_HPP__
#define __KM_QL_AST_HPP__

#include <km.hpp>

#include <list>

#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <boost/fusion/include/io.hpp>

namespace km { // Begin main namespace

namespace ql { // Begin query language namespace

namespace ast
{
    namespace x3     = boost::spirit::x3;
    namespace fusion = boost::fusion;

    /*!
     * The types
     */
    typedef std::size_t repository_index_t;
    typedef std::string repository_id_t;

    struct repository_t : x3::variant<repository_index_t, repository_id_t>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    typedef std::size_t entry_index_t;
    typedef std::string entry_id_t;

    struct entry_t : x3::variant<entry_index_t, entry_id_t>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    typedef std::string           property_t;
    typedef std::list<property_t> property_list_t;

    typedef std::string action_t;


    /*!
     * The statements
     */
    struct list_stmt
    {
        // No parameters
    };

    struct add_stmt
    {
        std::string name;
    };

    struct show_stmt
    {
        entry_t entry;
    };

    struct select_stmt
    {
        entry_t entry;
        property_list_t property_list;
    };

    struct update_stmt
    {
        entry_t entry;
        property_list_t property_list;
    };

    struct run_stmt
    {
        action_t action;
        entry_t entry;
    };

    struct copy_stmt
    {
        entry_t entry;
        property_list_t property_list;
    };

    struct commit_stmt
    {
        entry_t entry;
    };

    struct push_stmt
    {
        // No parameters
    };

    struct pull_stmt
    {
        // No parameters
    };

    struct remove_stmt
    {
        entry_t entry;
        property_list_t property_list;
    };

    struct stmt : x3::variant
    <
        list_stmt,
        show_stmt,
        add_stmt,
        select_stmt,
        update_stmt,
        run_stmt,
        copy_stmt,
        commit_stmt,
        push_stmt,
        pull_stmt,
        remove_stmt
    >
    {
        using base_type::base_type;
        using base_type::operator=;
    };


    /*!
     * The query
     */
    struct query
    {
        stmt statement;
    };
}

} // End of query language namespace

} // End of main namespace

#endif /* __KM_QL_AST_HPP__ */