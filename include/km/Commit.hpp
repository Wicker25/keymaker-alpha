/*!
 * Title ---- km/Commit.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_COMMIT_HPP__
#define __KM_COMMIT_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>

#include <chrono>

#include <git2.h>
#include <git2/commit.h>

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The repository commit.
 */
class Commit
{

public:

    /*!
     * Reads a commit from a Git commit.
     *
     * @param[in] gitCommit The Git commit.
     *
     * @return The repository commit.
     */
    static Commit fromGitCommit(git_commit *gitCommit);

    /*!
     * Destructor method.
     */
    virtual ~Commit();

    /*!
     * Returns the commit hash.
     *
     * @param[in] length The hash length.
     */
    Buffer getHash(std::uint8_t length = 32) const;

    /*!
     * Sets the commit summary.
     *
     * @param[in] summary The summary.
     */
    Commit &setSummary(const Buffer &summary);

    /*!
     * Returns the commit summary.
     */
    const Buffer &getSummary() const;

    /*!
     * Returns the commit message.
     */
    const Buffer &getMessage() const;

    /*!
     * Returns the author name.
     */
    const Buffer &getAuthorName() const;

    /*!
     * Returns the author email.
     */
    const Buffer &getAuthorEmail() const;

    /*!
     * Returns the commit time.
     */
    const std::tm *getTime() const;

protected:

    /*!
     * Constructor method.
     */
    Commit();


    /*!
     * The commit hash.
     */
    Buffer mHash;

    /*!
     * The commit summary.
     */
    Buffer mSummary;

    /*!
     * The commit message.
     */
    Buffer mMessage;

    /*!
     * The author name.
     */
    Buffer mAuthorName;

    /*!
     * The author email.
     */
    Buffer mAuthorEmail;

    /*!
     * The commit time.
     */
    std::tm *mTime;
};

} // End of main namespace

#endif /* __KM_COMMIT_HPP__ */

// Include inline methods
#include <km/Commit-inl.hpp>