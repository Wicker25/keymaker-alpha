/*!
 * Title ---- tests/CoreTest.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include "CoreTest.hpp"

namespace tests { // Begin test namespace

const Buffer
CoreTest::mUserId = Buffer::fromString("123f21bf-f513-4593-a946-ea64e3b0b8eb");

const Buffer
CoreTest::mRepositoryId = Buffer::fromString("5af99f42-badb-4768-8e77-aa7e3a7972bf");

const Buffer
CoreTest::mPassphrase = Buffer::fromString("passphrase");


void
CoreTest::SetUpTestCase()
{
    Console::setDebug(false);
}

void
CoreTest::TearDownTestCase()
{
    Console::setDebug(true);
}


TEST_F(CoreTest, authenticate)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
        app.authenticate(mPassphrase);
    });
}

TEST_F(CoreTest, createRepository)
{
    auto repositoryName = Buffer::fromString("My Repository");

    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
        app
            .authenticate(mPassphrase)
            .createRepository(repositoryName)
        ;
    });
}

TEST_F(CoreTest, openRepository)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
        app
            .authenticate(mPassphrase)
            .openRepository(mRepositoryId)
        ;
    });
}

TEST_F(CoreTest, shareRepository)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
        app
            .authenticate(mPassphrase)
            .openRepository(mRepositoryId)
            .shareRepository(mUserId)
        ;
    });
}

TEST_F(CoreTest, setRepositoryEntry)
{
    auto title   = Buffer::fromString("title"),
         content = Buffer::fromString("TestEntry");

    auto &app = Core::get();

    auto property = PropertyNode::create(title, content);

    auto entry = EntryNode::create();
    entry.setProperty(property.getName(), property);

    EXPECT_NO_THROW
    ({
         app
             .authenticate(mPassphrase)
             .openRepository(mRepositoryId)
             .setRepositoryEntry(entry)
         ;
     });
}

TEST_F(CoreTest, getRepositoryEntry)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
         app
             .authenticate(mPassphrase)
             .openRepository(mRepositoryId)
             .getRepositoryEntry(mUserId)
         ;
     });
}

TEST_F(CoreTest, commitRepository)
{
    auto commitMessage = Buffer::fromString("Commit message");

    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
        app
            .authenticate(mPassphrase)
            .openRepository(mRepositoryId)
            .commitRepository(commitMessage)
        ;
    });
}

TEST_F(CoreTest, pushRepository)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
        app
            .authenticate(mPassphrase)
            .openRepository(mRepositoryId)
            .pushRepository()
        ;
    });
}

TEST_F(CoreTest, fetchRepository)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
        app
            .authenticate(mPassphrase)
            .openRepository(mRepositoryId)
            .fetchRepository()
        ;
    });
}

TEST_F(CoreTest, logRepository)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
        app
            .authenticate(mPassphrase)
            .openRepository(mRepositoryId)
            .eachRepositoryLogs("HEAD~20..HEAD", [this](const Buffer &id, const Commit &commit) {
                // TODO
            });
        ;
    });
}

TEST_F(CoreTest, deleteRepository)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
         app
             .authenticate(mPassphrase)
             .openRepository(mRepositoryId)
             //.deleteRepository() // TODO: add an undeletable repository
         ;
     });
}

TEST_F(CoreTest, listRepositories)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
         app
             .authenticate(mPassphrase)
             .listRepositories()
         ;
    });
}

TEST_F(CoreTest, fetchRepositories)
{
    auto &app = Core::get();

    EXPECT_NO_THROW
    ({
         app
             .authenticate(mPassphrase)
             .fetchRepositories()
         ;
     });
}

} // End of test namespace