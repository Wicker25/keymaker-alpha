/*!
 * Title ---- km/Encrypter.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_ENCRYPTER_HPP__
#define __KM_ENCRYPTER_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>
#include <km/RsaKey.hpp>
#include <km/PrivateKey.hpp>
#include <km/PublicKey.hpp>
#include <km/AccessKey.hpp>
#include <km/PropertyNode.hpp>
#include <km/TextNode.hpp>

#include <memory>
#include <algorithm>

#include <openssl/aes.h>
#include <openssl/err.h>

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The encrypter.
 */
class Encrypter
{

public:

    /*!
     * Encryption parameters.
     */
    enum {

        ENCRYPT_KEY_LENGTH     = 256 / 8,        // 256 bit
        ENCRYPT_IV_LENGTH      = 128 / 8,        // 128 bit
        ENCRYPT_BLOCK_SIZE     = AES_BLOCK_SIZE,
        ENCRYPT_ROUNDS         = 30000,

        KEY_FINGERPRINT_LENGTH = 32,

        TEXT_NODE_SIZE         = 1024,
        PROPERTY_NODE_SIZE     = 128,

        PADDING_DIVISOR        = '\7'
    };

    /*!
     * Encrypts data by using a public key.
     *
     * @param[in] input     The data to encrypt.
     * @param[in] publicKey The public key.
     *
     * @return The encrypted data.
     */
    template <class T>
    static T encrypt(const T &input, const PublicKey &publicKey);

    /*!
     * Decrypts data by using a private key.
     *
     * @param[in] input      The data to decrypt.
     * @param[in] privateKey The private key.
     *
     * @return The decrypted data.
     */
    template <class T>
    static T decrypt(const T &input, const PrivateKey &privateKey);

    /*!
     * Returns the fingerprint of a key.
     *
     * @param[in] key The private or public key.
     *
     * @return The fingerprint.
     */
    template <class T>
    static Buffer getFingerprint(const T &key);

    /*!
     * Creates a new encrypter.
     *
     * @param[in] accessKey  The access key.
     * @param[in] privateKey The private key.
     *
     * @return The encrypter.
     */
    static Encrypter create(const AccessKey &accessKey, const PrivateKey &privateKey);

    /*!
     * Destructor method.
     */
	virtual ~Encrypter();

    /*!
     * Encrypts data.
     *
     * @param[in] input The data to encrypt.
     * @param[in] iv    The initialization vector.
     *
     * @return The encrypted data.
     */
    Buffer encrypt(const Buffer &input, const Buffer &iv) const;

    /*!
     * Decrypts data.
     *
     * @param[in] input The data to decrypt.
     * @param[in] iv    The initialization vector.
     *
     * @return The decrypted data.
     */
    Buffer decrypt(const Buffer &input, const Buffer &iv) const;

    /*!
     * Encrypts data.
     *
     * @param[in] input The data to encrypt.
     *
     * @return The encrypted data.
     */
    template <class T>
    T encrypt(const T &input) const;

    /*!
     * Decrypts data.
     *
     * @param[in] input The data to decrypt.
     *
     * @return The decrypted data.
     */
    template <class T>
    T decrypt(const T &data) const;

protected:

    /*!
     * Constructor method.
     */
    Encrypter();

    /*!
     * Adds padding to an buffer.
     *
     * @param[in] buffer The buffer to which add the padding.
     * @param[in] size   The final size of the data block.
     *
     * @return The processed buffer.
     */
    Buffer addPadding(const Buffer &buffer, size_t size) const;

    /*!
     * Removes padding from a buffer.
     *
     * @param[in] buffer The buffer from which remove the padding.
     *
     * @return The processed buffer.
     */
    Buffer removePadding(const Buffer &buffer) const;


    /*!
     * The OpenSSL cipher context.
     */
    std::shared_ptr<EVP_CIPHER_CTX> mCipherContext;

    /*!
     * The access key.
     */
    AccessKey mAccessKey;
};

} // End of main namespace

#endif /* __KM_ENCRYPTER_HPP__ */

// Include inline methods
#include <km/Encrypter-inl.hpp>