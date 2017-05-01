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
     * The type for command.
     */
    typedef std::string Command;

    /*!
     * The type for command arguments.
     */
    typedef std::vector<std::string> CommandArgs;

    /*!
     * The type for parser methods.
     */
    typedef std::function<void (CommandLine &, const CommandArgs &)> CommandParserMethod;

    /*!
     * The list of parser methods.
     */
    static std::map<const std::string, CommandParserMethod> mCommandParserMethods;


    /*!
     * Authenticates the user.
     */
    void authenticate();

    /*!
     * Handles the configurations.
     */
    void runConfig(const CommandArgs &args);

    /*!
     * Lists the repositories.
     */
    void runList(const CommandArgs &args);

    /*!
     * Creates a new repository.
     */
    void runCreate(const CommandArgs &args);

    /*!
     * Opens a repository.
     */
    void runOpen(const CommandArgs &args);

    /*!
     * Shares a repository.
     */
    void runShare(const CommandArgs &args);

    /*!
     * Commits the changes of a repository.
     */
    void runCommit(const CommandArgs &args);

    /*!
     * Pushes the changes of a repository.
     */
    void runPush(const CommandArgs &args);

    /*!
     * Fetches the changes of a repository.
     */
    void runFetch(const CommandArgs &args);

    /*!
     * Prints the logs of a repository.
     */
    void runLog(const CommandArgs &args);

    /*!
     * Destroys a repository.
     */
    void runDestroy(const CommandArgs &args);

    /*!
     * Parses the command.
     *
     * @param[in] command The command.
     * @param[in] args    The command arguments.
     *
     * @return True if success.
     */
    bool parseCommand(const Command &command, const CommandArgs &args);

    /*!
     * Parses the command arguments.
     *
     * @param[in] args               The command arguments.
     * @param[in] options            The options.
     * @param[in] positional_options The positional options.
     *
     * @return The variables map.
     */
    program_options::variables_map parseCommandArguments(
        const CommandArgs &args,
        const program_options::options_description &options,
        const program_options::positional_options_description &positional = {}
    );

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