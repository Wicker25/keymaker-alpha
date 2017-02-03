/*!
 * Title ---- km/AccessKey.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/AccessKey.hpp>

namespace km { // Begin main namespace

AccessKey
AccessKey::create(const Buffer &recipient, const Buffer &data)
{
    AccessKey accessKey;

    accessKey.mRecipient = recipient;
    accessKey.mData      = data;

    return accessKey;
}

AccessKey
AccessKey::fromConfig(property_tree::ptree &config)
{
    auto recipient = config.get<std::string>("recipient");
    auto data      = config.get<std::string>("data");

    algorithm::erase_all_regex(recipient, regex("\\s"));
    algorithm::erase_all_regex(data,      regex("\\s"));

    AccessKey accessKey;

    accessKey.mRecipient = Buffer::fromBase64(recipient);
    accessKey.mData      = Buffer::fromBase64(data);

    return accessKey;
}

property_tree::ptree
AccessKey::toConfig(const AccessKey &accessKey)
{
    property_tree::ptree config;

    config.put<std::string>("recipient", accessKey.mRecipient.toBase64<std::string>());
    config.put<std::string>("recipient.<xmlattr>.encoding", "base64");

    config.put<std::string>("data", accessKey.mData.toBase64<std::string>());
    config.put<std::string>("data.<xmlattr>.encoding", "base64");

    return config;
}

AccessKey::AccessKey()
{

}

AccessKey::AccessKey(const AccessKey &other)
{
    mData      = other.mData;
    mRecipient = other.mRecipient;
}

AccessKey::~AccessKey()
{

}

} // End of main namespace