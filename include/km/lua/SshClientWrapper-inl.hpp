/*!
 * Title ---- km/lua/SshClientWrapper-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_LUA_SSH_CLIENT_INL_HPP__
#define __KM_LUA_SSH_CLIENT_INL_HPP__

namespace km { // Begin main namespace

namespace lua { // Begin Lua namespace

inline int
SshClientWrapper::getReturnCode() const
{
    return mSshClient->getReturnCode();
}

} // End of Lua namespace

} // End of main namespace

#endif /* __KM_LUA_SSH_CLIENT_INL_HPP__ */