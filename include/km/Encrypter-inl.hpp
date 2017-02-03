/*!
 * Title ---- km/Encrypter-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_ENCRYPTER_INL_HPP__
#define __KM_ENCRYPTER_INL_HPP__

namespace km { // Begin main namespace

template <class T>
Buffer
Encrypter::getFingerprint(const T &key)
{
    auto rsa = key.mRsa;

    size_t nLenght = (size_t) BN_num_bytes(rsa->n),
           eLenght = (size_t) BN_num_bytes(rsa->e);

    Buffer buffer(nLenght + eLenght);

    BN_bn2bin(rsa->n, buffer.data());
    BN_bn2bin(rsa->e, buffer.data() + nLenght);

    Buffer fingerprint(KEY_FINGERPRINT_LENGTH);

    if (!EVP_Digest(buffer.data(), buffer.size(), fingerprint.data(), NULL, EVP_sha256(), NULL)) {
        throw Exception("Failed to generate fingerprint");
    }

    return fingerprint;
}

} // End of main namespace

#endif /* __KM_ENCRYPTER_INL_HPP__ */
