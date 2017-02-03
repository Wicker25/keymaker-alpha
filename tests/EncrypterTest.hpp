/*!
 * Title ---- tests/Encrypter.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_ENCRYPTER_TEST_HPP__
#define __KM_ENCRYPTER_TEST_HPP__

#include <gtest/gtest.h>

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Encrypter.hpp>

#include <memory>

using namespace km;

namespace tests { // Begin test namespace

/*!
 * The Encrypter test case.
 */
class EncrypterTest : public testing::Test
{

public:

    /*!
     * Sets up the test case.
     */
    static void SetUpTestCase();

    /*!
     * Cleans up the test case.
     */
    static void TearDownTestCase();

protected:

    /*!
     * The private key.
     */
    static std::unique_ptr<PrivateKey> mPrivateKey;

    /*!
     * The public key.
     */
    static std::unique_ptr<PublicKey> mPublicKey;

    /*!
     * The access key.
     */
    static std::unique_ptr<AccessKey> mAccessKey;

    /*!
     * The encrypter.
     */
    static std::unique_ptr<Encrypter> mEncrypter;
};

} // End of test namespace

#endif /* __KM_ENCRYPTER_TEST_HPP__ */