/*!
 * Title ---- km/PropertyNode.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_PROPERTY_NODE_HPP__
#define __KM_PROPERTY_NODE_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>

#include <map>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace boost;

namespace km { // Begin main namespace

class Encrypter;

/*!
 * The entry property.
 */
class PropertyNode
{
    friend class Encrypter;

public:

	/*!
     * Creates a property node.
     *
     * @param[in] name    The name.
     * @param[in] content The content.
     *
     * @return The property node.
     */
	static PropertyNode create(const Buffer &name, const Buffer &content);

    /*!
     * Creates a property node.
     *
     * @param[in] name    The name.
     * @param[in] content The content.
     * @param[in] nonce   The nonce.
     *
     * @return The property node.
     */
    static PropertyNode create(const Buffer &name, const Buffer &content, const Buffer &nonce);

    /*!
     * Creates a property node from a property tree.
     *
     * @param[in] config The property tree.
     *
     * @return The property node.
     */
    static PropertyNode fromConfig(property_tree::ptree &config);

	/*!
     * Converts a property node into a property tree.
     *
     * @param[in] property The property node.
     *
     * @return The configuration node.
     */
	static property_tree::ptree toConfig(const PropertyNode &property);

    /*!
     * Destructor method.
     */
	virtual ~PropertyNode();

    /*!
     * Sets the name.
     */
	PropertyNode &setName(const Buffer &name);

    /*!
     * Returns the name.
     */
	const Buffer &getName() const;

    /*!
     * Sets the value.
     *
     * @param[in] value The value.
     */
	PropertyNode &setContent(const Buffer &value);

    /*!
     * Returns the value.
     */
	const Buffer &getContent() const;

	/*!
     * Sets the nonce.
     */
	PropertyNode &setNonce(const Buffer &nonce);

	/*!
     * Returns the nonce.
     */
	const Buffer &getNonce() const;

protected:

	/*!
     * Constructor method.
     */
	PropertyNode();


	/*!
     * The name.
     */
	Buffer mName;

    /*!
     * The content.
     */
	Buffer mContent;

	/*!
     * The nonce.
     */
	Buffer mNonce;
};

} // End of main namespace

#endif /* __KM_PROPERTY_NODE_HPP__ */

// Include inline methods
#include <km/PropertyNode-inl.hpp>