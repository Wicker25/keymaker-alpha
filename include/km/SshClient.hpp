/*!
 * Title ---- km/SshClient.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_SSH_CLIENT_HPP__
#define __KM_SSH_CLIENT_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Console.hpp>
#include <km/Buffer.hpp>
#include <km/PrivateKey.hpp>

#include <memory>
#include <string>

#include <libssh2.h>

#include <boost/asio.hpp>

using namespace boost;
using namespace boost::asio;

using tcp = boost::asio::ip::tcp;

namespace km { // Begin main namespace

/*!
 * The SSH client.
 */
class SshClient
{

public:

    /*!
     * Constructor method.
     *
     * @param[in] hostname The server hostname.
     * @param[in] port     The server port.
     */
    SshClient(const Buffer &hostname, const Buffer &port);

    /*!
     * Destructor method.
     */
	virtual ~SshClient();

    /*!
     * Connects the client to the server.
     *
     * @param[in] username The username.
     * @param[in] password The password.
     */
    void connect(const Buffer &username, const Buffer &password);

    /*!
     * Connects the client to the server.
     *
     * @param[in] username   The username.
     * @param[in] privateKey The private key.
     */
    void connect(const Buffer &username, const PrivateKey &privateKey);

    /*!
     * Executes a command.
     *
     * @param[in] command The command to execute.
     * @param[in] args    The arguments.
     *
     * @return The response content.
     */
    template <typename ... T_ARGS>
    Buffer exec(const Buffer &command, T_ARGS && ... args);

    /*!
     * Returns the command return code.
     */
    int getReturnCode() const;

    // TODO: add close() method

protected:

    /*!
     * Opens the SSH session.
     */
    void openSession();

    /*!
     * Checks for an authentication method.
     *
     * @param[in] username The username.
     * @param[in] method   The method to look for.
     */
    void checkAuthMethod(const Buffer &username, const std::string &method);

    /*!
     * Reads data from a channel.
     */
    Buffer readFromChannel(LIBSSH2_CHANNEL *channel);

    /*!
     * Closes the SSH session.
     */
    void closeSession();

    /*!
     * Opens the socket.
     */
    void openSocket();

    /*!
     * Closes the socket.
     */
    void closeSocket();


    /*!
     * The hostname.
     */
    Buffer mHostname;

    /*!
     * The host port.
     */
    Buffer mPort;

    /*!
     * The I/O service.
     */
    std::shared_ptr<asio::io_service> mIoService;

    /*!
     * The socket.
     */
    std::shared_ptr<tcp::socket> mSocket;

    /*!
     * The SSH session.
     */
    std::shared_ptr<LIBSSH2_SESSION> mSession;

    /*!
     * The remote response.
     */
    Buffer mResponse;

    /*!
     * The remote return code.
     */
    int mReturnCode;
};

} // End of main namespace

#endif /* __KM_SSH_CLIENT_HPP__ */

// Include inline methods
#include <km/SshClient-inl.hpp>