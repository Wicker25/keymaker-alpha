/*!
 * Title ---- km/ql/ast_adapted.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_QL_AST_ADAPTED_HPP__
#define __KM_QL_AST_ADAPTED_HPP__

#include <km.hpp>
#include <km/ql/ast.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::add_stmt,
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::list_stmt
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::show_stmt,
    (km::ql::ast::entry_t, entry)
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::select_stmt,
    (km::ql::ast::property_list_t, property_list)
    (km::ql::ast::entry_t,         entry)
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::update_stmt,
    (km::ql::ast::entry_t,         entry)
    (km::ql::ast::property_list_t, property_list)
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::run_stmt,
    (km::ql::ast::action_t, action)
    (km::ql::ast::entry_t,  entry)
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::copy_stmt,
    (km::ql::ast::property_list_t, property_list)
    (km::ql::ast::entry_t,         entry)
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::commit_stmt,
    (km::ql::ast::entry_t, entry)
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::push_stmt
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::pull_stmt
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::remove_stmt,
    (km::ql::ast::property_list_t, property_list)
    (km::ql::ast::entry_t,         entry)
)

BOOST_FUSION_ADAPT_STRUCT(
    km::ql::ast::query,
    (km::ql::ast::stmt, statement)
)

#endif /* __KM_QL_AST_ADAPTED_HPP__ */