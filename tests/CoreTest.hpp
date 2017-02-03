/*!
 * Title ---- tests/Core.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_APPLICATION_TEST_HPP__
#define __KM_APPLICATION_TEST_HPP__

#include <gtest/gtest.h>

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Console.hpp>
#include <km/Core.hpp>

#include <memory>

using namespace km;

namespace tests { // Begin test namespace

/*!
 * The Core test case.
 */
class CoreTest : public testing::Test
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
     * The user ID.
     */
    static const Buffer mUserId;

    /*!
     * The repository ID.
     */
    static const Buffer mRepositoryId;

    /*!
     * The passphrase.
     */
    static const Buffer mPassphrase;
};

} // End of test namespace

#endif /* __KM_APPLICATION_TEST_HPP__ */