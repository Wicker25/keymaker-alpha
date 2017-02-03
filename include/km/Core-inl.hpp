/*!
 * Title ---- km/Core-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_CORE_INL_HPP__
#define __KM_CORE_INL_HPP__

namespace km { // Begin main namespace

inline Core &
Core::get()
{
    static std::unique_ptr<Core> mInstance(new Core());
	return *(mInstance.get());
}

inline std::map<const Buffer, boost::any>
Core::getEnvironment() const
{
	return mEnvironment;
}

template <typename T>
inline T
Core::getEnvironment(const Buffer &name)
{
    return boost::any_cast<T>(mEnvironment[name]);
}

template <>
inline Buffer
Core::getEnvironment(const Buffer &name)
{
    return boost::any_cast<std::string>(mEnvironment[name]);
}

inline Repository &
Core::getRepository()
{
    return *mRepository;
}

inline KeyringNode &
Core::getKeyring()
{
    return *mKeyringNode;
}

inline const std::vector<Buffer> &
Core::getRepositoryList() const
{
    return mRepositoryList;
}

inline const std::vector<Buffer> &
Core::getEntryList() const
{
    return mEntryList;
}

} // End of main namespace

#endif /* __KM_CORE_INL_HPP__ */