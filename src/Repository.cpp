/*!
 * Title ---- km/Repository.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/Repository.hpp>

namespace km { // Begin main namespace

int
Repository::acquireCredentials(git_cred **out, const char *, const char *, unsigned int, void *privateKey)
{
    auto pkcsKey = static_cast<PrivateKey *>(privateKey)->getPkcsKey();

    int error = git_cred_ssh_key_memory_new
    (
        out,
        GIT_USERNAME,
        NULL,
        (const char *) pkcsKey.data(),
        NULL
    );

    if (error < 0) {
        throwGitException();
    }

    return error;
}

void
Repository::throwGitException()
{
    throw Exception("Repository error: %1%, %2%", giterr_last()->klass, giterr_last()->message);
}

Repository
Repository::create(const Buffer &id, const filesystem::path &path, const PrivateKey &privateKey)
{
    Repository repository(id, path);

    BEGIN_TASK("git", "Build repository");

    repository
        .access(privateKey)
        .init()
        .open()
    ;

    END_TASK();

    return repository;
}

Repository
Repository::fromPath(const Buffer &id, const filesystem::path &path, const PrivateKey &privateKey)
{
    Repository repository(id, path);

    repository
        .access(privateKey)
        .open()
    ;

    return repository;
}

Repository
Repository::fromRemote(const Buffer &id, const filesystem::path &path, const PrivateKey &privateKey)
{
    Repository repository(id, path);

    BEGIN_TASK("git", "Clone repository");

    repository
        .access(privateKey)
        .clone()
        .open()
    ;

    END_TASK();

    return repository;
}

bool
Repository::isValid(const filesystem::path &path)
{
    if (git_repository_open_ext(NULL, path.c_str(), GIT_REPOSITORY_OPEN_NO_SEARCH, NULL) == 0) {
        return true;
    }

    return false;
}

Repository::Repository(const Buffer &id, const filesystem::path &path) : mId(id), mPath(path)
{

}

Repository::~Repository()
{

}

Repository &
Repository::add(const filesystem::path &path)
{
    git_index *idx;

    int error;

    error = git_repository_index(&idx, mGitRepository.get());

    if (error < 0) {
        throwGitException();
    }

    error = git_index_read(idx, 1);

    if (error < 0) {
        throwGitException();
    }

    const char *paths[] = {
        path.c_str()
    };

    git_strarray pathspec = { (char **) paths, 2 };

    error = git_index_add_all(idx, &pathspec, GIT_INDEX_ADD_DEFAULT, NULL, NULL);

    if (error < 0) {
        throwGitException();
    }

    git_index_write(idx);
    git_index_free(idx);

    return *this;
}

Repository &
Repository::update(const filesystem::path &path)
{
    git_index *idx;

    int error;

    error = git_repository_index(&idx, mGitRepository.get());

    if (error < 0) {
        throwGitException();
    }

    error = git_index_read(idx, 1);

    if (error < 0) {
        throwGitException();
    }

    const char *paths[] = {
        path.c_str()
    };

    git_strarray pathspec = { (char **) paths, 2 };

    error = git_index_update_all(idx, &pathspec, NULL, NULL);

    if (error < 0) {
        throwGitException();
    }

    git_index_write(idx);
    git_index_free(idx);

    return *this;
}

Repository &
Repository::remove(const filesystem::path &path)
{
    git_index *idx;

    int error;

    error = git_repository_index(&idx, mGitRepository.get());

    if (error < 0) {
        throwGitException();
    }

    error = git_index_read(idx, 1);

    if (error < 0) {
        throwGitException();
    }

    const char *paths[] = {
        path.c_str()
    };

    git_strarray pathspec = { (char **) paths, 2 };

    error = git_index_remove_all(idx, &pathspec, NULL, NULL);

    if (error < 0) {
        throwGitException();
    }

    git_index_write(idx);
    git_index_free(idx);

    return *this;
}

Repository &
Repository::commit(const Buffer &message)
{
    BEGIN_TASK("git", "Commit changes");

    git_index *idx;
    git_oid new_tree_id;
    git_tree *tree;
    git_signature *me;

    git_commit *parent_commit;
    git_oid oid_parent_commit;

    int error;

    error = git_repository_index(&idx, mGitRepository.get());

    if (error < 0) {
        throwGitException();
    }

    error = git_index_read(idx, 1);

    if (error < 0) {
        throwGitException();
    }

    error = git_index_write_tree(&new_tree_id, idx);

    if (error < 0) {
        throwGitException();
    }

    git_index_free(idx);

    error = git_tree_lookup(&tree, mGitRepository.get(), &new_tree_id);

    if (error < 0) {
        throwGitException();
    }

    // FIXME: change this
    error = git_signature_now(&me, "Wicker25", "wicker25@gmail.com");

    if (error < 0) {
        throwGitException();
    }


    error = git_reference_name_to_id(&oid_parent_commit, mGitRepository.get(), "HEAD");

    unsigned int numOfParents = 0;

    const git_commit **parents = NULL;

    if (error == GIT_OK) {

        error = git_commit_lookup(&parent_commit, mGitRepository.get(), &oid_parent_commit);

        if (error < 0) {
            throwGitException();
        }

        numOfParents = 1;

        parents = (const git_commit **) &parent_commit;

    } else if (error == GIT_ENOTFOUND) {

    } else {
        throwGitException();
    }

    git_oid new_commit_id;

    error = git_commit_create
    (
        &new_commit_id,
        mGitRepository.get(),
        "HEAD",
        me,
        me,
        "UTF-8",
        (const char *) message.toString().c_str(),
        tree,
        numOfParents,
        parents
    );

    if (error < 0) {
        throwGitException();
    }

    END_TASK();

    return *this;
}

Repository &
Repository::fetch()
{
    BEGIN_TASK("git", "Fetch changes");

    git_reference *ref;

    int error;

    error = git_reference_lookup(&ref, mGitRepository.get(), "refs/heads/master");

    if (error < 0) {
        throwGitException();
    }

    git_fetch_options options = GIT_FETCH_OPTIONS_INIT;

    options.callbacks.credentials = (git_cred_acquire_cb) &Repository::acquireCredentials;
    options.callbacks.payload = (void *) &mPrivateKey;

    char *refspec = (char *) "refs/heads/master:refs/heads/master";

    const git_strarray refspecs = {
        &refspec,
        1
    };

    error = git_remote_fetch(mGitRemote.get(), &refspecs, &options, NULL);

    if (error < 0) {
        throwGitException();
    }

    git_reference_free(ref);

    END_TASK();

    return *this;
}

Repository &
Repository::push()
{
    BEGIN_TASK("git", "Push changes");

    git_reference *ref;

    int error;

    error = git_reference_lookup(&ref, mGitRepository.get(), "refs/heads/master");

    if (error < 0) {
        throwGitException();
    }

    git_push_options options = GIT_PUSH_OPTIONS_INIT;

    options.callbacks.credentials = (git_cred_acquire_cb) &Repository::acquireCredentials;
    options.callbacks.payload = (void *) &mPrivateKey;

    char *refspec = (char *) "refs/heads/master:refs/heads/master";

    const git_strarray refspecs = {
        &refspec,
        1
    };

    error = git_remote_push(mGitRemote.get(), &refspecs, &options);

    if (error < 0) {
        throwGitException();
    }

    git_reference_free(ref);

    END_TASK();

    return *this;
}

Repository &
Repository::init()
{
    git_repository *repository;

    int error;

    error = git_repository_init(&repository, mPath.c_str(), 0);

    if (error < 0) {
        throwGitException();
    }

    mGitRepository = std::shared_ptr<git_repository>(repository, [](git_repository *data) {
        git_repository_free(data);
    });

    auto remoteUrl = boost::format(GIT_USERNAME "@" GIT_ADDRESS ":%1%") % mId.toString();

    error = git_remote_set_url
    (
        mGitRepository.get(),
        "origin",
        remoteUrl.str().c_str()
    );

    if (error < 0) {
        throwGitException();
    }

    // Create the directories
    auto path = mPath / "entries";

    if (!boost::filesystem::create_directory(path)) {
        throw Exception("Failed creating the directory '" + path.string() + "'");
    }

    filesystem::ofstream(path / ".gitkeep");

    return *this;
}

Repository &
Repository::clone()
{
    git_repository *repository;

    int error;

    auto remoteUrl = boost::format(GIT_USERNAME "@" GIT_ADDRESS ":%1%") % mId.toString();

    git_clone_options options = GIT_CLONE_OPTIONS_INIT;

    options.fetch_opts.callbacks.credentials = (git_cred_acquire_cb) &Repository::acquireCredentials;
    options.fetch_opts.callbacks.payload = (void *) &mPrivateKey;

    error = git_clone
    (
        &repository,
        remoteUrl.str().c_str(),
        mPath.string().c_str(),
        &options
    );

    if (error < 0) {
        throwGitException();
    }

    mGitRepository = std::shared_ptr<git_repository>(repository, [](git_repository *data) {
        git_repository_free(data);
    });

    return *this;
}

Repository &
Repository::open()
{
    git_repository *repository;
    git_remote *remote;

    int error;

    error = git_repository_open(&repository, mPath.c_str());

    if (error < 0) {
        throwGitException();
    }

    mGitRepository = std::shared_ptr<git_repository>(repository, [](git_repository *data) {
        git_repository_free(data);
    });

    error = git_remote_lookup(&remote, mGitRepository.get(), "origin");

    if (error < 0) {
        throwGitException();
    }

    mGitRemote = std::shared_ptr<git_remote>(remote, [](git_remote *data) {
        git_remote_free(data);
    });

    return *this;
}

Repository &
Repository::eachLogs(const std::string &range, std::function<void (const Buffer &id, const Commit &)> callback)
{
    // FIXME: improve this shit

    git_oid oid;
    git_revwalk *walker;
    git_commit *commit;

    int error;

    git_revwalk_new(&walker, mGitRepository.get());
    git_revwalk_sorting(walker, GIT_SORT_TIME);

    error = git_revwalk_push_range(walker, range.c_str());

    if (error < 0) {
        throwGitException();
    }

    while (!git_revwalk_next(&oid, walker)) {

        error = git_commit_lookup(&commit, mGitRepository.get(), &oid);

        if (error < 0) {
            throwGitException();
        }

        auto repositoryCommit = Commit::fromGitCommit(commit);
        callback(repositoryCommit.getHash(), repositoryCommit);

        git_commit_free(commit);
    }

    git_revwalk_free(walker);

    return *this;
}

Repository &
Repository::destroy()
{
    boost::filesystem::remove_all(mPath);

    return *this;
}

} // End of main namespace
