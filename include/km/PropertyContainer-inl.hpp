/*!
 * Title ---- km/PropertyContainer-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_PROPERTY_CONTAINER_INL_HPP__
#define __KM_PROPERTY_CONTAINER_INL_HPP__

namespace km { // Begin main namespace

template <typename T>
PropertyContainer<T>
PropertyContainer<T>::fromConfig(PropertyContainer<T> &container, property_tree::ptree &config)
{
    for (auto &node : config.get_child("properties")) {

        if (node.first == "<xmlattr>") {
            continue;
        }

        PropertyNode property = PropertyNode::fromConfig(node.second);
        container.setProperty(property.getName(), property);
    }

    return container;
}

template <typename T>
property_tree::ptree
PropertyContainer<T>::toConfig(const PropertyContainer &container)
{
    property_tree::ptree config;

    container.eachProperty([&config](const Buffer &key, const PropertyNode &property) {
        config.add_child("properties.property", PropertyNode::toConfig(property));
    });

    return config;
}

template <typename T>
PropertyContainer<T>::PropertyContainer()
{

}

template <typename T>
PropertyContainer<T>::~PropertyContainer()
{

}

template <typename T>
T &
PropertyContainer<T>::setProperty(const Buffer &name, const Buffer &value)
{
    auto property = PropertyNode::create(name, value);
    setProperty(name, property);

    return dynamic_cast<T &>(*this);
}

template <typename T>
T &
PropertyContainer<T>::eachProperty(std::function<void (const Buffer &, PropertyNode &)> callback)
{
    for (auto &node : mProperties) {
        callback(node.first, node.second);
    }

    return dynamic_cast<T &>(*this);
}

template <typename T>
const T &
PropertyContainer<T>::eachProperty(std::function<void (const Buffer &, const PropertyNode &)> callback) const
{
    for (auto &node : mProperties) {
        callback(node.first, node.second);
    }

    return dynamic_cast<const T &>(*this);
}

template <typename T>
T
PropertyContainer<T>::mapProperty(std::function<PropertyNode (const Buffer &, const PropertyNode &)> callback) const
{
    auto output = T::create(
        dynamic_cast<const T &>(*this)
    );

    for (auto &node : mProperties) {

        auto property = callback(node.first, node.second);

        output
            .removeProperty(node.first)
            .setProperty(property.getName(), property);
    }

    return output;
}

template <typename T>
inline T &
PropertyContainer<T>::setProperty(const Buffer &name, const PropertyNode &property)
{
    mProperties.insert_or_assign(name, property);

    return dynamic_cast<T &>(*this);
}

template <typename T>
inline const PropertyNode &
PropertyContainer<T>::getProperty(const Buffer &name) const
{
    auto it  = mProperties.find(name),
         end = mProperties.end();

    if (it == end) {
        throw PropertyNotFoundException("Property '%1%' does not exist", name);
    }

    return it->second;
}

template <typename T>
inline T &
PropertyContainer<T>::removeProperty(const Buffer &name)
{
    mProperties.erase(name);

    return dynamic_cast<T &>(*this);
}

} // End of main namespace

#endif /* __KM_PROPERTY_CONTAINER_INL_HPP__ */
