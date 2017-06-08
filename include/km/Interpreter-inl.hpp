/*!
 * Title ---- km/Interpreter-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_INTERPRETER_INL_HPP__
#define __KM_INTERPRETER_INL_HPP__

namespace km { // Begin main namespace

template <typename Iterator, typename Attribute, typename Rule>
void
Interpreter::parseRule(Iterator first, Iterator last, Attribute &attribute, Rule &rule)
{
    namespace x3 = boost::spirit::x3;

    using error_handler_tag  = ql::grammar::error_handler_tag;
    using error_handler_type = ql::grammar::error_handler_type;

    // Build the error handler
    std::stringstream error_output;
    error_handler_type error_handler(first, last, error_output);

    // Build the parser
    auto const parser =
        x3::with<error_handler_tag>(std::ref(error_handler))[rule]
    ;

    try {
        auto result = x3::phrase_parse(first, last, parser, x3::space, attribute);

        if (!result) {
            throw SyntaxException(error_output.str());
        }

    } catch (ql::grammar::parser_exception &e) {
        throw SyntaxException(error_output.str());
    }
}

inline void
Interpreter::printEntryProperty(const PropertyNode &property) const
{
    printEntryProperty(property.getName(), property.getContent());
}

} // End of main namespace

#endif /* __KM_INTERPRETER_INL_HPP__ */