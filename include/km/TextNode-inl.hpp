/*!
 * Title ---- km/TextNode-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_TEXT_NODE_INL_HPP__
#define __KM_TEXT_NODE_INL_HPP__

namespace km { // Begin main namespace

inline TextNode &
TextNode::setContent(const Buffer &value)
{
    mContent = value;
    return *this;
}

inline const Buffer &
TextNode::getContent() const
{
    return mContent;
}

inline TextNode &
TextNode::setNonce(const Buffer &nonce)
{
    mNonce = nonce;
    return *this;
}

inline const Buffer &
TextNode::getNonce() const
{
    return mNonce;
}

inline Buffer
TextNode::toBuffer() const
{
    return TextNode::toBuffer(*this);
}

} // End of main namespace

#endif /* __KM_TEXT_NODE_INL_HPP__ */