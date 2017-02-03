/*!
 * Title ---- km/PropertyNode-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_PROPERTY_NODE_INL_HPP__
#define __KM_PROPERTY_NODE_INL_HPP__

namespace km { // Begin main namespace

inline PropertyNode &
PropertyNode::setName(const Buffer &name)
{
    mName = name;
    return *this;
}

inline const Buffer &
PropertyNode::getName() const
{
    return mName;
}

inline PropertyNode &
PropertyNode::setContent(const Buffer &value)
{
    mContent = value;
    return *this;
}

inline const Buffer &
PropertyNode::getContent() const
{
    return mContent;
}

inline PropertyNode &
PropertyNode::setNonce(const Buffer &nonce)
{
    mNonce = nonce;
    return *this;
}

inline const Buffer &
PropertyNode::getNonce() const
{
    return mNonce;
}

} // End of main namespace

#endif /* __KM_PROPERTY_NODE_INL_HPP__ */