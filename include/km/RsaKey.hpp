/*!
 * Title ---- km/RsaKey.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_RSA_KEY_HPP__
#define __KM_RSA_KEY_HPP__

#include <km.hpp>
#include <km/Exception.hpp>

#include <memory>

#include <boost/filesystem.hpp>

#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

namespace km { // Begin main namespace

class Encrypter;

/*!
 * The RSA key.
 */
class RsaKey
{
    friend class Encrypter;

public:

    /*!
     * Constructor method.
     */
    RsaKey();

    /*!
     * Destructor method.
     */
	virtual ~RsaKey();

protected:

    /*!
     * The RSA data.
     */
    std::shared_ptr<RSA> mRsa;
};

} // End of main namespace

#endif /* __KM_RSA_KEY_HPP__ */

// Include inline methods
#include <km/RsaKey-inl.hpp>