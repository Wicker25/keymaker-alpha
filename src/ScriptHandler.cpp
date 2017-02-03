/*!
 * Title ---- km/ScriptHandler.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/ScriptHandler.hpp>

namespace km { // Begin main namespace

ScriptHandler::ScriptHandler()
{

}

ScriptHandler::~ScriptHandler()
{

}

void
ScriptHandler::perform(const Buffer &a, const Buffer &b)
{
    sol::state lua;

    lua.open_libraries();

    sol::table core = lua.create_named_table("core",
        "popen", popen
    );

    lua["core"]["platform"] = BOOST_PLATFORM;

    core.new_usertype<lua::BufferWrapper>("Buffer",
        "assign",     &lua::BufferWrapper::assign,
        //"push",       &lua::BufferWrapper::push,
        "to_string",  &lua::BufferWrapper::to_string,
        "__tostring", &lua::BufferWrapper::to_string
    );


    sol::table ssh = lua.create_named_table("ssh");

    ssh.new_usertype<lua::SshClientWrapper>("Client",
        sol::constructors<sol::types<lua::BufferWrapper, lua::BufferWrapper>>(),
        "connect",         &lua::SshClientWrapper::connect,
        "exec",            &lua::SshClientWrapper::exec,
        "get_return_code", &lua::SshClientWrapper::getReturnCode
    );

    lua.script_file("/home/master/Development/security/keymaker/scripts/client-builder.lua");

    sol::table shell = lua.create_table_with(
        "hostname", lua::BufferWrapper(Buffer::fromString("keymaker.dev")),
        "port",     lua::BufferWrapper(Buffer::fromString("22")),
        "username", lua::BufferWrapper(a),
        "password", lua::BufferWrapper(b),
        "type",     lua::BufferWrapper(Buffer::fromString("ssh"))
    );

    sol::table entry = lua.create_table_with(
        "hostname", lua::BufferWrapper(Buffer::fromString("127.0.0.1")),
        "port",     lua::BufferWrapper(Buffer::fromString("3306")),
        "username", lua::BufferWrapper(Buffer::fromString("vagrant")),
        "password", lua::BufferWrapper(Buffer::fromString("vagrant")),
        "type",     lua::BufferWrapper(Buffer::fromString("unix-user"))
    );

    sol::table args = lua.create_table_with(
        "shell", shell,
        "entry", entry
    );

    sol::function build_client = lua["core"]["build_client"];
    sol::object client = build_client(shell);

    lua.script_file("/home/master/Development/security/keymaker/scripts/adapter-builder.lua");
    sol::function build_adapter = lua["core"]["build_adapter"];
    build_adapter(client, entry);
}

} // End of main namespace