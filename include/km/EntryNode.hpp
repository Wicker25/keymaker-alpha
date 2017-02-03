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
#include <km/PropertyNode.hpp>

#include <map>

#include <boost/filesystem.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The property map.
 */
typedef std::map<Buffer, PropertyNode> PropertyMap;

/*!
 * The repository entry.
 */
class EntryNode
{

public:

	/*!
     * Creates an entry node.
     *
     * @return The entry node.
     */
	static EntryNode create();

    /*!
     * Creates an entry node.
     *
     * @param[in] id The entry ID.
     *
     * @return The entry node.
     */
    static EntryNode create(const Buffer &id);

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

    /*!
     * Sets a property in the entry.
     *
     * @param[in] name     The property name.
     * @param[in] property The property instance.
     *
     * @return The entry node.
     */
	EntryNode &setProperty(const Buffer &name, const PropertyNode &property);

	/*!
     * Sets a property in the entry.
     *
     * @param[in] name  The property name.
     * @param[in] value The property value.
     *
     * @return The entry node.
     */
	EntryNode &setProperty(const Buffer &name, const Buffer &value);

    /*!
     * Returns a property in the entry.
     *
     * @param[in] name The property name.
     *
     * @return The property node.
     */
	const PropertyNode &getProperty(const Buffer &name) const;

	/*!
     * Removes a property from the entry.
     *
     * @param[in] name The property name.
     *
     * @return The entry node.
     */
	EntryNode &removeProperty(const Buffer &name);

    /*!
     * Iterates over the properties in the entry.
     *
     * @param[in] callback The callback function.
     *
     * @return The entry node.
     */
	EntryNode &eachProperty(std::function<void (const Buffer &, PropertyNode &)> callback);

	/*!
     * Iterates over the properties in the entry.
     *
     * @param[in] callback The callback function.
     *
     * @return The entry node.
     */

    // FIXME
    const EntryNode &eachProperty(std::function<void (const Buffer &, const PropertyNode &)> callback) const;

    EntryNode mapProperty(std::function<PropertyNode (const Buffer &, const PropertyNode &)> callback) const;

private:

	/*!
     * Constructor method.
     */
	EntryNode();


    /*!
     * The entry ID.
     */
    Buffer mId;

    /*!
     * The entry properties.
     */
    PropertyMap mProperties;
};


/*!
 * The property not found exception.
 */
class PropertyNotFoundException : public Exception
{
	using Exception::Exception;
};

} // End of main namespace

#endif /* __KM_ENTRY_NODE_HPP__ */

// Include inline methods
#include <km/EntryNode-inl.hpp>