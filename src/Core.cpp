/*!
 * Title ---- km/Core.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/Core.hpp>

namespace km { // Begin main namespace

Core::Core()
{
    OpenSSL_add_all_ciphers();

    libssh2_init(LIBSSH2_INIT_NO_CRYPTO);
    git_libgit2_init();

    loadPath();
    loadConfig();

    setUpLogger();

    loadRepositories();
}

Core::~Core()
{
    git_libgit2_shutdown();
    libssh2_exit();

    EVP_cleanup();
}

void
Core::loadPath()
{
    // Get the home path
    # if defined(BOOST_POSIX_API)
        mHomePath = std::getenv("HOME");

    # elif defined(BOOST_WINDOWS_API)
        filesystem::path homeDrive = std::getenv("HOMEDRIVE");
        filesystem::path homePath  = std::getenv("HOMEPATH");

        mHomePath = homeDrive / homePath;
    # else
    #   error "Unsupported platform"
    # endif

    // Get the path to the configuration file
    mConfigPath  = mHomePath / ".km/config";

    // Get the path to the keyrings
    mKeyringPath = mHomePath / ".km/keyrings";
}

void
Core::loadConfig()
{
    // Build the user directories
    auto directoryPaths =
    {
        mHomePath / ".km",
        mHomePath / ".km/logs",
        mHomePath / ".km/keyrings"
    };

    for (auto &path : directoryPaths) {
        filesystem::create_directories(path);
    }

    property_tree::ini_parser::read_ini(mConfigPath.string(), mConfig);

    // Load the owner data
    for (auto &node : mConfig.get_child("owner")) {
        mEnvironment[node.first] = node.second.get_value<std::string>();
    }

    // Load the security data
    for (auto &node : mConfig.get_child("security")) {
        mEnvironment[node.first] = node.second.get_value<std::string>();
    }
}

void
Core::setUpLogger()
{
    auto logPath = mHomePath / ".km/logs/km-log";

    spdlog::rotating_logger_mt("km", logPath.string(), 1048576 * 5, 5);
}

Core &
Core::authenticate(const Buffer &passphrase)
{
    filesystem::path keyPath = any_cast<std::string>(mEnvironment["private_key"]);

    mPrivateKey = PrivateKey::fromPath
    (
        filesystem::canonical(keyPath, mHomePath),
        passphrase
    );

    mPublicKey = PublicKey::fromPrivateKey(mPrivateKey);

    return *this;
}

Core &
Core::openRepository(const Buffer &repositoryId)
{
    auto repositoryPath = mKeyringPath / repositoryId.toString();

    openGitRepository(
        Repository::fromPath(repositoryId, repositoryPath, mPrivateKey)
    );

    parseRepository();

    return *this;
}

Core &
Core::createRepository(const Buffer &name)
{
    auto &shell = getShell();

    Buffer response = shell.exec("repository-create --name='%1%'", name);

    if (shell.getReturnCode() != 0) {
        throw Exception("Failure creating the repository");
    }

    std::istringstream ss((char *) response.data());

    property_tree::ptree responseTree;
    property_tree::read_json(ss, responseTree);

    auto status  = responseTree.get("status", 500);
    auto content = responseTree.get_child("content");

    if (status != 201) {
        throw Exception("Failed: status code " + responseTree.get<std::string>("status"));
    }

    auto repositoryId = Buffer::fromString(
        content.get<std::string>("id")
    );


    buildGitRepository(repositoryId, name);
    parseRepository();

    auto textNode = mEncrypter->encrypt<TextNode>(
        TextNode::create(Buffer::fromString("Initial commit"))
    );

    auto &repository = getRepository();

    repository
        .commit(textNode.toBuffer())
        .push()
    ;

    return *this;
}

Core &
Core::updateRepository(const Buffer &name)
{
    auto &repository = getRepository();
    auto &shell      = getShell();

    Buffer response = shell.exec("repository-update '%1%' --name='%2%'", repository.getId(), name);

    if (shell.getReturnCode() != 0) {
        throw Exception("Failure updating the repository");
    }

    return *this;
}

Core &
Core::shareRepository(const Buffer &userId)
{
    auto &repository = getRepository();
    auto &shell      = getShell();

    Buffer response;

    response = shell.exec("user-show '%1%'", userId);

    if (shell.getReturnCode() != 0) {
        throw Exception("Failure getting the user");
    }

    std::istringstream ss((char *) response.data());

    property_tree::ptree responseTree;
    property_tree::read_json(ss, responseTree);

    auto status  = responseTree.get("status", 500);
    auto content = responseTree.get_child("content");

    if (status != 200) {
        throw Exception("Failed: status code " + responseTree.get<std::string>("status"));
    }

    auto pkcsKey = Buffer::fromString(
        content.get<std::string>("pkcsKey")
    );

    PublicKey publicKey = PublicKey::fromMemory(pkcsKey);

    // TODO: allow other permission types
    response = shell.exec("repository-share '%1%' '%2%' --read", repository.getId(), userId);

    if (shell.getReturnCode() != 0) {
        throw Exception("Failure sharing the repository");
    }

    shareGitRepository(repository, publicKey);

    auto textNode = mEncrypter->encrypt<TextNode>(
        TextNode::create(Buffer::fromString("Shared repository"))
    );

    repository
        .commit(textNode.toBuffer())
        .push()
    ;

    return *this;
}

Core &
Core::setRepositoryEntry(const EntryNode &input)
{
    auto &repository = getRepository();
    auto &keyring    = getKeyring();

    auto output = mEncrypter->encrypt<EntryNode>(input);

    keyring
        .setEntry(output)
        .save(repository.getPath())
    ;

    loadEntries();

    return *this;
}

EntryNode
Core::getRepositoryEntry(const Buffer &id)
{
    return mEncrypter->decrypt<EntryNode>(
        mKeyringNode->getEntry(id)
    );
}

Core &
Core::eachRepositoryEntry(std::function<void (const Buffer &id, EntryNode &)> callback)
{
    auto &keyring = getKeyring();

    keyring.eachEntry([this, &callback](const Buffer &id, const EntryNode &input) {

        auto output = input.mapProperty([this](const Buffer &name, const PropertyNode &property) {
            return mEncrypter->decrypt<PropertyNode>(property);
        });

        callback(id, output);
    });

    return *this;
}

Core &
Core::commitRepository(const Buffer &message)
{
    auto &repository = getRepository();

    auto textNode = mEncrypter->encrypt<TextNode>(
        TextNode::create(message)
    );

    repository
        .add("entries/*")
        .update("entries/*")
        .commit(textNode.toBuffer())
    ;

    return *this;
}

Core &
Core::pushRepository()
{
    auto &repository = getRepository();
    repository.push();

    return *this;
}

Core &
Core::fetchRepository()
{
    auto &repository = getRepository();
    repository.fetch();

    return *this;
}

Core &
Core::eachRepositoryLogs(const std::string &range, std::function<void (const Buffer &id, const Commit &)> callback)
{
    mRepository->eachLogs(range, [this, &callback](const Buffer &id, const Commit &input) {

        auto textNode = mEncrypter->decrypt<TextNode>(
            TextNode::fromBuffer(input.getSummary())
        );

        Commit ouput = input;
        ouput.setSummary(textNode.getContent());

        callback(id, ouput);
    });

    return *this;
}

Core &
Core::destroyRepository()
{
    auto &repository = getRepository();
    auto &shell      = getShell();

    Buffer response = shell.exec("repository-delete '%1%'", repository.getId());

    if (shell.getReturnCode() != 0) {
        throw Exception("Failure deleting the repository");
    }

    repository.destroy();

    return *this;
}






void
Core::loadRepositories()
{
    mRepositoryList.clear();

    eachRepository([this](const Buffer &id, Repository &repository) {
        mRepositoryList.push_back(id);
    });
}

void
Core::loadEntries()
{
    mEntryList.clear();

    eachRepositoryEntry([this](const Buffer &id, EntryNode &entry) {
        mEntryList.push_back(id);
    });
}





Core &
Core::listRepositories()
{
    // TODO

    return *this;
}

Core &
Core::eachRepository(std::function<void (const Buffer &id, Repository &)> callback)
{
    if (!filesystem::is_directory(mKeyringPath)) {
        throw Exception("Unable to find the directory 'keyrings'");
    }

    filesystem::directory_iterator it(mKeyringPath), end;

    for ( ; it != end; ++it) {

        auto name = it->path().filename().string();

        if (name[0] == '.') {
            continue;
        }

        auto repositoryId   = Buffer::fromString(name);
        auto repositoryPath = mKeyringPath / repositoryId.toString();

        // FIXME: improve this shit!!!
        auto repository = Repository::fromPath(repositoryId, repositoryPath, mPrivateKey);

        callback(repositoryId, repository);
    }

    return *this;
}

Core &
Core::eachKeyring(std::function<void (const Buffer &id, KeyringNode &)> callback)
{
    if (!filesystem::is_directory(mKeyringPath)) {
        throw Exception("Unable to find the directory 'keyrings'");
    }

    filesystem::directory_iterator it(mKeyringPath), end;

    for ( ; it != end; ++it) {

        auto name = it->path().filename().string();

        if (name[0] == '.') {
            continue;
        }

        auto repositoryId   = Buffer::fromString(name);
        auto repositoryPath = mKeyringPath / repositoryId.toString();

        // FIXME: improve this shit!!!
        auto repository = Repository::fromPath(repositoryId, repositoryPath, mPrivateKey);

        auto keyringNode = std::make_unique<KeyringNode>(
            KeyringNode::fromPath(repository.getPath())
        );

        auto accessKey = keyringNode->getAccessKey(
            Encrypter::getFingerprint(mPrivateKey)
        );

        auto encrypter = std::make_unique<Encrypter>(
            Encrypter::create(accessKey, mPrivateKey)
        );

        auto aaa = encrypter->decrypt<KeyringNode>(
            *keyringNode
        );

        callback(aaa.getId(), aaa);
    }

    return *this;
}





Core &
Core::fetchRepositories()
{
    auto repositories = getRepositories();

    for (auto &repositoryId : repositories) {

        auto repositoryPath = mKeyringPath / repositoryId.toString();

        if (Repository::isValid(repositoryPath)) {

            auto repository = Repository::fromPath(repositoryId, repositoryPath, mPrivateKey);
            repository.fetch();

        } else {

            Repository repository = Repository::fromRemote(repositoryId, repositoryPath, mPrivateKey);
        }
    }

    return *this;
}




std::vector<Buffer>
Core::getRepositories()
{
    auto &shell = getShell();

    Buffer response = shell.exec("repository-list");

    if (shell.getReturnCode() != 0) {
        throw Exception("Failure listing the repository");
    }

    std::istringstream ss((char *) response.data());

    property_tree::ptree responseTree;
    property_tree::read_json(ss, responseTree);

    auto status  = responseTree.get("status", 500);
    auto content = responseTree.get_child("content");

    if (status != 200) {
        throw Exception("Failed: status code " + responseTree.get<std::string>("status"));
    }

    // FIXME
    std::vector<Buffer> list;

    for (auto &repositoryNode : content) {

        auto repositoryId = (repositoryNode.second).get<std::string>("id");
        list.push_back(Buffer::fromString(repositoryId));
    }

    return list;
}




KeyringNode
Core::createKeyring(const Buffer &name, const PublicKey &publicKey) const
{
    auto accessKey = createAccessKey
    (
        Buffer::fromRandom(32), // 256 bit
        publicKey
    );

    // FIXME: check this
    auto keyring = KeyringNode::create();
    keyring.addAccessKey(accessKey);

    return keyring;
}

AccessKey
Core::createAccessKey(const Buffer &key, const PublicKey &publicKey) const
{
    auto recipient = Encrypter::getFingerprint(publicKey);

    return Encrypter::encrypt<AccessKey>
    (
        AccessKey::create(recipient, key),
        publicKey
    );
}

void
Core::buildGitRepository(const Buffer &repositoryId, const Buffer &name)
{
    auto repositoryPath = mKeyringPath / repositoryId.toString();

    auto repository = Repository::create(repositoryId, repositoryPath, mPrivateKey);

    openGitRepository(repository);

    // FIXME FIXME FIXME
    auto aaa = createKeyring(name, mPublicKey);

    // FIXME
    aaa.setProperty("name", name);

    auto accessKey = aaa.getAccessKey(
        Encrypter::getFingerprint(mPrivateKey)
    );

    mEncrypter = std::make_unique<Encrypter>(
        Encrypter::create(accessKey, mPrivateKey)
    );

    auto keyringNode = mEncrypter->encrypt<KeyringNode>(
        aaa
    );

    keyringNode.save(repository.getPath());

    repository
        .add("keymaker.xml")
        .add("entries/*")
    ;
}

void
Core::openGitRepository(const Repository &repository)
{
    mRepository = std::make_unique<Repository>(repository);
}

void
Core::shareGitRepository(Repository &repository, const PublicKey &publicKey) const
{
    KeyringNode keyring = KeyringNode::fromPath(repository.getPath());

    auto fingerprint = Encrypter::getFingerprint(mPublicKey);

    // Get the current access key
    auto accessKey = Encrypter::decrypt<AccessKey>
    (
        keyring.getAccessKey(fingerprint),
        mPrivateKey
    );

    // Generate a new access key
    auto newAccessKey = createAccessKey
    (
        accessKey.getData(),
        publicKey
    );

    keyring
        .addAccessKey(newAccessKey)
        .save(repository.getPath())
    ;

    repository
        .update("keymaker.xml")
    ;
}

void
Core::parseRepository()
{
    mKeyringNode = std::make_unique<KeyringNode>(
        KeyringNode::fromPath(mRepository->getPath())
    );

    auto accessKey = mKeyringNode->getAccessKey(
        Encrypter::getFingerprint(mPrivateKey)
    );

    mEncrypter = std::make_unique<Encrypter>(
        Encrypter::create(accessKey, mPrivateKey)
    );

    loadEntries();
}

SshClient &
Core::getShell()
{
    if (!mShell) {
        mShell = std::make_unique<SshClient>(ShellHostname, ShellPort);
        mShell->connect(ShellUser, mPrivateKey);
    }

    return *(mShell.get());
}

} // End of main namespace