/*!
 * Title ---- km/EntryNode-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_ENTRY_NODE_INL_HPP__
#define __KM_ENTRY_NODE_INL_HPP__

namespace km { // Begin main namespace

inline const Buffer &
EntryNode::getId() const
{
    return mId;
}

inline EntryNode &
EntryNode::setProperty(const Buffer &name, const PropertyNode &property)
{
    mProperties.insert_or_assign(name, property);

    return *this;
}

inline const PropertyNode &
EntryNode::getProperty(const Buffer &name) const
{
    auto it  = mProperties.find(name),
         end = mProperties.end();

    if (it == end) {
        throw PropertyNotFoundException("Property '%1%' does not exist", name);
    }

    return it->second;
}

inline EntryNode &
EntryNode::removeProperty(const Buffer &name)
{
    mProperties.erase(name);

    return *this;
}

} // End of main namespace

#endif /* __KM_ENTRY_NODE_INL_HPP__ */
