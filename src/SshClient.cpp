/*!
 * Title ---- km/SshClient.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/SshClient.hpp>

namespace km { // Begin main namespace

SshClient::SshClient(const Buffer &hostname, const Buffer &port) : mHostname(hostname), mPort(port)
{

}

SshClient::~SshClient()
{

}

void
SshClient::connect(const Buffer &username, const Buffer &password)
{
    openSocket();
    openSession();

    BEGIN_TASK("shell", "Authenticate user");

    checkAuthMethod(username, "password");

    int error = libssh2_userauth_password_ex
    (
        mSession.get(),
        (const char *) username.data(),
        (unsigned int) username.size(),
        (const char *) password.data(),
        (unsigned int) password.size(),
        NULL
    );

    if (error) {
        throw Exception("SshClient: authentication failed");
    }

    END_TASK();
}

void
SshClient::connect(const Buffer &username, const PrivateKey &privateKey)
{
    openSocket();
    openSession();

    BEGIN_TASK("shell", "Authenticate user");

    checkAuthMethod(username, "publickey");

    auto pkcsKey = privateKey.getPkcsKey();

    int error = libssh2_userauth_publickey_frommemory
    (
        mSession.get(),
        (const char *) username.data(),
        (unsigned int) username.size(),
        0,
        0,
        (const char *) pkcsKey.data(),
        (size_t) pkcsKey.size(),
        NULL
    );

    if (error) {
        throw Exception("SshClient: authentication failed");
    }

    END_TASK();
}

void
SshClient::openSession()
{
    BEGIN_TASK("shell", "Start SSH session");

    LIBSSH2_SESSION *session = libssh2_session_init();

    if (!session) {
        throw Exception("SshClient: session initialization failed");
    }

    mSession = std::shared_ptr<LIBSSH2_SESSION>(session, [this](LIBSSH2_SESSION *) {
        closeSession();
    });

    if (libssh2_session_handshake(mSession.get(), mSocket->native_handle())) {
        throw Exception("SshClient: session handshake failed");
    }

    END_TASK();
}

void
SshClient::checkAuthMethod(const Buffer &username, const std::string &method)
{
    std::string methods = libssh2_userauth_list
    (
        mSession.get(),
        (const char *) username.data(),
        (unsigned int) username.size()
    );

    if (methods.find(method) == std::string::npos) {
        throw Exception("SshClient: unsupported password authentication");
    }
}

void
SshClient::closeSession()
{
    BEGIN_TASK("shell", "Stop SSH session");

    if (libssh2_session_disconnect(mSession.get(), "Shutdown ...")) {
        throw Exception("SshClient: failed to disconnect the session");
    }

    libssh2_session_free(mSession.get());

    END_TASK();
}

void
SshClient::openSocket()
{
    using query = tcp::resolver::query;

    mIoService = std::make_shared<asio::io_service>();

    mSocket = std::shared_ptr<tcp::socket>(new tcp::socket(*mIoService), [this](tcp::socket *) {
        closeSocket();
    });

    system::error_code error;
    tcp::resolver::iterator end, endpoint_iterator;

    BEGIN_TASK("shell", "Resolve " << mHostname);

    tcp::resolver resolver(*mIoService);

    endpoint_iterator = resolver.resolve(
        query(mHostname.toString(), mPort.toString()),
        error
    );

    if (error) {
        throw Exception("SshClient: unable to resolve '" + mHostname.toString() + "'");
    }

    END_TASK();

    while (endpoint_iterator != end)
    {
        tcp::endpoint endpoint = *endpoint_iterator++;

        BEGIN_TASK("shell", "Connect to " << endpoint.address());

        if (!mSocket->connect(endpoint, error)) {
            return;
        }

        throw TaskException("SshClient: unable to connect");

        END_TASK();
    }

    throw Exception("SshClient: failed to connect");
}

void
SshClient::closeSocket()
{
    BEGIN_TASK("shell", "Close connection");

    mSocket->close();

    END_TASK();
}

Buffer
SshClient::readFromChannel(LIBSSH2_CHANNEL *channel)
{
    Buffer response, buffer(1024);

    ssize_t length;

    do {
        length = libssh2_channel_read(channel, (char *) buffer.data(), 1024);
        response.insert(response.end(), buffer.begin(), buffer.begin() + length);

    } while (length != LIBSSH2_ERROR_EAGAIN && length > 0);

    response.push_back('\0');

    return response;
}

} // End of main namespace