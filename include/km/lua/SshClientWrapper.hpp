/*!
 * Title ---- km/lua/SshClientWrapper.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_LUA_SSH_CLIENT_HPP__
#define __KM_LUA_SSH_CLIENT_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>
#include <km/SshClient.hpp>

#include <km/lua/BufferWrapper.hpp>

#include <memory>

#include <sol/sol.hpp>

namespace km { // Begin main namespace

namespace lua { // Begin Lua namespace

/*!
 * The SSH client.
 */
struct SshClientWrapper
{
    /*!
     * Constructor method.
     *
     * @param[in] hostname The server hostname.
     * @param[in] port     The server port.
     */
    SshClientWrapper(const BufferWrapper &hostname, const BufferWrapper &port);

    /*!
     * Destructor method.
     */
    virtual ~SshClientWrapper();

    /*!
     * Connects the client to the server.
     *
     * @param[in] username   The username.
     * @param[in] privateKey The private key.
     */
    void connect(const BufferWrapper &username, const BufferWrapper &password);

    /*!
     * Executes a command.
     *
     * @param[in] command The command to execute.
     *
     * @return The response content.
     */
    std::string exec(const std::string &command);

    /*!
     * Returns the command return code.
     */
    int getReturnCode() const;


    /*!
     * The remote shell.
     */
    std::shared_ptr<SshClient> mSshClient;
};

} // End of Lua namespace

} // End of main namespace

#endif /* __KM_LUA_SSH_CLIENT_HPP__ */

// Include inline methods
#include <km/lua/SshClientWrapper-inl.hpp>