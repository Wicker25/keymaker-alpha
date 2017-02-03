/*!
 * Title ---- km/Repository-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_REPOSITORY_INL_HPP__
#define __KM_REPOSITORY_INL_HPP__

namespace km { // Begin main namespace

inline Buffer
Repository::getId() const
{
    return mId;
}

inline filesystem::path
Repository::getPath() const
{
    return mPath;
}

inline Repository &
Repository::access(const PrivateKey &privateKey)
{
    mPrivateKey = privateKey;
    return *this;
}

} // End of main namespace

#endif /* __KM_REPOSITORY_INL_HPP__ */
