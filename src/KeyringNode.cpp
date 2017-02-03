/*!
 * Title ---- km/KeyringNode.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/KeyringNode.hpp>

namespace km { // Begin main namespace

const filesystem::path
KeyringNode::ConfigFile = "keymaker.xml";

const filesystem::path
KeyringNode::EntryDir = "entries";


KeyringNode
KeyringNode::create()
{
    KeyringNode entry;

    return entry;
}

KeyringNode
KeyringNode::fromPath(const filesystem::path &path)
{
    auto filename = path.filename();
    auto configPath = path / ConfigFile;

    KeyringNode keyring;

    keyring.mId = Buffer::fromString(filename.string());
    keyring.mPath = path;

    property_tree::ptree config;

    property_tree::read_xml(configPath.string(), config);

    for (auto &node : config.get_child("keymaker.accessKeys")) {

        auto accessKey = AccessKey::fromConfig(node.second);
        keyring.mAccessKeys[accessKey.getRecipient()] = AccessKey::fromConfig(node.second);
    }

    // FIXME: move this into a function
    {
        Buffer id;

        for (filesystem::directory_iterator it(path / EntryDir); it != filesystem::directory_iterator(); ++it) {

            filesystem::path path, filename;
            path = it->path();
            filename = path.filename();

            if (filename.string()[0] != '.' && filesystem::is_regular_file(path)) {

                id = Buffer::fromString(filename.string());

                EntryNode entry = EntryNode::fromFile(path);

                keyring.mEntries.insert_or_assign(id, entry);
            }
        }
    }

    return keyring;
}

void
KeyringNode::toPath(const filesystem::path &path, const KeyringNode &keyring)
{
    auto configPath = path / ConfigFile;

    property_tree::ptree config;

    config.put<std::string>("keymaker.<xmlattr>.version", "1.0");

    for (auto &it : keyring.mAccessKeys) {
        config.add_child("keymaker.accessKeys.entry", AccessKey::toConfig(it.second));
    }

    property_tree::write_xml
    (
        configPath.string(),
        config,
        std::locale(),
        property_tree::xml_writer_make_settings<std::string>(' ', 4)
    );

    keyring.eachEntry([&path](const Buffer &id, const EntryNode &entry) {

        auto entryPath = path / EntryDir / id.toString();
        EntryNode::toFile(entryPath, entry);
    });
}

KeyringNode::KeyringNode()
{

}

KeyringNode::~KeyringNode()
{

}

KeyringNode &
KeyringNode::addAccessKey(const AccessKey &accessKey)
{
    mAccessKeys[accessKey.getRecipient()] = accessKey;

    return *this;
}

const AccessKey &
KeyringNode::getAccessKey(const Buffer &recipient) const
{
    auto it  = mAccessKeys.find(recipient),
         end = mAccessKeys.end();

    if (it == end) {
        throw Exception("Failed getting AccessKey");
    }

    return it->second;
}

KeyringNode &
KeyringNode::setEntry(const EntryNode &entry)
{
    mEntries.insert_or_assign(entry.getId(), entry);

    return *this;
}

const EntryNode &
KeyringNode::getEntry(const Buffer &id) const
{
    auto it  = mEntries.find(id),
         end = mEntries.end();

    if (it == end) {
        throw Exception("Failed getting EntryNode");
    }

    return it->second;
}

KeyringNode &
KeyringNode::eachEntry(std::function<void (const Buffer &id, const EntryNode &)> callback)
{
    for (auto &node : mEntries) {
        callback(node.first, node.second);
    }

    return *this;
}

const KeyringNode &
KeyringNode::eachEntry(std::function<void (const Buffer &id, const EntryNode &)> callback) const
{
    for (auto &node : mEntries) {
        callback(node.first, node.second);
    }

    return *this;
}

} // End of main namespace