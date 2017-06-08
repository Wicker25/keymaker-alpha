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

std::map<const std::string, CommandLine::CommandParserMethod> CommandLine::mCommandParserMethods =
{
    { "config",  &CommandLine::runConfig  },
    { "list",    &CommandLine::runList    },
    { "create",  &CommandLine::runCreate  },
    { "open",    &CommandLine::runOpen    },
    { "share",   &CommandLine::runShare   },
    { "commit",  &CommandLine::runCommit  },
    { "push",    &CommandLine::runPush    },
    { "fetch",   &CommandLine::runFetch   },
    { "log",     &CommandLine::runLog     },
    { "destroy", &CommandLine::runDestroy }
};


CommandLine::CommandLine(int argc, char **argv)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km [OPTIONS] <COMMAND>");

    // Positional arguments
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("command", po::value<Command>(),     "The command to run"   )
        ("extra",   po::value<CommandArgs>(), "The command arguments")
    ;

    po::positional_options_description positional;
    positional
        .add("command", 1)
        .add("extra",  -1)
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(hidden)
        .add(usage)
    ;

    po::variables_map vm;

    Command     command;
    CommandArgs command_args;

    try {

        po::parsed_options parsed = po::command_line_parser(argc, (const char **) argv)
            .options(options)
            .positional(positional)
            .allow_unregistered()
            .run()
        ;

        po::store(parsed, vm);
        po::notify(vm);

        if (vm.count("command")) {
            command = vm["command"].as<Command>();
        }

        command_args = po::collect_unrecognized(
            parsed.options,
            po::include_positional
        );

        command_args.erase(command_args.begin());

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    if (vm.count("help")) {

        std::cout << usage;
        return;
    }

    if (!parseCommand(command, command_args)) {
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
CommandLine::runConfig(const CommandArgs &args)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km config [OPTIONS]");
    usage.add_options()
        ("init,i", "Initializes the configurations")
        ("list,l", "Lists the configurations"      )
        ("help,h", "Show this help message"        )
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(usage)
    ;

    // Execute the command
    po::variables_map vm = parseCommandArguments(args, options);

    if (vm.count("init")) {

        // TODO
        std::cout << "Initialize\n";

    } else if (vm.count("list")) {

        // List the configurations
        for (auto &config : OwnerConfigs) {
            printParameter(config.second, mCore.getEnvironment<Buffer>(config.first));
        }

    } else if (vm.count("help")) {
        std::cerr << usage;
    }
}

void
CommandLine::runList(const CommandArgs &args)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km list [OPTIONS]");
    usage.add_options()
        ("help,h", "Show this help message")
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(usage)
    ;

    // Execute the command
    po::variables_map vm = parseCommandArguments(args, options);

    if (vm.count("help")) {
        std::cout << usage;

    } else {

        authenticate();

        std::size_t index = 0;

        mCore.eachKeyring([this, &index](const Buffer &id, KeyringNode &keyringNode) {

            auto nameProperty = keyringNode.getProperty("name");

            std::cout
                << "> " << index++ << ") "
                << nameProperty.getContent()
                << " [" << keyringNode.getNumberOfEntries() << " entries]"
                << std::endl;
        });
    }
}

void
CommandLine::runCreate(const CommandArgs &args)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km create [OPTIONS] <NAME>");
    usage.add_options()
        ("help,h", "Show this help message")
    ;

    // Positional arguments
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("name", po::value<std::string>(), "The name of the new repository")
    ;

    po::positional_options_description positional;
    positional
        .add("name", 1)
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(hidden)
        .add(usage)
    ;

    // Execute the command
    po::variables_map vm = parseCommandArguments(args, options, positional);

    if (vm.count("help")) {
        std::cerr << usage;

    } else if (vm.count("name")) {

        auto name = Buffer::fromString(vm["name"].as<std::string>());

        authenticate();

        mCore.createRepository(name);
    }
}

void
CommandLine::runOpen(const CommandArgs &args)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km open [OPTIONS] <REPOSITORY>");
    usage.add_options()
        ("help,h", "Show this help message")
    ;

    // Positional arguments
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("repository_id", po::value<std::string>(), "The repository ID")
    ;

    po::positional_options_description positional;
    positional
        .add("repository_id", 1)
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(hidden)
        .add(usage)
    ;

    // Execute the command
    po::variables_map vm = parseCommandArguments(args, options, positional);

    if (vm.count("help")) {
        std::cout << usage;

    } else if (vm.count("repository_id")) {

        auto repositoryId = mInterpreter.parseRepository(
            vm["repository_id"].as<std::string>()
        );

        authenticate();

        mCore.openRepository(repositoryId);

        mInterpreter.openShell();
    }
}

void
CommandLine::runShare(const CommandArgs &args)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km share [OPTIONS] <REPOSITORY> <USER>");
    usage.add_options()
        ("help,h", "Show this help message")
    ;

    // Positional arguments
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("repository_id", po::value<std::string>(), "The repository ID")
        ("user_id",       po::value<std::string>(), "The user ID"      )
    ;

    po::positional_options_description positional;
    positional
        .add("repository_id", 1)
        .add("user_id",       1)
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(hidden)
        .add(usage)
    ;

    // Execute the command
    po::variables_map vm = parseCommandArguments(args, options, positional);

    if (vm.count("help")) {
        std::cerr << usage;

    } else if (vm.count("repository_id") && vm.count("user_id")) {

        auto repositoryId = mInterpreter.parseRepository(vm["repository_id"].as<std::string>());
        auto userId = Buffer::fromString(vm["user_id"].as<std::string>());

        authenticate();

        mCore
            .openRepository(repositoryId)
            .shareRepository(userId)
        ;
    }
}

void
CommandLine::runCommit(const CommandArgs &args)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km commit [OPTIONS] <REPOSITORY> <COMMIT_MESSAGE>");
    usage.add_options()
        ("help,h", "Show this help message")
    ;

    // Positional arguments
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("repository_id", po::value<std::string>(), "The repository ID" )
        ("message",       po::value<std::string>(), "The commit message")
    ;

    po::positional_options_description positional;
    positional
        .add("repository_id", 1)
        .add("message",       1)
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(hidden)
        .add(usage)
    ;

    // Execute the command
    po::variables_map vm = parseCommandArguments(args, options, positional);

    if (vm.count("help")) {
        std::cerr << usage;

    } else if (vm.count("repository_id") && vm.count("message")) {

        auto repositoryId = mInterpreter.parseRepository(vm["repository_id"].as<std::string>());
        auto message      = Buffer::fromString(vm["message"].as<std::string>());

        authenticate();

        mCore
            .openRepository(repositoryId)
            .commitRepository(message)
        ;
    }
}

void
CommandLine::runPush(const CommandArgs &args)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km push [OPTIONS] <REPOSITORY>");
    usage.add_options()
        ("help,h", "Show this help message")
    ;

    // Positional arguments
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("repository_id", po::value<std::string>(), "The repository ID")
    ;

    po::positional_options_description positional;
    positional
        .add("repository_id", 1)
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(hidden)
        .add(usage)
    ;

    // Execute the command
    po::variables_map vm = parseCommandArguments(args, options, positional);

    if (vm.count("help")) {
        std::cerr << usage;

    } else if (vm.count("repository_id")) {

        auto repositoryId = mInterpreter.parseRepository(vm["repository_id"].as<std::string>());

        authenticate();

        mCore
            .openRepository(repositoryId)
            .pushRepository()
        ;
    }
}

void
CommandLine::runFetch(const CommandArgs &args)
{
    /*
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
    */
}

void
CommandLine::runLog(const CommandArgs &args)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km commit [OPTIONS] <REPOSITORY> [REVISION_RANGE]");
    usage.add_options()
        ("help,h", "Show this help message")
    ;

    // Positional arguments
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("repository_id",  po::value<std::string>(),                    "The repository ID" )
        ("revision_range", po::value<std::string>()->default_value(""), "The revision range")
    ;

    po::positional_options_description positional;
    positional
        .add("repository_id",  1)
        .add("revision_range", 1)
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(hidden)
        .add(usage)
    ;

    // Execute the command
    po::variables_map vm = parseCommandArguments(args, options, positional);

    if (vm.count("help")) {
        std::cerr << usage;

    } else if (vm.count("repository_id")) {

        auto repositoryId  = mInterpreter.parseRepository(vm["repository_id"].as<std::string>());
        auto revisionRange = vm["revision_range"].as<std::string>();

        authenticate();

        mCore
            .openRepository(repositoryId)
            .eachRepositoryLogs(revisionRange, [this](const Buffer &id, const Commit &commit) {
                printCommit(commit);
            })
        ;
    }
}

void
CommandLine::runDestroy(const CommandArgs &args)
{
    namespace po = boost::program_options;

    // Usage description
    po::options_description usage("Usage: km destroy [OPTIONS] <REPOSITORY>");
    usage.add_options()
        ("help,h", "Show this help message")
    ;

    // Positional arguments
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("repository_id", po::value<std::string>(), "The repository ID" )
    ;

    po::positional_options_description positional;
    positional
        .add("repository_id", 1)
    ;

    // All arguments
    po::options_description options("Allowed options");
    options
        .add(hidden)
        .add(usage)
    ;

    // Execute the command
    po::variables_map vm = parseCommandArguments(args, options, positional);

    if (vm.count("help")) {
        std::cerr << usage;

    } else if (vm.count("repository_id")) {

        auto repositoryId  = mInterpreter.parseRepository(vm["repository_id"].as<std::string>());

        authenticate();

        mCore
            .openRepository(repositoryId)
            .destroyRepository()
        ;
    }
}

bool
CommandLine::parseCommand(const Command &command, const CommandArgs &args)
{
    CommandParserMethod commandParserMethod;

    try {

        commandParserMethod = mCommandParserMethods.at(command);

    } catch (std::out_of_range &e) {

        std::cerr << boost::format("Unknown command '%1%'\n\n") % command;
        return false;
    }

    commandParserMethod(*this, args);

    return true;
}

program_options::variables_map
CommandLine::parseCommandArguments
(
    const CommandArgs &args,
    const program_options::options_description &options,
    const program_options::positional_options_description &positional
)
{
    namespace po = boost::program_options;

    po::variables_map vm;

    try {

        auto parsed_options = po::command_line_parser(args)
            .options(options)
            .positional(positional)
            .run()
        ;

        po::store(parsed_options, vm);
        po::notify(vm);

    } catch (po::error &e) {
        std::cerr << e.what() << std::endl;
    }

    return vm;
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