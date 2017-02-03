/*!
 * Title ---- km/lua/BufferWrapper.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_LUA_BUFFER_WRAPPER_HPP__
#define __KM_LUA_BUFFER_WRAPPER_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>

#include <memory>

#include <sol/sol.hpp>

namespace km { // Begin main namespace

namespace lua { // Begin Lua namespace

/*!
 * The buffer wrapper.
 */
struct BufferWrapper
{
    /*!
     * Constructor method.
     *
     * @param[in] data The buffer.
     */
    BufferWrapper(const Buffer &data);

    /*!
     * Destructor method.
     */
    virtual ~BufferWrapper();

    /*!
     * Assigns the buffer content.
     */
    void assign(const std::string &data);


    /*!
     * Converts the buffer into a string.
     */
    std::string to_string(sol::table args);

    /*!
     * Returns the buffer.
     */
    const Buffer &getBuffer() const;


    /*!
     * The buffer.
     */
    std::shared_ptr<Buffer> mBuffer;
};

} // End of Lua namespace

} // End of main namespace

#endif /* __KM_LUA_BUFFER_WRAPPER_HPP__ */

// Include inline methods
#include <km/lua/BufferWrapper-inl.hpp>