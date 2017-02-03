/*!
 * Title ---- km/lua/BufferWrapper-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_LUA_BUFFER_WRAPPER_INL_HPP__
#define __KM_LUA_BUFFER_WRAPPER_INL_HPP__

namespace km { // Begin main namespace

namespace lua { // Begin Lua namespace

inline void
BufferWrapper::assign(const std::string &data)
{
    *mBuffer = Buffer::fromString(data);
}

inline std::string
BufferWrapper::to_string(sol::table args)
{
    return mBuffer->toString();
}

inline const Buffer &
BufferWrapper::getBuffer() const
{
    return *mBuffer;
}

} // End of Lua namespace

} // End of main namespace

#endif /* __KM_LUA_BUFFER_WRAPPER_INL_HPP__ */