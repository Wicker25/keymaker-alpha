/*!
 * Title ---- km/SshClient-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_SSH_CLIENT_INL_HPP__
#define __KM_SSH_CLIENT_INL_HPP__

namespace km { // Begin main namespace

template <typename ... T_ARGS>
Buffer
SshClient::exec(const Buffer &command, T_ARGS && ... args)
{
    BEGIN_TASK("shell", "Execute command");

    LIBSSH2_CHANNEL *channel = libssh2_channel_open_session(mSession.get());

    if (!channel) {
        throw Exception("SshClient: failed to open the channel");
    }

    Buffer planCommand = format_buffer(command, std::forward<T_ARGS>(args) ...);
    planCommand.push_back('\0');

    if (libssh2_channel_exec(channel, (const char *) planCommand.data())) {
        throw Exception("SshClient: failed to exec the command");
    }

    mResponse   = readFromChannel(channel);
    mReturnCode = libssh2_channel_get_exit_status(channel);

    libssh2_channel_free(channel);

    END_TASK();

    return mResponse;
}

inline int
SshClient::getReturnCode() const
{
    return mReturnCode;
}

} // End of main namespace

#endif /* __KM_SSH_CLIENT_INL_HPP__ */