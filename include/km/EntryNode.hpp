/*!
 * Title ---- km/EntryNode.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_ENTRY_NODE_HPP__
#define __KM_ENTRY_NODE_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>
#include <km/PropertyContainer.hpp>

#include <boost/filesystem.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The repository entry.
 */
class EntryNode : public PropertyContainer<EntryNode>
{

public:

	/*!
     * Creates an entry node.
     *
     * @return The entry node.
     */
	static EntryNode create();

    /*!
     * Copies a entry node.
     *
     * @return The new entry node.
     */
    static EntryNode create(const EntryNode &other);

    /*!
     * Reads an entry from a file.
     *
     * @param[in] path The path to the local repository.
     *
     * @return The entry node.
     */
    static EntryNode fromFile(const filesystem::path &path);

    /*!
     * Writes an entry into a file.
     *
     * @param[in] path  The path to the local repository.
     * @param[in] entry The entry node.
     */
    static void toFile(const filesystem::path &path, const EntryNode &entry);

    /*!
     * Destructor method.
     */
	virtual ~EntryNode();

    /*!
     * Returns the entry ID.
     */
    const Buffer &getId() const;

private:

	/*!
     * Constructor method.
     */
	EntryNode();


    /*!
     * The entry ID.
     */
    Buffer mId;
};

} // End of main namespace

#endif /* __KM_ENTRY_NODE_HPP__ */

// Include inline methods
#include <km/EntryNode-inl.hpp>