/*!
 * Title ---- km/lua/SshClientWrapper.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/lua/SshClientWrapper.hpp>

namespace km { // Begin main namespace

namespace lua { // Begin Lua namespace

SshClientWrapper::SshClientWrapper(const BufferWrapper &hostname, const BufferWrapper &port)
{
    mSshClient = std::make_shared<SshClient>
    (
        hostname.getBuffer(),
        port.getBuffer()
    );
}

SshClientWrapper::~SshClientWrapper()
{

}

void
SshClientWrapper::connect(const BufferWrapper &username, const BufferWrapper &password)
{
    mSshClient->connect
    (
        username.getBuffer(),
        password.getBuffer()
    );
}

std::string
SshClientWrapper::exec(const std::string &command)
{
    auto response = mSshClient->exec(command);
    return response.toString();
}

} // End of Lua namespace

} // End of main namespace