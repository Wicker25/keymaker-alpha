/*!
 * Title ---- km/Commit.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/Commit.hpp>

namespace km { // Begin main namespace

Commit
Commit::fromGitCommit(git_commit *gitCommit)
{
    Commit commit;

    auto &hash = commit.mHash;
    hash.resize(GIT_OID_HEXSZ + 1);

    const git_oid *oid = git_commit_id(gitCommit);

    git_oid_tostr
    (
        (char *) hash.data(),
        (size_t) hash.size(),
        oid
    );

    commit.mSummary = Buffer::fromString(git_commit_summary(gitCommit));
    commit.mMessage = Buffer::fromString(git_commit_message(gitCommit));

    const git_signature *author     = git_commit_committer(gitCommit);
    const git_time      *actionTime = &author->when;

    auto offset = (std::time_t) actionTime->offset * 60;
    auto time   = (std::time_t) actionTime->time + offset;

    commit.mTime = gmtime(&time);
    commit.mTime->tm_gmtoff = offset;

    // TODO: move into a RepositoryAuthor?
    commit.mAuthorName  = Buffer::fromString(author->name);
    commit.mAuthorEmail = Buffer::fromString(author->email);

    return commit;
}

Commit::Commit()
{

}

Commit::~Commit()
{

}

Buffer
Commit::getHash(std::uint8_t length) const
{
    if (length > 32) {
        throw Exception("Length must be lesser or equal to 32");
    }

    return Buffer(mHash.cbegin(), mHash.cbegin() + length);
}

} // End of main namespace
