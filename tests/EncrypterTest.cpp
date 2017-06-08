/*!
 * Title ---- tests/EncrypterTest.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include "EncrypterTest.hpp"

namespace tests { // Begin test namespace

std::unique_ptr<PrivateKey> EncrypterTest::mPrivateKey;
std::unique_ptr<PublicKey>  EncrypterTest::mPublicKey;
std::unique_ptr<AccessKey>  EncrypterTest::mAccessKey;
std::unique_ptr<Encrypter>  EncrypterTest::mEncrypter;


void
EncrypterTest::SetUpTestCase()
{
    OpenSSL_add_all_ciphers();

    mPrivateKey = std::make_unique<PrivateKey>(
        PrivateKey::fromPath("tests/fixtures/mykey", Buffer::fromString("passphrase"))
    );

    mPublicKey = std::make_unique<PublicKey>(
        PublicKey::fromPath("tests/fixtures/mykey.pub")
    );

    mAccessKey = std::make_unique<AccessKey>
    (
        AccessKey::create
        (
            Buffer::fromBase64<std::string>("BjwrC0p6oIoJxe3ECoJULVyi1Z9xDJReP0N4VIPAqg4="),
            Buffer::fromBase64<std::string>
            (
                "H/Surhx0Coat/pzngo5TlME/TA9HvGs2C4kg82evSSkRr8VapgxWRwoGG4xFTOYl5s0gGxzONWJqc1h5DTApqS"
                "ga7expYo9uu5zw4aPpGcBSZn2b+LKhBLX8IN+3/6416YBwSiF+wiGFM9PkXp06CrAvlhhWQEc9zdIXVG9Cm8DI"
                "wJPC61Xj3qWg07L4EJOKl9Hx7G7lt60Gjr4OtqlOq5J3k+a5ijrbBhsJeigc/g4Gp07hcJXp7r3FD0g4ofSZsQ"
                "cahDElHOZhptN6YjRZ/ABqxyrsh3pF7kIqMwgo0sbQ6lQsII4dIkqq2wDu4ex9Fxu3FMJWD9JXdFHjohWcFA=="
            )
        )
    );

    mEncrypter = std::make_unique<Encrypter>(
        Encrypter::create(*mAccessKey, *mPrivateKey)
    );
}

void
EncrypterTest::TearDownTestCase()
{
    EVP_cleanup();
}


TEST_F(EncrypterTest, getFingerprint)
{
    auto result = Encrypter::getFingerprint(*mPublicKey);

    auto expected = Buffer::fromBase64<std::string>("alkfNwQWIyXEGftBRav8Z0xDZY92xwSM+MhGKmNfDK4=");

    ASSERT_EQ(expected, result);
}

TEST_F(EncrypterTest, encrypt_AccessKey)
{
    auto accessKey = AccessKey::create
    (
        Buffer::fromBase64<std::string>("alkfNwQWIyXEGftBRav8Z0xDZY92xwSM+MhGKmNfDK4="),
        Buffer::fromBase64<std::string>("BjwrC0p6oIoJxe3ECoJULVyi1Z9xDJReP0N4VIPAqg4=")
    );

    auto result = Encrypter::decrypt<AccessKey>
    (
        Encrypter::encrypt<AccessKey>(accessKey, *mPublicKey),
        *mPrivateKey
    );

    ASSERT_EQ(accessKey.getData(), result.getData());
}

TEST_F(EncrypterTest, encrypt_Buffer)
{
    auto data = Buffer::fromString("abcde");
    auto iv   = Buffer::fromRandom(32);

    auto result = mEncrypter->decrypt
    (
        mEncrypter->encrypt(data, iv),
        iv
    );

    ASSERT_EQ(data, result);
}

TEST_F(EncrypterTest, encrypt_TextNode)
{
    auto text = TextNode::create(
        Buffer::fromString("abcde")
    );

    auto result = mEncrypter->decrypt<TextNode>(
        mEncrypter->encrypt<TextNode>(text)
    );

    ASSERT_EQ(text.getContent(), result.getContent());
}

TEST_F(EncrypterTest, encrypt_PropertyNode)
{
    auto property = PropertyNode::create
    (
        Buffer::fromString("name"),
        Buffer::fromString("value")
    );

    auto result = mEncrypter->decrypt<PropertyNode>(
        mEncrypter->encrypt<PropertyNode>(property)
    );

    ASSERT_EQ(property.getContent(), result.getContent());
}

// TODO: add the `*crypt_KeyringNode` tests

} // End of test namespace