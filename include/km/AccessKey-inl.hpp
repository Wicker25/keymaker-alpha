/*!
 * Title ---- km/AccessKey-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_ACCESS_KEY_INL_HPP__
#define __KM_ACCESS_KEY_INL_HPP__

namespace km { // Begin main namespace

inline AccessKey &
AccessKey::setRecipient(const Buffer &recipient)
{
    mRecipient = recipient;
    return *this;
}

inline const Buffer &
AccessKey::getRecipient() const
{
    return mRecipient;
}

inline AccessKey &
AccessKey::setData(const Buffer &data)
{
    mData = data;
    return *this;
}

inline const Buffer &
AccessKey::getData() const
{
    return mData;
}

} // End of main namespace

#endif /* __KM_ACCESS_KEY_INL_HPP__ */
