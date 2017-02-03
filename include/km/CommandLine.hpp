/*!
 * Title ---- km/CommandLine.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_COMMAND_LINE_HPP__
#define __KM_COMMAND_LINE_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>
#include <km/Console.hpp>
#include <km/Core.hpp>
#include <km/Interpreter.hpp>

#include <iostream>
#include <vector>
#include <tuple>
#include <map>

#include <boost/program_options.hpp>

#include <termcolor/termcolor.hpp>

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The command line interface.
 */
class CommandLine
{

public:

    /*!
     * The maximum number of access attempts.
     */
    const int AccessMaxAttempts = 3;


    /*!
     * Constructor method.
     *
     * @param[in] argc The argument count.
     * @param[in] argv The argument vector.
     */
	CommandLine(int argc, char **argv);

    /*!
     * Destructor method.
     */
	virtual ~CommandLine();

protected:

    /*!
     * The type for command arguments.
     */
    typedef std::vector<std::string> CommandArgs;

    /*!
     * The type for parser methods.
     */
    typedef std::function<void (CommandLine &, const CommandArgs &)> ParserMethod;

    /*!
     * The list of parser methods.
     */
    static std::map<const std::string, ParserMethod> mParserMethods;


    /*!
     * Authenticates the user.
     */
    void authenticate();

    /*!
     * Lists the repositories.
     */
    void listRepositories(const CommandArgs &args);

    /*!
     * Creates a new repository.
     */
    void createRepository(const CommandArgs &args);

    /*!
     * Opens a repository.
     */
    void openRepository(const CommandArgs &args);

    /*!
     * Shares a repository.
     */
    void shareRepository(const CommandArgs &args);

    /*!
     * Commits the changes of a repository.
     */
    void commitRepository(const CommandArgs &args);

    /*!
     * Pushes the changes of a repository.
     */
    void pushRepository(const CommandArgs &args);

    /*!
     * Fetches the changes of a repository.
     */
    void fetchRepository(const CommandArgs &args);

    /*!
     * Prints the logs of a repository.
     */
    void logRepository(const CommandArgs &args);

    /*!
     * Destroys a repository.
     */
    void destroyRepository(const CommandArgs &args);

    /*!
     * Shows the owner data.
     */
    void showOwner(const CommandArgs &args) const;

    /*!
     * Parses the command.
     *
     * @param[in] vm The variables map.
     *
     * @return True if success.
     */
    bool parseCommand(program_options::variables_map &vm);

    /*!
     * Prints a commit.
     *
     * @param[in] commit The commit to print.
     */
    void printCommit(const Commit &commit) const;

    /*!
     * Prints a parameter value.
     *
     * @param[in] name  The parameter name.
     * @param[in] value The parameter value.
     */
    void printParameter(const Buffer &name, const Buffer &value) const;

    /*!
     * Prints an error message and terminate the program execution.
     */
    void throwError(const std::string &message, int exitCode = 1) const;


    /*!
     * The KeyMaker Core.
     */
    Core &mCore = Core::get();

    /*!
     * The interpreter.
     */
    Interpreter mInterpreter;
};

} // End of main namespace

#endif /* __KM_COMMAND_LINE_HPP__ */

// Include inline methods
#include <km/CommandLine-inl.hpp>