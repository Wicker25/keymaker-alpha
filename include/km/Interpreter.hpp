/*!
 * Title ---- km/Interpreter.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_INTERPRETER_HPP__
#define __KM_INTERPRETER_HPP__

#include <km.hpp>
#include <km/config.hpp>
#include <km/Exception.hpp>
#include <km/Console.hpp>
#include <km/Core.hpp>
#include <km/ScriptHandler.hpp>

#include <km/ql/config.hpp>
#include <km/ql/grammar.hpp>
#include <km/ql/grammar_def.hpp>

#include <sstream>
#include <vector>
#include <algorithm>

#include <cerrno>

#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/home/x3.hpp>

#include <termcolor/termcolor.hpp>

#include <linenoise.h>

using namespace boost;

namespace km { // Begin main namespace

namespace ast = ql::ast;

/*!
 * The repository argument parser.
 */
struct RepositoryArgumentParser : public boost::static_visitor<Buffer>
{
    /*!
     * The argument parsers.
     */
    Buffer operator()(const ast::repository_id_t    &value) const;
    Buffer operator()(const ast::repository_index_t &value) const;

    /*!
     * The KeyMaker Core.
     */
    Core &mCore = Core::get();
};


/*!
 * The entry argument parser.
 */
struct EntryArgumentParser : public boost::static_visitor<Buffer>
{
    /*!
     * The argument parsers.
     */
    Buffer operator()(const ast::entry_id_t    &value) const;
    Buffer operator()(const ast::entry_index_t &value) const;

    /*!
     * The KeyMaker Core.
     */
    Core &mCore = Core::get();
};


/*!
 * The QL interpreter.
 */
class Interpreter
{

public:

    /*!
     * Constructor method.
     */
	Interpreter();

    /*!
     * Destructor method.
     */
	virtual ~Interpreter();

    /*!
     * Opens a shell.
     */
    void openShell();

    /*!
     * Executes a query.
     *
     * @param[in] query The query.
     */
    void execQuery(const std::string &query);

    /*!
     * Parses the repository reference.
     *
     * @param[in] ref The repository reference.
     *
     * @return The repository ID.
     */
    Buffer parseRepository(const std::string &reference);

    /*!
     * Parses the entry reference.
     *
     * @param[in] ref The repository reference.
     *
     * @return The entry ID.
     */
    Buffer parseEntry(const std::string &reference);

    /*!
     * The statement parsers.
     */
    void operator()(const ast::list_stmt   &stmt);
    void operator()(const ast::add_stmt    &stmt);
    void operator()(const ast::show_stmt   &stmt);
    void operator()(const ast::select_stmt &stmt);
    void operator()(const ast::update_stmt &stmt);
    void operator()(const ast::run_stmt    &stmt);
    void operator()(const ast::copy_stmt   &stmt);
    void operator()(const ast::commit_stmt &stmt);
    void operator()(const ast::push_stmt   &stmt);
    void operator()(const ast::pull_stmt   &stmt);
    void operator()(const ast::remove_stmt &stmt);

protected:

    /*!
     * Parses a rule.
     */
    template <typename Iterator, typename Attribute, typename Rule>
    void parseRule(Iterator first, Iterator last, Attribute &attribute, Rule &rule);

    /*!
     * Lists the entries in the repository.
     */
    void listEntries() const;

    /*!
     * Adds an entry into the repository.
     *
     * @param[in] name The entry name.
     */
    void addEntry(const Buffer &name);

    /*!
     * Shows an entry in the repository.
     *
     * @param[in] entryId The entry ID.
     */
    void showEntry(const Buffer &entryId) const;

    /*!
     * Runs an action on an entry in the repository.
     *
     * @param[in] entryId The entry ID.
     * @param[in] action  The action name.
     */
    void runActionOnEntry(const Buffer &entryId, const Buffer &action);

    /*!
     * Removes an entry from the repository.
     *
     * @param[in] entryId The entry ID.
     */
    void removeEntry(const Buffer &entryId);

    /*!
     * Sets the property of an entry in the repository.
     *
     * @param[in] entryId The entry ID.
     * @param[in] name    The property name.
     */
    void setEntryProperty(const Buffer &entryId, const Buffer &name);

    /*!
     * Shows the property of an entry in the repository.
     *
     * @param[in] entryId The entry ID.
     * @param[in] name    The property name.
     */
    void showEntryProperty(const Buffer &entryId, const Buffer &name) const;

    /*!
     * Removes an entry from the repository.
     *
     * @param[in] entryId The entry ID.
     * @param[in] name    The property name.
     */
    void removeEntryProperty(const Buffer &entryId, const Buffer &name);

    /*!
     * Prints an entry property.
     *
     * @param[in] name  The property name.
     * @param[in] value The property value.
     */
    void printEntryProperty(const Buffer &name, const Buffer &value) const;

    /*!
     * Prints an entry property.
     *
     * @param[in] property The property.
     */
    void printEntryProperty(const PropertyNode &property) const;

    /*!
     * Prints an error.
     *
     * @param[in] message The error message.
     */
    void printError(const Buffer &message) const;

    /*!
     * Prompts for a string.
     *
     * @param[in] message The prompt message.
     *
     * @return The input data.
     */
    Buffer promptString(const Buffer &message) const;

    /*!
     * Prompts for a secret.
     *
     * @param[in] message The prompt message.
     *
     * @return The input data.
     */
    Buffer promptSecret(const Buffer &message) const;

    /*!
     * Prompts for a password.
     *
     * @return The input data.
     */
    Buffer promptPassword() const;


    /*!
     * The KeyMaker Core.
     */
    Core &mCore = Core::get();

    /*!
     * The repository argument parser.
     */
    RepositoryArgumentParser mRepositoryParser;

    /*!
     * The entry argument parser.
     */
    EntryArgumentParser mEntryParser;
};


/*!
 * The syntax exception.
 */
class SyntaxException : public Exception
{
    using Exception::Exception;
};

/*!
 * The argument exception.
 */
class ArgumentException : public Exception
{
    using Exception::Exception;
};

} // End of main namespace

#endif /* __KM_INTERPRETER_HPP__ */

// Include inline methods
#include <km/Interpreter-inl.hpp>