/*!
 * Title ---- km/PropertyNode.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/PropertyNode.hpp>

namespace km { // Begin main namespace

PropertyNode
PropertyNode::create(const Buffer &name, const Buffer &content)
{
    PropertyNode node;

    node.mName    = name;
    node.mContent = content;

    return node;
}

PropertyNode
PropertyNode::create(const Buffer &name, const Buffer &content, const Buffer &nonce)
{
    PropertyNode node;

    node.mName    = name;
    node.mContent = content;
    node.mNonce   = nonce;

    return node;
}

PropertyNode
PropertyNode::fromConfig(property_tree::ptree &config)
{
    PropertyNode node;

    auto nonce   = config.get<std::string>("nonce");
    auto name    = config.get<std::string>("name");
    auto content = config.get<std::string>("content");

    algorithm::erase_all_regex(nonce,   regex("\\s"));
    algorithm::erase_all_regex(name,    regex("\\s"));
    algorithm::erase_all_regex(content, regex("\\s"));

    node.mNonce   = Buffer::fromBase64(nonce);
    node.mName    = Buffer::fromBase64(name);
    node.mContent = Buffer::fromBase64(content);

    return node;
}

property_tree::ptree
PropertyNode::toConfig(const PropertyNode &node)
{
    property_tree::ptree config;

    config.put<std::string>("nonce", node.mNonce.toBase64<std::string>());
    config.put<std::string>("nonce.<xmlattr>.encoding", "base64");

    config.put<std::string>("name", node.mName.toBase64<std::string>());
    config.put<std::string>("name.<xmlattr>.encoding", "base64");

    config.put<std::string>("content", node.mContent.toBase64<std::string>());
    config.put<std::string>("content.<xmlattr>.encoding", "base64");

    return config;
}

PropertyNode::PropertyNode()
{

}

PropertyNode::~PropertyNode()
{

}

} // End of main namespace