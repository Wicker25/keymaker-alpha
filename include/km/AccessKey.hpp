/*!
 * Title ---- km/AccessKey.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_ACCESS_KEY_HPP__
#define __KM_ACCESS_KEY_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>

using namespace boost;

namespace km { // Begin main namespace

class Encrypter;

/*!
 * The access key.
 */
class AccessKey
{
    friend class Encrypter;

public:

    /*!
     * Creates a new access key.
     *
     * @param[in] recipient The recipient key.
     * @param[in] data      The key data.
     *
     * @return The access key.
     */
    static AccessKey create(const Buffer &recipient, const Buffer &data);

    /*!
     * Builds a new access key from configuration.
     *
     * @param[in] config The property tree of the configuration.
     *
     * @return The access key.
     */
    static AccessKey fromConfig(property_tree::ptree &config);

    /*!
     * Convert an access key into a property tree.
     *
     * @param[in] accessKey The access key.
     *
     * @return The configuration node.
     */
    static property_tree::ptree toConfig(const AccessKey &accessKey);

    /*!
     * Constructor method.
     */
    AccessKey();

    /*!
     * Copy constructor method.
     *
     * @param[in] other The other.
     */
	AccessKey(const AccessKey &other);

    /*!
     * Destructor method.
     */
	virtual ~AccessKey();

    /*!
     * Sets the key recipient.
     *
     * @param[in] recipient The recipient key.
     *
     * @return The access key.
     */
	AccessKey &setRecipient(const Buffer &recipient);

    /*!
     * Returns the recipient key.
     */
    const Buffer &getRecipient() const;

    /*!
     * Sets the key data.
     *
     * @param[in] data The key data.
     *
     * @return The access key.
     */
    AccessKey &setData(const Buffer &data);

    /*!
     * Returns the key data.
     */
    const Buffer &getData() const;

protected:

    /*!
     * The recipient key.
     */
    Buffer mRecipient;

    /*!
     * The key data.
     */
    Buffer mData;
};

} // End of main namespace

#endif /* __KM_ACCESS_KEY_HPP__ */

// Include inline methods
#include <km/AccessKey-inl.hpp>