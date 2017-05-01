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

inline const Buffer &
KeyringNode::getId() const
{
    return mId;
}

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

inline std::size_t
KeyringNode::getNumberOfEntries() const
{
    return mEntries.size();
}

inline KeyringNode &
KeyringNode::save(const filesystem::path &path)
{
    KeyringNode::toPath(path, *this);

    return *this;
}

} // End of main namespace

#endif /* __KM_KEYRING_NODE_INL_HPP__ */
