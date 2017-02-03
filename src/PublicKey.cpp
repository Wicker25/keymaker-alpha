/*!
 * Title ---- km/PublicKey.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/PublicKey.hpp>

namespace km { // Begin main namespace

PublicKey
PublicKey::fromPath(const filesystem::path &path)
{
    BIO *bio = BIO_new_file(path.c_str(), "r");

    if (!bio) {
        throw Exception("Unable to read PEM");
    }

    PublicKey publicKey(bio);

    BIO_free(bio);

    return publicKey;
}

PublicKey
PublicKey::fromMemory(const Buffer &data)
{
    BIO *bio = BIO_new_mem_buf((const void *) data.data(), (int) data.size());

    if (!bio) {
        throw Exception("Unable to read PEM");
    }

    PublicKey publicKey(bio);

    BIO_free(bio);

    return publicKey;
}

PublicKey
PublicKey::fromPrivateKey(const PrivateKey &privateKey)
{
    Buffer buffer(4096);

    BIO *bio = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPublicKey(bio, privateKey.mRsa.get());

    size_t length = (size_t) BIO_read(
        bio,
        (void *) buffer.data(),
        (int) buffer.size()
    );

    BIO_free(bio);

    buffer.resize(length);

    return PublicKey::fromMemory(buffer);
}

PublicKey::PublicKey()
{

}

PublicKey::PublicKey(const PublicKey &other) : RsaKey(other)
{

}

PublicKey::PublicKey(BIO *bio)
{
    RSA *rsa = NULL;

    if (!PEM_read_bio_RSAPublicKey(bio, &rsa, NULL, NULL)) {
        throw Exception("Failed getting RSA");
    }

    mRsa = std::shared_ptr<RSA>(rsa, [](RSA *data) {
        RSA_free(data);
    });
}

PublicKey::~PublicKey()
{

}

Buffer
PublicKey::getPkcsKey() const
{
    Buffer output(4096);

    BIO *bio = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPublicKey(bio, mRsa.get());

    size_t length = (size_t) BIO_read(
        bio,
        (void *) output.data(),
        (int) output.size()
    );

    BIO_free(bio);

    output.resize(length);

    return output;
}

} // End of main namespace