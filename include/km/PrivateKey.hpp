/*!
 * Title ---- km/PrivateKey.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_PRIVATE_KEY_HPP__
#define __KM_PRIVATE_KEY_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>
#include <km/RsaKey.hpp>

#include <boost/filesystem.hpp>

namespace km { // Begin main namespace

class PublicKey;
class Encrypter;

/*!
 * The private key.
 */
class PrivateKey : protected RsaKey
{
    friend class Encrypter;
    friend class PublicKey;

public:

    /*!
     * Reads a private key from a file.
     *
     * @param[in] path       The key path.
     * @param[in] passphrase The key passphrase.
     *
     * @return The private key.
     */
    static PrivateKey fromPath(const filesystem::path &path, const Buffer &passphrase);

    /*!
     * Reads a private key from memory.
     *
     * @param[in] data       The key data.
     * @param[in] passphrase The key passphrase.
     *
     * @return The private key.
     */
    static PrivateKey fromMemory(const Buffer &data, const Buffer &passphrase);

    /*!
     * Constructor method.
     */
    PrivateKey();

    /*!
     * Copy constructor method.
     *
     * @param[in] other The other instance.
     */
    PrivateKey(const PrivateKey &other);

    /*!
     * Destructor method.
     */
	virtual ~PrivateKey();

    /*!
     * Returns the PKCS#1 private key.
     */
    Buffer getPkcsKey() const;

protected:

	/*!
     * Constructor method.
     *
     * @param[in] bio        The OpenSSL bio.
     * @param[in] passphrase The key passphrase.
     */
    explicit PrivateKey(BIO *bio, const Buffer &passphrase);
};

} // End of main namespace

#endif /* __KM_PRIVATE_KEY_HPP__ */

// Include inline methods
#include <km/PrivateKey-inl.hpp>