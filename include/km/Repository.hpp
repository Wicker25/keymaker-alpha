/*!
 * Title ---- km/Repository.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_REPOSITORY_HPP__
#define __KM_REPOSITORY_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Console.hpp>
#include <km/Buffer.hpp>
#include <km/PrivateKey.hpp>
#include <km/Commit.hpp>

#include <memory>
#include <functional>
#include <algorithm>
#include <vector>
#include <map>
#include <fstream>

#include <git2.h>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#define GIT_ADDRESS  "keymaker.io"
#define GIT_USERNAME "km"

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The repository.
 */
class Repository
{

public:

    /*!
     * Creates a new repository.
     *
     * @param[in] id         The repository ID.
     * @param[in] path       The repository path.
     * @param[in] privateKey The private key.
     *
     * @return The repository.
     */
    static Repository create(const Buffer &id, const filesystem::path &path, const PrivateKey &privateKey);

    /*!
     * Opens a repository from local.
     *
     * @param[in] id         The repository ID.
     * @param[in] path       The repository path.
     * @param[in] privateKey The private key.
     *
     * @return The repository.
     */
    static Repository fromPath(const Buffer &id, const filesystem::path &path, const PrivateKey &privateKey);

    /*!
     * Clones a repository from remote.
     *
     * @param[in] id         The repository ID.
     * @param[in] path       The repository path.
     * @param[in] privateKey The private key.
     *
     * @return The repository.
     */
    static Repository fromRemote(const Buffer &id, const filesystem::path &path, const PrivateKey &privateKey);

    /*!
     * Checks if a repository exists.
     *
     * @param[in] path The repository path.
     *
     * @return True or false.
     */
    static bool isValid(const filesystem::path &path);

    /*!
     * Destructor method.
     */
	virtual ~Repository();

    /*!
     * Returns the repository ID.
     */
    Buffer getId() const;

    /*!
     * Returns the repository path.
     */
    filesystem::path getPath() const;

    /*!
     * Adds a file to the index.
     *
     * @param[in] path The file path.
     */
    Repository &add(const filesystem::path &path);

    /*!
     * Updates a file in the index.
     *
     * @param[in] path The file path.
     */
    Repository &update(const filesystem::path &path);

    /*!
     * Removes a file from the index.
     *
     * @param[in] path The file path.
     */
    Repository &remove(const filesystem::path &path);

    /*!
     * Commits an entry in the current repository.
     *
     * @param[in] message The commit message.
     */
    Repository &commit(const Buffer &message);

    /*!
     * Fetches the changes.
     */
    Repository &fetch();

    /*!
     * Pushes the changes.
     */
    Repository &push();

    /*!
     * FIXME.
     */
    Repository &eachCommits(const std::string &range, std::function<void (const Buffer &, const Commit &)> callback);

    /*!
     * Destroys the repository.
     */
    Repository &destroy();

protected:

    /*!
     * The acquire credentials callback.
     */
    static int acquireCredentials(git_cred **out, const char *, const char *, unsigned int, void *privateKey);

    /*!
     * Throws a Git exception.
     */
    static void throwGitException(); // TODO: see BOOST_PROPERTY_TREE_THROW

    /*!
     * Constructor method.
     *
     * @param[in] id   The repository ID.
     * @param[in] path The repository path.
     */
    explicit Repository(const Buffer &id, const filesystem::path &path);

    /*!
     * Initializes the repository.
     */
    Repository &init();

    /*!
     * Access the repository.
     *
     * @param[in] privateKey The private key.
     *
     * @return The repository instance.
     */
    Repository &access(const PrivateKey &privateKey);

    /*!
     * Clones the repository.
     */
    Repository &clone();

    /*!
     * Opens the repository.
     */
    Repository &open();


    /*!
     * The repository ID.
     */
    Buffer mId;

    /*!
     * The repository local path.
     */
    filesystem::path mPath;

    /*!
     * The GIT repository instance.
     */
    std::shared_ptr<git_repository> mGitRepository;

    /*!
     * The GIT remote instance.
     */
    std::shared_ptr<git_remote> mGitRemote;

    /*!
     * The private key.
     */
    PrivateKey mPrivateKey;
};



/*!
 * The task exception. FIXME
 */
class BlaException : public Exception
{

public:

    /*!
     * Constructor method.
     *
     * @param[in] message The exception message.
     */
    explicit BlaException(const std::string &message) : Exception(message) { }
};

} // End of main namespace

#endif /* __KM_REPOSITORY_HPP__ */

// Include inline methods
#include <km/Repository-inl.hpp>