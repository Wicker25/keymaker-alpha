/*!
 * Title ---- km/Commit-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_COMMIT_INL_HPP__
#define __KM_COMMIT_INL_HPP__

namespace km { // Begin main namespace

inline Commit &
Commit::setSummary(const Buffer &summary)
{
    mSummary = summary;
    return *this;
}

inline const Buffer &
Commit::getSummary() const
{
    return mSummary;
}

inline const Buffer &
Commit::getMessage() const
{
    return mMessage;
}

inline const Buffer &
Commit::getAuthorName() const
{
    return mAuthorName;
}

inline const Buffer &
Commit::getAuthorEmail() const
{
    return mAuthorEmail;
}

inline const std::tm *
Commit::getTime() const
{
    return mTime;
}

} // End of main namespace

#endif /* __KM_COMMIT_INL_HPP__ */
