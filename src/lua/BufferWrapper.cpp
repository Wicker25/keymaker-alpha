/*!
 * Title ---- km/lua/BufferWrapper.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/lua/BufferWrapper.hpp>

namespace km { // Begin main namespace

namespace lua { // Begin Lua namespace

BufferWrapper::BufferWrapper(const Buffer &data)
{
    mBuffer = std::make_shared<Buffer>(data);
}

BufferWrapper::~BufferWrapper()
{

}

} // End of Lua namespace

} // End of main namespace