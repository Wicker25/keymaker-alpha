/*!
 * Title ---- km/Exception.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_EXCEPTION_HPP__
#define __KM_EXCEPTION_HPP__

#include <string>
#include <stdexcept>

#include <boost/format.hpp>

namespace km { // Begin main namespace

/*!
 * The exception.
 */
class Exception : public std::runtime_error
{

public:

    /*!
     * Constructor method.
     *
     * @param[in] message The exception message.
     */
    Exception(const std::string &message) : std::runtime_error(message) { }

    /*!
     * Constructor method.
     *
     * @param[in] message The exception message.
     * @param[in] arg0    The first argument.
     */
    template <typename T_ARG0>
    Exception(const std::string &message, T_ARG0 arg0) : std::runtime_error(
        boost::str(boost::format(message) % arg0)
    ) { }

    /*!
     * Constructor method.
     *
     * @param[in] message The exception message.
     * @param[in] arg0    The first argument.
     * @param[in] arg1    The second argument.
     */
    template <typename T_ARG0, typename T_ARG1>
    Exception(const std::string &message, T_ARG0 arg0, T_ARG1 arg1) : std::runtime_error(
        boost::str(boost::format(message) % arg0 % arg1)
    ) { }

    /*!
     * Constructor method.
     *
     * @param[in] message The exception message.
     * @param[in] arg0    The first argument.
     * @param[in] arg1    The second argument.
     * @param[in] arg2    The third argument.
     */
    template <typename T_ARG0, typename T_ARG1, typename T_ARG2>
    Exception(const std::string &message, T_ARG0 arg0, T_ARG1 arg1, T_ARG2 arg2) : std::runtime_error(
        boost::str(boost::format(message) % arg0 % arg1 % arg2)
    ) { }

    /*!
     * Constructor method.
     *
     * @param[in] message The exception message.
     * @param[in] arg0    The first argument.
     * @param[in] arg1    The second argument.
     * @param[in] arg2    The third argument.
     * @param[in] arg3    The fourth argument.
     */
    template <typename T_ARG0, typename T_ARG1, typename T_ARG2, typename T_ARG3>
    Exception(const std::string &message, T_ARG0 arg0, T_ARG1 arg1, T_ARG2 arg2, T_ARG3 arg3) : std::runtime_error(
        boost::str(boost::format(message) % arg0 % arg1 % arg2 % arg3)
    ) { }
};

} // End of main namespace

#endif /* __KM_EXCEPTION_HPP__ */
