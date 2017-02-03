/*!
 * Title ---- km/CommandLine.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/CommandLine.hpp>

namespace km { // Begin main namespace

std::map<const std::string, CommandLine::ParserMethod> CommandLine::mParserMethods =
{
    { "list",    &CommandLine::listRepositories  },
    { "create",  &CommandLine::createRepository  },
    { "open",    &CommandLine::openRepository    },
    { "share",   &CommandLine::shareRepository   },
    { "commit",  &CommandLine::commitRepository  },
    { "push",    &CommandLine::pushRepository    },
    { "fetch",   &CommandLine::fetchRepository   },
    { "log",     &CommandLine::logRepository     },
    { "destroy", &CommandLine::destroyRepository },
    { "owner",   &CommandLine::showOwner         }
};


CommandLine::CommandLine(int argc, char **argv)
{
    // Optional parameters
    program_options::options_description usage("Usage: km [OPTION...] [COMMAND]");
    usage.add_options()
        ("help,h", "show this help message")
    ;

    // Positional parameters
    program_options::options_description hidden("Hidden options");
    hidden.add_options()
        ("command", program_options::value<CommandArgs>(), "command")
    ;

    program_options::positional_options_description positional;
    positional.add("command", -1);

    // All parameters
    program_options::options_description all("Allowed options");
    all
        .add(usage)
        .add(hidden)
    ;

    program_options::variables_map vm;

    program_options::store
    (
        program_options::command_line_parser(argc, argv)
            .options(all)
            .positional(positional)
            .run()
        ,
        vm
    );

    program_options::notify(vm);


    if (vm.count("help")) {

        std::cout << usage;
        return;
    }


    if (!parseCommand(vm)) {
        std::cerr << usage;
    }
}

CommandLine::~CommandLine()
{

}

void
CommandLine::authenticate()
{
    auto email = mCore.getEnvironment<std::string>("email");

    auto message = boost::str(
        boost::format("[km] password for \"%s\": ") % email
    );

    int attempts = 0;

    while (++attempts <= AccessMaxAttempts) {

        try {

            auto passphrase = promptSecret(message);
            mCore.authenticate(passphrase);

            return;

        } catch (Exception &e) {

            if (attempts < AccessMaxAttempts) {
                std::cerr << "Sorry, try again.\n";
            }
        }
    }

    throwError("km: 3 incorrect password attempts!");
}

void
CommandLine::listRepositories(const CommandArgs &args)
{
    std::size_t index = 0;

    mCore.eachRepository([this, &index](const Buffer &id, Repository &repository) {
        std::cout << index++ << " => " << id << std::endl;
    });
}

void
CommandLine::createRepository(const CommandArgs &args)
{
    // TODO: improve this
    if (args.size() != 2) {
        throwError("Invalid number of arguments");
    }

    auto name = Buffer::fromString(args[1]);

    authenticate();

    mCore.createRepository(name);
}

void
CommandLine::openRepository(const CommandArgs &args)
{
    // TODO: improve this
    if (args.size() != 2) {
        throwError("Invalid number of arguments");
    }

    auto repositoryId = mInterpreter.parseRepository(args[1]);

    authenticate();

    mCore.openRepository(repositoryId);

    mInterpreter.openShell();
}

void
CommandLine::shareRepository(const CommandArgs &args)
{
    // TODO: improve this
    if (args.size() != 3) {
        throwError("Invalid number of arguments");
    }

    auto repositoryId = mInterpreter.parseRepository(args[1]);
    auto userId       = Buffer::fromString(args[2]);

    authenticate();

    mCore
        .openRepository(repositoryId)
        .shareRepository(userId)
    ;
}

void
CommandLine::commitRepository(const CommandArgs &args)
{
    // TODO: improve this
    if (args.size() != 3) {
        throwError("Invalid number of arguments");
    }

    auto repositoryId = mInterpreter.parseRepository(args[1]);
    auto message      = Buffer::fromString(args[2]);

    authenticate();

    mCore
        .openRepository(repositoryId)
        .commitRepository(message)
    ;
}

void
CommandLine::pushRepository(const CommandArgs &args)
{
    // TODO: improve this
    if (args.size() != 2) {
        throwError("Invalid number of arguments");
    }

    auto repositoryId = mInterpreter.parseRepository(args[1]);

    authenticate();

    mCore
        .openRepository(repositoryId)
        .pushRepository()
    ;
}

void
CommandLine::fetchRepository(const CommandArgs &args)
{
    // TODO: improve this
    if (args.size() != 2) {
        throwError("Invalid number of arguments");
    }

    auto repositoryId = mInterpreter.parseRepository(args[1]);

    authenticate();

    mCore
        .openRepository(repositoryId)
        .fetchRepository()
    ;
}

void
CommandLine::logRepository(const CommandArgs &args)
{
    // TODO: improve this
    if (args.size() != 3) {
        throwError("Invalid number of arguments");
    }

    auto repositoryId  = mInterpreter.parseRepository(args[1]);
    auto revisionRange = args[2];

    authenticate();

    mCore
        .openRepository(repositoryId)
        .eachRepositoryLogs(revisionRange, [this](const Buffer &id, const Commit &commit) {
            printCommit(commit);
        })
    ;
}

void
CommandLine::destroyRepository(const CommandArgs &args)
{
    // TODO: improve this
    if (args.size() != 2) {
        throwError("Invalid number of arguments");
    }

    auto repositoryId = mInterpreter.parseRepository(args[1]);

    authenticate();

    mCore
        .openRepository(repositoryId)
        .destroyRepository()
    ;
}

void
CommandLine::showOwner(const CommandArgs &args) const
{
    // TODO: improve this
    if (args.size() == 1) {
        throwError("Invalid number of arguments");
    }

    for (auto &config : OwnerConfigs) {
        printParameter(config.second, mCore.getEnvironment<Buffer>(config.first));
    }
}

bool
CommandLine::parseCommand(program_options::variables_map &vm)
{
    auto args = vm["command"].as<CommandArgs>();

    ParserMethod parserMethod;

    try {
        parserMethod = mParserMethods.at(args[0]);

    } catch (std::out_of_range &e) {

        std::cerr << boost::format("Unknown command '%1%'\n\n") % args[0];
        return false;
    }

    parserMethod(*this, args);

    return true;
}

void
CommandLine::printCommit(const Commit &commit) const
{
    std::cout
        << termcolor::cyan
        << std::put_time(commit.getTime(), "[%F %X]")
        << termcolor::reset
        << " "
        << termcolor::green
        << "[" << commit.getHash(7) << "]"
        << termcolor::reset
        << " "
        << std::setw(40)
        << std::left
        << commit.getSummary()
        << " "
        << termcolor::yellow
        << commit.getAuthorName()
        << " <" << commit.getAuthorEmail() << ">"
        << termcolor::reset
        << std::endl
    ;
}

void
CommandLine::printParameter(const Buffer &name, const Buffer &value) const
{
    std::cout
        << "> "
        << termcolor::green
        << name
        << termcolor::reset
        << " "
        << std::string(16 - name.size(), '.')
        << " "
        << value
        << std::endl
    ;
}

void
CommandLine::throwError(const std::string &message, int exitCode) const
{
    std::cerr << message << std::endl;

    std::exit(exitCode);
}

} // End of main namespace