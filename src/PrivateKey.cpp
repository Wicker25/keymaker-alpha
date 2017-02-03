/*!
 * Title ---- km/PrivateKey.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/PrivateKey.hpp>

namespace km { // Begin main namespace

PrivateKey
PrivateKey::fromPath(const filesystem::path &path, const Buffer &passphrase)
{
    BIO *bio = BIO_new_file(path.c_str(), "r");

    if (!bio) {
        throw Exception("Unable to read PEM");
    }

    PrivateKey privateKey(bio, passphrase);

    BIO_free(bio);

    return privateKey;
}

PrivateKey
PrivateKey::fromMemory(const Buffer &data, const Buffer &passphrase)
{
    BIO *bio = BIO_new_mem_buf((const void *) data.data(), (int) data.size());

    if (!bio) {
        throw Exception("Unable to read PEM");
    }

    PrivateKey privateKey(bio, passphrase);

    BIO_free(bio);

    return privateKey;
}

PrivateKey::PrivateKey()
{

}

PrivateKey::PrivateKey(const PrivateKey &other) : RsaKey(other)
{

}

PrivateKey::PrivateKey(BIO *bio, const Buffer &passphrase)
{
    RSA *rsa = NULL;

    Buffer password = passphrase;
    password.push_back('\0');

    if (!PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, (void *) password.data())) {
        throw Exception("Failed getting RSA");
    }

    mRsa = std::shared_ptr<RSA>(rsa, [](RSA *data) {
        RSA_free(data);
    });
}

PrivateKey::~PrivateKey()
{

}

Buffer
PrivateKey::getPkcsKey() const
{
    Buffer output(4096);

    BIO *bio = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(bio, mRsa.get(), NULL, NULL, 0, NULL, 0);

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