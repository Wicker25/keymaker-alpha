/*!
 * Title ---- km/PropertyContainer.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_PROPERTY_CONTAINER_HPP__
#define __KM_PROPERTY_CONTAINER_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>
#include <km/PropertyNode.hpp>

#include <algorithm>
#include <map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace boost;

namespace km { // Begin main namespace

/*!
 * The property map.
 */
typedef std::map<Buffer, PropertyNode> PropertyMap;


/*!
 * The property container.
 */
template <typename T>
class PropertyContainer
{

public:

    /*!
     * Creates a property container from a property tree.
     *
     * @param[in] container The property container.
     * @param[in] config    The property tree.
     *
     * @return The property container.
     */
    static PropertyContainer fromConfig(PropertyContainer<T> &container, property_tree::ptree &config);

    /*!
     * Converts a property container into a property tree.
     *
     * @param[in] container The property container.
     *
     * @return The configuration node.
     */
    static property_tree::ptree toConfig(const PropertyContainer &container);

    /*!
     * Destructor method.
     */
	virtual ~PropertyContainer();

    /*!
     * Sets a property in the container.
     *
     * @param[in] name     The property name.
     * @param[in] property The property instance.
     *
     * @return The entry node.
     */
    T &setProperty(const Buffer &name, const PropertyNode &property);

    /*!
     * Sets a property in the container.
     *
     * @param[in] name  The property name.
     * @param[in] value The property value.
     *
     * @return The entry node.
     */
    T &setProperty(const Buffer &name, const Buffer &value);

    /*!
     * Returns a property in the container.
     *
     * @param[in] name The property name.
     *
     * @return The property node.
     */
    const PropertyNode &getProperty(const Buffer &name) const;

    /*!
     * Removes a property from the container.
     *
     * @param[in] name The property name.
     *
     * @return The property container.
     */
    T &removeProperty(const Buffer &name);

    /*!
     * Iterates over the properties in the container.
     *
     * @param[in] callback The callback function.
     *
     * @return The property container.
     */
    T &eachProperty(std::function<void (const Buffer &, PropertyNode &)> callback);

    /*!
     * Iterates over the properties in the container.
     *
     * @param[in] callback The callback function.
     *
     * @return The property container.
     */
    const T &eachProperty(std::function<void (const Buffer &, const PropertyNode &)> callback) const;

    /*!
     * Iterates over the properties in the container and produce a new property container.
     *
     * @param[in] callback The callback function.
     *
     * @return The property container.
     */
    T mapProperty(std::function<PropertyNode (const Buffer &, const PropertyNode &)> callback) const;

protected:

    /*!
     * Constructor method.
     */
    PropertyContainer();


    /*!
     * The properties.
     */
    PropertyMap mProperties;
};


/*!
 * The property not found exception.
 */
class PropertyNotFoundException : public Exception
{
    using Exception::Exception;
};

} // End of main namespace

#endif /* __KM_PROPERTY_CONTAINER_HPP__ */

// Include inline methods
#include <km/PropertyContainer-inl.hpp>