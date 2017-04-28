/*!
 * Title ---- km/KeyringNode-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_KEYRING_NODE_INL_HPP__
#define __KM_KEYRING_NODE_INL_HPP__

namespace km { // Begin main namespace

inline KeyringNode &
KeyringNode::setAccessKeys(const AccessKeyMap &accessKeys)
{
    mAccessKeys = accessKeys;
    return *this;
}

inline const AccessKeyMap &
KeyringNode::getAccessKeys() const
{
    return mAccessKeys;
}

inline KeyringNode &
KeyringNode::save(const filesystem::path &path)
{
    KeyringNode::toPath(path, *this);

    return *this;
}

} // End of main namespace

#endif /* __KM_KEYRING_NODE_INL_HPP__ */
