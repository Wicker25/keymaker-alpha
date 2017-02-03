/*!
 * Title ---- km/TextNode.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/TextNode.hpp>

namespace km { // Begin main namespace

TextNode
TextNode::create(const Buffer &content)
{
    TextNode node;

    node.mContent = content;

    return node;
}

TextNode
TextNode::create(const Buffer &content, const Buffer &nonce)
{
    TextNode node;

    node.mContent = content;
    node.mNonce   = nonce;

    return node;
}

TextNode
TextNode::fromBuffer(const Buffer &data)
{
    TextNode node;

    auto end = data.cend();
    auto it  = std::find(data.cbegin(), end, ':');

    if (it == end) {
        throw Exception("Unable to parse text node");
    }

    node.mContent = Buffer::fromBase64(data.cbegin(), it);
    node.mNonce   = Buffer::fromBase64(it + 1, data.cend());

    return node;
}

Buffer
TextNode::toBuffer(const TextNode &node)
{
    auto content = node.getContent().toBase64<Buffer>();
    auto nonce   = node.getNonce().toBase64<Buffer>();

    Buffer buffer(content.size() + nonce.size() + 1);

    std::copy(content.cbegin(), content.cend(), buffer.begin());
    std::copy(nonce.cbegin(),   nonce.cend(),   buffer.begin() + content.size() + 1);

    buffer[content.size()] = ':';

    return buffer;
}

TextNode::TextNode()
{

}

TextNode::~TextNode()
{

}

} // End of main namespace