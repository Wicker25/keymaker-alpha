/*!
 * Title ---- km/ScriptHandler.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_SCRIPT_HANDLER_HPP__
#define __KM_SCRIPT_HANDLER_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>

#include <km/lua/BufferWrapper.hpp>
#include <km/lua/SshClientWrapper.hpp>

#include <iostream> // FIXME
#include <sstream> // FIXME
#include <memory>

#define LUA_COMPAT_BITLIB

#include <sol/sol.hpp>

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The Script Handler.
 */
class ScriptHandler
{

public:

    /*!
     * Constructor method.
     */
    ScriptHandler();

    /*!
     * Destructor method.
     */
	virtual ~ScriptHandler();

    /*!
     * TODO
     */
    void perform(const Buffer &a, const Buffer &b);

protected:

};

} // End of main namespace

#endif /* __KM_SCRIPT_HANDLER_HPP__ */

// Include inline methods
#include <km/ScriptHandler-inl.hpp>