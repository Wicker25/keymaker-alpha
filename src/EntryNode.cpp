/*!
 * Title ---- km/EntryNode.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/EntryNode.hpp>

namespace km { // Begin main namespace

EntryNode
EntryNode::create()
{
    boost::uuids::uuid uuid = uuids::random_generator()();

    EntryNode entry;

    entry.mId = Buffer::fromString(
        uuids::to_string(uuid)
    );

    return entry;
}

EntryNode
EntryNode::create(const EntryNode &other)
{
    return EntryNode(other);
}

EntryNode
EntryNode::fromFile(const filesystem::path &path)
{
    property_tree::ptree config;
    property_tree::read_xml(path.string(), config);

    EntryNode entry;

    entry.mId = path.filename().string();

    PropertyContainer<EntryNode>::fromConfig(
        entry,
        config.get_child("entry")
    );

    return entry;
}

void
EntryNode::toFile(const filesystem::path &path, const EntryNode &entry)
{
    property_tree::ptree config;

    config.put<std::string>("entry.<xmlattr>.version", "1.0");

    config.put_child("entry", PropertyContainer<EntryNode>::toConfig(entry));

    property_tree::write_xml(
        path.string(),
        config,
        std::locale(),
        property_tree::xml_writer_make_settings<std::string>('\t', 1)
    );
}

EntryNode::EntryNode()
{

}

EntryNode::~EntryNode()
{

}

} // End of main namespace