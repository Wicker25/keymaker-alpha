/*!
 * Title ---- km/Encrypter.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/Encrypter.hpp>

namespace km { // Begin main namespace

template <>
AccessKey
Encrypter::encrypt<AccessKey>(const AccessKey &input, const PublicKey &publicKey)
{
    Buffer buffer(2048);

    int length = RSA_public_encrypt
    (
        (int)             input.mData.size(),
        (unsigned char *) input.mData.data(),
        (unsigned char *) buffer.data(),
        (RSA *)           publicKey.mRsa.get(),
        RSA_PKCS1_PADDING
    );

    if (length == -1) {
        throw Exception("Failed to encryptData RSA");
    }

    buffer.resize((size_t) length);

    AccessKey output;

    output
        .setRecipient(input.getRecipient())
        .setData(buffer);

    return output;
}

template <>
AccessKey
Encrypter::decrypt<AccessKey>(const AccessKey &input, const PrivateKey &privateKey)
{
    Buffer buffer(2048);

    int length = RSA_private_decrypt
    (
        (int)             input.mData.size(),
        (unsigned char *) input.mData.data(),
        (unsigned char *) buffer.data(),
        (RSA *)           privateKey.mRsa.get(),
        RSA_PKCS1_PADDING
    );

    if (length == -1) {
        throw Exception("Failed to decryptData RSA");
    }

    buffer.resize((size_t) length);

    AccessKey output;

    output
        .setRecipient(input.getRecipient())
        .setData(buffer);

    return output;
}

Encrypter
Encrypter::create(const AccessKey &accessKey, const PrivateKey &privateKey)
{
    Encrypter crypter;

    crypter.mAccessKey = Encrypter::decrypt<AccessKey>(accessKey, privateKey);

    return crypter;
}

Encrypter::Encrypter()
{
    EVP_CIPHER_CTX *cipherContext = EVP_CIPHER_CTX_new();

    mCipherContext = std::shared_ptr<EVP_CIPHER_CTX>(cipherContext, [](EVP_CIPHER_CTX *data) {
        EVP_CIPHER_CTX_free(data);
    });
}

Encrypter::~Encrypter()
{

}

template <>
TextNode
Encrypter::encrypt<TextNode>(const TextNode &input) const
{
    auto nonce = Buffer::fromRandom(ENCRYPT_IV_LENGTH);

    auto content = addPadding(input.getContent(), TEXT_NODE_SIZE);

    return TextNode::create
    (
        encrypt(content, nonce),
        nonce
    );
}

template <>
TextNode
Encrypter::decrypt<TextNode>(const TextNode &input) const
{
    auto nonce = input.getNonce();

    auto content = decrypt(input.getContent(), nonce);

    return TextNode::create
    (
        removePadding(content),
        nonce
    );
}

template <>
PropertyNode
Encrypter::encrypt<PropertyNode>(const PropertyNode &input) const
{
    auto nonce = Buffer::fromRandom(ENCRYPT_IV_LENGTH);

    auto name    = addPadding(input.getName(),    PROPERTY_NODE_SIZE);
    auto content = addPadding(input.getContent(), PROPERTY_NODE_SIZE);

    return PropertyNode::create
    (
        encrypt(name, nonce),
        encrypt(content, nonce),
        nonce
    );
}

template <>
PropertyNode
Encrypter::decrypt<PropertyNode>(const PropertyNode &input) const
{
    auto nonce = input.getNonce();

    auto name    = decrypt(input.getName(),    nonce);
    auto content = decrypt(input.getContent(), nonce);

    return PropertyNode::create
    (
        removePadding(name),
        removePadding(content),
        nonce
    );
}

Buffer
Encrypter::encrypt(const Buffer &input, const Buffer &iv) const
{
    int chunk, error, length = 0;

    Buffer output(input.size() + ENCRYPT_BLOCK_SIZE);

    EVP_CIPHER_CTX_init(mCipherContext.get());

    error = EVP_EncryptInit_ex
    (
        mCipherContext.get(),
        EVP_aes_256_cbc(),
        NULL,
        (unsigned char *) mAccessKey.mData.data(),
        (unsigned char *) iv.data()
    );

    if (!error) {
        throw Exception("Error with 'EVP_EncryptInit_ex'");
    }

    error = EVP_EncryptUpdate
    (
        mCipherContext.get(),
        output.data(),
        &chunk,
        (unsigned char *) input.data(),
        (int) input.size()
    );

    if (!error) {
        throw Exception("Error with 'EVP_EncryptUpdate'");
    }

    length += chunk;

    error = EVP_EncryptFinal_ex
    (
        mCipherContext.get(),
        output.data() + length,
        &chunk
    );

    if (!error) {
        throw Exception("Error with 'EVP_EncryptFinal_ex'");
    }

    length += chunk;

    EVP_CIPHER_CTX_cleanup(mCipherContext.get());

    output.resize((size_t) length);

    return output;
}

Buffer
Encrypter::decrypt(const Buffer &input, const Buffer &iv) const
{
    int chunk, error, length = 0;

    Buffer output(input.size() + ENCRYPT_BLOCK_SIZE);

    EVP_CIPHER_CTX_init(mCipherContext.get());

    error = EVP_DecryptInit_ex
    (
        mCipherContext.get(),
        EVP_aes_256_cbc(),
        NULL,
        (unsigned char *) mAccessKey.mData.data(),
        (unsigned char *) iv.data()
    );

    if (!error) {
        throw Exception("Error with 'EVP_DecryptInit_ex'");
    }

    error = EVP_DecryptUpdate
    (
        mCipherContext.get(),
        output.data(),
        &chunk,
        (unsigned char *) input.data(),
        (int) input.size()
    );

    if (!error) {
        throw Exception("Error with 'EVP_DecryptUpdate'");
    }

    length += chunk;

    error = EVP_DecryptFinal_ex
    (
        mCipherContext.get(),
        output.data() + length,
        &chunk
    );

    if (!error) {
        throw Exception("Error with 'EVP_DecryptFinal_ex'");
    }

    length += chunk;

    EVP_CIPHER_CTX_cleanup(mCipherContext.get());

    output.resize((size_t) length);

    return output;
}

Buffer
Encrypter::addPadding(const Buffer &buffer, size_t size) const
{
    Buffer output = Buffer::fromRandom(size + 1);

    std::copy(buffer.cbegin(), buffer.cend(), output.begin());
    output[buffer.size()] = PADDING_DIVISOR;

    return output;
}

Buffer
Encrypter::removePadding(const Buffer &buffer) const
{
    Buffer output(buffer);

    auto it = std::find(output.cbegin(), output.cend(), PADDING_DIVISOR);
    output.resize((std::size_t) std::distance(output.cbegin(), it));

    return output;
}

} // End of main namespace