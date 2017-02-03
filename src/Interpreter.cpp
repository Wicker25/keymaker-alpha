/*!
 * Title ---- km/Interpreter.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/Interpreter.hpp>

namespace km { // Begin main namespace

Buffer
RepositoryArgumentParser::operator()(const ast::repository_id_t &value) const
{
    // TODO: improve this shit

    try {
        auto id = uuids::string_generator()(value);

        auto repositoryId = Buffer::fromString(
            uuids::to_string(id)
        );

        auto repositoryList = mCore.getRepositoryList();

        if (std::find(repositoryList.begin(), repositoryList.end(), repositoryId) == repositoryList.end()) {
            throw ArgumentException("Repository does not exist");
        }

        return repositoryId;

    } catch (std::runtime_error &e) {
        throw ArgumentException("Malformed repository ID");
    }
}

Buffer
RepositoryArgumentParser::operator()(const ast::repository_index_t &value) const
{
    // TODO: improve this shit

    auto repositoryList = mCore.getRepositoryList();

    try {
        return repositoryList.at(value);

    } catch (std::out_of_range &e) {
        throw ArgumentException("Repository does not exist");
    }
}


Buffer
EntryArgumentParser::operator()(const ast::entry_id_t &value) const
{
    // TODO: improve this shit

    try {
        auto id = uuids::string_generator()(value);

        auto entryId = Buffer::fromString(
            uuids::to_string(id)
        );

        auto entryList = mCore.getEntryList();

        if (std::find(entryList.begin(), entryList.end(), entryId) == entryList.end()) {
            throw ArgumentException("Entry does not exist");
        }

        return entryId;

    } catch (ArgumentException &e) {
        throw e;

    } catch (std::runtime_error &e) {
        throw ArgumentException("Malformed entry ID");
    }
}

Buffer
EntryArgumentParser::operator()(const ast::entry_index_t &value) const
{
    // TODO: improve this shit

    auto entryList = mCore.getEntryList();

    try {
        return entryList.at(value);

    } catch (std::out_of_range &e) {
        throw ArgumentException("Entry does not exist");
    }
}


Interpreter::Interpreter()
{

}

Interpreter::~Interpreter()
{

}

void
Interpreter::openShell()
{
    // TODO: improve this
    linenoiseHistoryLoad("history.txt");

    char *line;

    for ( ;; ) {

        errno = 0;
        line  = linenoise("km~> ");

        // Handle CTRL-C and CTRL-D
        if (line == NULL) {

            if (errno == EAGAIN) {
                continue;
            }

            break;
        }

        // Handle empty lines
        if (line[0] == '\0') {

            linenoiseFree(line);
            continue;
        }

        // Execute the query
        try {
            execQuery(line);

        } catch (SyntaxException &e) {
            printError(e.what());

        } catch (ArgumentException &e) {
            printError(e.what());
        }

        linenoiseHistoryAdd(line);
        linenoiseHistorySave("history.txt");

        linenoiseFree(line);
    }
}

void
Interpreter::execQuery(const std::string &query)
{
    ql::ast::query attribute;
    parseRule(query.begin(), query.end(), attribute, ql::query());

    boost::apply_visitor(*this, attribute.statement);
}

Buffer
Interpreter::parseRepository(const std::string &reference)
{
    ql::ast::repository_t attribute;
    parseRule(reference.begin(), reference.end(), attribute, ql::entry());

    return boost::apply_visitor(mRepositoryParser, attribute);
}

Buffer
Interpreter::parseEntry(const std::string &reference)
{
    ql::ast::entry_t attribute;
    parseRule(reference.begin(), reference.end(), attribute, ql::entry());

    return boost::apply_visitor(mEntryParser, attribute);
}

void
Interpreter::operator()(const ast::list_stmt &stmt)
{
    listEntries();
}

void
Interpreter::operator()(const ast::add_stmt &stmt)
{
    addEntry(stmt.name);
}

void
Interpreter::operator()(const ast::show_stmt &stmt)
{
    auto entryId = boost::apply_visitor(mEntryParser, stmt.entry);
    showEntry(entryId);
}

void
Interpreter::operator()(const ast::select_stmt &stmt)
{
    auto entryId = boost::apply_visitor(mEntryParser, stmt.entry);

    if (!stmt.property_list.empty()) {

        for (auto &name : stmt.property_list) {
            showEntryProperty(entryId, name);
        }

    } else {
        showEntry(entryId);
    }
}

void
Interpreter::operator()(const ast::update_stmt &stmt)
{
    auto entryId = boost::apply_visitor(mEntryParser, stmt.entry);

    for (auto &name : stmt.property_list) {
        setEntryProperty(entryId, name);
    }
}

void
Interpreter::operator()(const ast::run_stmt &stmt)
{
    auto entryId = boost::apply_visitor(mEntryParser, stmt.entry);

    runActionOnEntry(entryId, stmt.action);
}

void
Interpreter::operator()(const ast::copy_stmt &stmt)
{
    // TODO
}

void
Interpreter::operator()(const ast::commit_stmt &stmt)
{
    // TODO
}

void
Interpreter::operator()(const ast::push_stmt &stmt)
{
    // TODO
}

void
Interpreter::operator()(const ast::pull_stmt &stmt)
{
    // TODO
}

void
Interpreter::operator()(const ast::remove_stmt &stmt)
{
    auto entryId = boost::apply_visitor(mEntryParser, stmt.entry);

    for (auto &name : stmt.property_list) {
        removeEntryProperty(entryId, name);
    }
}

void
Interpreter::listEntries() const
{
    std::size_t index = 0;

    mCore.eachRepositoryEntry([this, &index](const Buffer &entryId, EntryNode &entry) {

        try {
            auto property = entry.getProperty("name");
            std::cout << "> " << index << ": " << property.getContent() << std::endl;

        } catch (PropertyNotFoundException &e) {
            std::cout << "> " << index << ": " << entryId << std::endl;
        }

        index++;
    });
}

void
Interpreter::addEntry(const Buffer &name)
{
    if (name.empty()) {
        throw ArgumentException("Name must be a non-empty string");
    }

    auto username = promptString("Username");
    auto password = promptPassword();

    auto entry = EntryNode::create();

    entry
        .setProperty("name",     name    )
        .setProperty("username", username)
        .setProperty("password", password)
    ;

    mCore.setRepositoryEntry(entry);
}

void
Interpreter::showEntry(const Buffer &entryId) const
{
    auto entry = mCore.getRepositoryEntry(entryId);

    entry.eachProperty([this](const Buffer &key, PropertyNode &property) {
        printEntryProperty(property);
    });
}

void
Interpreter::runActionOnEntry(const Buffer &entryId, const Buffer &action)
{
    // TODO: complete this

    auto entry = mCore.getRepositoryEntry(entryId);

    auto userProperty     = entry.getProperty("username");
    auto passwordProperty = entry.getProperty("password");

    ScriptHandler handler;
    handler.perform(userProperty.getContent(), passwordProperty.getContent());
}

void
Interpreter::removeEntry(const Buffer &entryId)
{
    // TODO
}

void
Interpreter::setEntryProperty(const Buffer &entryId, const Buffer &name)
{
    Buffer content;

    if (name == "password") {
        content = promptPassword();
    } else {
        content = promptString(name);
    }

    auto entry = mCore.getRepositoryEntry(entryId);
    entry.setProperty(name, content);

    mCore.setRepositoryEntry(entry);
}

void
Interpreter::showEntryProperty(const Buffer &entryId, const Buffer &name) const
{
    auto entry = mCore.getRepositoryEntry(entryId);

    try {
        auto property = entry.getProperty(name);
        printEntryProperty(property);

    } catch (PropertyNotFoundException &e) {
        throw ArgumentException(e.what());
    }
}

void
Interpreter::removeEntryProperty(const Buffer &entryId, const Buffer &name)
{
    auto entry = mCore.getRepositoryEntry(entryId);
    entry.removeProperty(name);

    mCore.setRepositoryEntry(entry);
}

void
Interpreter::printEntryProperty(const Buffer &name, const Buffer &value) const
{
    auto it  = StandardEntryFields.find(name);
    auto end = StandardEntryFields.end();

    std::cout << "> ";

    if (it != end) {

        std::cout
            << termcolor::green
            << termcolor::bold
            << it->second
            << termcolor::reset
        ;

    } else {
        std::cout
            << termcolor::green
            << name
            << termcolor::reset
        ;
    }

    std::cout
        << " "
        << std::string(16 - name.size(), '.')
        << " "
    ;

    if (name == "password") {

        std::cout
            << termcolor::yellow
            << "<hidden>"
            << termcolor::reset
        ;

    } else {
        std::cout << value;
    }

    std::cout << std::endl;
}

void
Interpreter::printError(const Buffer &message) const
{
    std::cerr
        << termcolor::red
        << "!> "
        << message
        << termcolor::reset
        << std::endl
    ;
}

Buffer
Interpreter::promptString(const Buffer &message) const
{
    std::cout
        << "km~[ "
        << termcolor::green
        << message
        << termcolor::reset
        << " ]~% "
        << std::flush
    ;

    Buffer input;

    // Read the input line
    int c;

    while ((c = std::cin.get()) && c != '\n' && c != '\r') {
        input.push_back((std::uint8_t) c);
    }

    return input;
}

Buffer
Interpreter::promptSecret(const Buffer &message) const
{
    std::cout
        << "km~[ "
        << termcolor::green
        << message
        << termcolor::reset
        << " ]~% "
        << std::flush
    ;

    return km::promptSecret();
}

Buffer
Interpreter::promptPassword() const
{
    Buffer password, confirm;

    int attempts = 0;

    do {
        if (attempts++ > 0) {
            printError("Sorry, passwords do not match");
        }

        password = promptSecret("Password");
        confirm  = promptSecret("Retype password");

    } while (password.empty() || password != confirm);

    return password;
}

} // End of main namespace