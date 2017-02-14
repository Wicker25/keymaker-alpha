/*!
 * Title ---- km/Core.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_CORE_HPP__
#define __KM_CORE_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/PrivateKey.hpp>
#include <km/AccessKey.hpp>
#include <km/Encrypter.hpp>
#include <km/KeyringNode.hpp>
#include <km/TextNode.hpp>
#include <km/Repository.hpp>
#include <km/SshClient.hpp>

#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <map>

#include <cstdlib>

#include <boost/any.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <spdlog/spdlog.h>

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The application.
 */
class Core
{

public:

    /*!
     * The remote shell user.
     */
    const Buffer ShellUser = Buffer::fromString("km");

    /*!
     * The remote shell hostname.
     */
    const Buffer ShellHostname = Buffer::fromString("keymaker.io");

    /*!
     * The remote shell address.
     */
    const Buffer ShellPort = Buffer::fromString("22");

	/*!
     * Returns a pointer to the singleton.
     */
	static Core &get();

    /*!
     * Destructor method.
     */
	virtual ~Core();

    /*!
     * Authenticate the user.
     *
     * @param[in] passphrase The private key passphrase.
     */
    Core &authenticate(const Buffer &passphrase);

    /*!
     * Creates a new repository.
     *
     * @param[in] name The repository name.
     */
    Core &createRepository(const Buffer &name);

    /*!
     * Opens a repository.
     *
     * @param[in] repositoryId The repository ID.
     */
    Core &openRepository(const Buffer &repositoryId);

    /*!
     * Updates the repository.
     *
     * @param[in] name The repository name.
     */
    Core &updateRepository(const Buffer &name);

    /*!
     * Shares the repository.
     *
     * @param[in] userId The user ID.
     */
    Core &shareRepository(const Buffer &userId);

    /*!
     * Sets an entry in the repository.
     *
     * @param[in] entry The entry.
     */
    Core &setRepositoryEntry(const EntryNode &entry);

    /*!
     * Returns an entry in the repository.
     *
     * @param[in] id The entry ID.
     *
     * @return The entry.
     */
    EntryNode getRepositoryEntry(const Buffer &id);

    /*!
     * Iterates over the repository entries.
     *
     * @param[in] callback The callback function.
     */
    Core &eachRepositoryEntry(std::function<void (const Buffer &id, EntryNode &)> callback);

    /*!
     * Commits changes in the repository.
     *
     * @param[in] message The commit message.
     */
    Core &commitRepository(const Buffer &message);

    /*!
     * Pushes the repository.
     */
    Core &pushRepository();

    /*!
     * Fetches changes of the repository.
     */
    Core &fetchRepository();

    /*!
     * Iterates over the repository logs.
     */
    Core &eachRepositoryLogs(const std::string &range, std::function<void (const Buffer &id, const Commit &)> callback);

    /*!
     * Destroys the repository.
     */
    Core &destroyRepository();

    /*!
     * Iterates over the repository entries.
     *
     * @param[in] callback The callback function.
     */
    Core &eachRepository(std::function<void (const Buffer &id, Repository &repository)> callback);

    /*!
     * Lists the repositories.
     */
    Core &listRepositories();

    /*!
     * Fetches the shared repositories.
     */
    Core &fetchRepositories();



    /*!
     * Returns the environment information.
     */
    std::map<const Buffer, boost::any> getEnvironment() const;

    /*!
     * Returns the environment information.
     */
    template <typename T>
    T getEnvironment(const Buffer &name);

    /*!
     * Returns the list of repositories.
     */
    const std::vector<Buffer> &getRepositoryList() const;

    /*!
     * Returns the list of entries in the repository.
     */
    const std::vector<Buffer> &getEntryList() const;

protected:

    /*!
     * Constructor method.
     */
	Core();

    /*!
     * Disables the copy constructor.
     */
	Core(const Core &other) = delete;

    /*!
     * Disables the copy constructor.
     */
	void operator=(const Core &other) = delete;

    /*!
     * Loads the application paths.
     */
    void loadPath();

    /*!
     * Loads the application configuration.
     */
	void loadConfig();

    /*!
     * FIXME
     */
    void setUpLogger();

    /*!
     * Loads the repositories.
     */
    void loadRepositories();

    /*!
     * Loads the repository entries.
     */
    void loadEntries();




    /*!
     * The repository list.
     */
    std::vector<Buffer> mRepositoryList;

    /*!
     * The repository entry list.
     */
    std::vector<Buffer> mEntryList;


    /*!
     * FIXME.
     */
    std::vector<Buffer> getRepositories();


    /*!
     * Creates a keyring node by using a public key.
     *
     * @param[in] publicKey The public key.
     */
    KeyringNode createKeyring(const PublicKey &publicKey) const;

    /*!
     * Creates an access key.
     *
     * @param[in] key       The access key data.
     * @param[in] publicKey The public key.
     */
    AccessKey createAccessKey(const Buffer &key, const PublicKey &publicKey) const;

    /*!
     * Builds a Git repository.
     *
     * @param[in] repositoryId The repository ID.
     */
    void buildGitRepository(const Buffer &repositoryId);

    /*!
     * Opens a Git repository.
     *
     * @param[in] repository The repository instance.
     */
    void openGitRepository(const Repository &repository);

    /*!
     * Shares a Git repository.
     *
     * @param[in] repository The repository instance.
     * @param[in] publicKey  The public key.
     */
    void shareGitRepository(Repository &repository, const PublicKey &publicKey) const;

    /*!
     * Parses the repository.
     */
    void parseRepository();


    Repository  &getRepository();
    SshClient   &getShell();
    KeyringNode &getKeyring();

    filesystem::path mHomePath;

    /*!
     * FIXME.
     */
    filesystem::path mConfigPath;

    /*!
     * FIXME.
     */
    filesystem::path mKeyringPath;

    /*!
     * The property tree of the application configuration.
     */
    property_tree::ptree mConfig;

    /*!
     * The environment settings.
     */
    std::map<const Buffer, boost::any> mEnvironment;

    /*!
     * The public key.
     */
    PublicKey mPublicKey;

    /*!
     * The private key.
     */
    PrivateKey mPrivateKey;

    /*!
     * FIXME
     */
    std::unique_ptr<SshClient> mShell;

    /*!
     * The current repository.
     */
    std::unique_ptr<Repository> mRepository;

    /*!
     * The crypter.
     */
    std::unique_ptr<Encrypter> mEncrypter;

    /*!
     * FIXME
     */
    std::unique_ptr<KeyringNode> mKeyringNode;
};

} // End of main namespace

#endif /* __KM_CORE_HPP__ */

// Include inline methods
#include <km/Core-inl.hpp>