/*!
 * Title ---- km/PublicKey.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_PUBLIC_KEY_HPP__
#define __KM_PUBLIC_KEY_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>
#include <km/RsaKey.hpp>
#include <km/PrivateKey.hpp>

#include <boost/filesystem.hpp>

namespace km { // Begin main namespace

class Encrypter;

/*!
 * The public key.
 */
class PublicKey : protected RsaKey
{
    friend class Encrypter;

public:

    /*!
     * Reads a public key from a file.
     *
     * @param[in] path The key path.
     *
     * @return The public key.
     */
    static PublicKey fromPath(const filesystem::path &path);

    /*!
     * Reads a public key from memory.
     *
     * @param[in] data The key data.
     *
     * @return The public key.
     */
    static PublicKey fromMemory(const Buffer &data);

    /*!
     * Reads a public key from a private key.
     *
     * @param[in] data The private key.
     *
     * @return The public key.
     */
    static PublicKey fromPrivateKey(const PrivateKey &privateKey);

    /*!
     * Constructor method.
     */
    PublicKey();

    /*!
     * Copy constructor method.
     *
     * @param[in] other The other instance.
     */
    PublicKey(const PublicKey &other);

    /*!
     * Destructor method.
     */
	virtual ~PublicKey();

    /*!
     * Returns the PKCS#1 public key.
     */
    Buffer getPkcsKey() const;

protected:

    /*!
     * Constructor method.
     *
     * @param[in] bio The OpenSSL bio.
     */
    explicit PublicKey(BIO *bio);
};

} // End of main namespace

#endif /* __KM_PUBLIC_KEY_HPP__ */

// Include inline methods
#include <km/PublicKey-inl.hpp>