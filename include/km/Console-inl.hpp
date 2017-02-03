/*!
 * Title ---- km/Console-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_CONSOLE_INL_HPP__
#define __KM_CONSOLE_INL_HPP__

namespace km { // Begin main namespace

inline Buffer
promptSecret(const Buffer &message)
{
    // Print the prompt message
    std::cerr << message;

    return promptSecret();
}

inline void
Console::setDebug(bool state)
{
    mDebug = false;
}

inline bool
Console::getDebug()
{
    return mDebug;
}

} // End of main namespace

#endif /* __KM_CONSOLE_INL_HPP__ */