/*!
 * Title ---- km/KeyringNode.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_KEYRING_NODE_HPP__
#define __KM_KEYRING_NODE_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>
#include <km/AccessKey.hpp>
#include <km/PropertyContainer.hpp>
#include <km/EntryNode.hpp>

#include <algorithm>
#include <vector>
#include <map>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The access key map.
 */
typedef std::map<Buffer, AccessKey> AccessKeyMap;

/*!
 * The keyring.
 */
class KeyringNode : public PropertyContainer<KeyringNode>
{

public:

    /*!
     * The configuration file.
     */
    static const filesystem::path ConfigFile;

    /*!
     * The entries directory path.
     */
    static const filesystem::path EntryDir;

    /*!
     * Creates a keyring node.
     *
     * @return The keyring node.
     */
    static KeyringNode create();

    /*!
     * Copies a keyring node.
     *
     * @return The new keyring node.
     */
    static KeyringNode create(const KeyringNode &other);

    /*!
     * Builds a new keyring from a path.
     *
     * @param[in] path The keyring path.
     *
     * @return The keyring.
     */
    static KeyringNode fromPath(const filesystem::path &path);

    /*!
     * Stores a keyring into a path.
     *
     * @param[in] path    The keyring path.
     * @param[in] keyring The keyring.
     *
     * @return The keyring.
     */
    static void toPath(const filesystem::path &path, const KeyringNode &keyring);

    /*!
     * Destructor method.
     */
	virtual ~KeyringNode();

    /*!
     * TODO
     */
    KeyringNode &addAccessKey(const AccessKey &accessKey);

    /*!
     * Returns the access key associated with a specified recipient.
     * TODO
     */
	const AccessKey &getAccessKey(const Buffer &recipient) const;

    // FIXME
    KeyringNode &setAccessKeys(const AccessKeyMap &accessKeys);
    const AccessKeyMap &getAccessKeys() const;


    /*!
     * Sets an entry into the keyring.
     *
     * @param[in] entry The entry instance.
     *
     * @return The keyring instance.
     */
    KeyringNode &setEntry(const EntryNode &entry);

    /*!
     * Returns an entry associated with the keyring.
     *
     * @param[in] id The entry ID.
     *
     * @return The entry instance.
     */
	const EntryNode &getEntry(const Buffer &id) const;

    /*!
     * Iterates over the entries in the current keyring.
     *
     * @param[in] callback The callback function.
     *
     * @return The keyring instance.
     */
    KeyringNode &eachEntry(std::function<void (const Buffer &id, const EntryNode &)> callback);

    // FIXME
    const KeyringNode &eachEntry(std::function<void (const Buffer &id, const EntryNode &)> callback) const;

    // TODO: add mapEntry

    // FIXME
    KeyringNode &save(const filesystem::path &path);

protected:

    /*!
     * Constructor method.
     */
    KeyringNode();


    /*!
     * The keyring ID.
     */
    Buffer mId;

    /*!
     * The keyring local path.
     */
    filesystem::path mPath;

    /*!
     * The keyring properties.
     */
    PropertyMap mProperties;

    /*!
     * The access keys.
     */
    AccessKeyMap mAccessKeys;

    /*!
     * The keyring entries.
     */
    std::map<Buffer, EntryNode> mEntries;
};

} // End of main namespace

#endif /* __KM_KEYRING_NODE_HPP__ */

// Include inline methods
#include <km/KeyringNode-inl.hpp>