/*!
 * Title ---- km/Console.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_CONSOLE_HPP__
#define __KM_CONSOLE_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>

#include <iostream>
#include <iomanip>
#include <sstream>

#include <chrono>
#include <thread>

#include <boost/system/api_config.hpp>

# if defined(BOOST_POSIX_API)
#   include <unistd.h>
#   include <termios.h>
# elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
# endif

#include <termcolor/termcolor.hpp>

#include <spdlog/spdlog.h>


#define BEGIN_TASK(AGENT, MESSAGE) \
{ \
    std::stringstream ss; \
    ss << MESSAGE; \
\
    ::km::TaskTracker __tracker(AGENT, ss.str()); \
\
    try {

#define END_TASK() \
        __tracker.done(); \
    } \
    catch (::km::TaskException e) { __tracker.fail(); } \
    catch (::km::Exception e)     { __tracker.fail(); throw e; } \
}


using namespace boost;

namespace km { // Begin main namespace

/*!
 * Prompts for a secret.
 *
 * @return The input data.
 */
Buffer promptSecret();

/*!
 * Prompts for a secret.
 *
 * @param[in] message The prompt message.
 *
 * @return The input data.
 */
Buffer promptSecret(const Buffer &message);


/*!
 * The console.
 */
class Console
{

public:

    /*!
     * Sets the debug state.
     */
    static void setDebug(bool state);

    /*!
     * Returns the debug state.
     */
    static bool getDebug();

protected:

    /*!
     * The name of the agent that initiated the task.
     */
    static bool mDebug;
};


/*!
 * The task tracker.
 */
class TaskTracker
{

public:

    /*!
     * Constructor method.
     *
     * @param[in] agent       The agent name.
     * @param[in] description The task description.
     */
    TaskTracker(const std::string &agent, const std::string &description);

    /*!
     * Destructor method.
     */
    virtual ~TaskTracker();

    /*!
     * Marks the task as done.
     */
    void done();

    /*!
     * Marks the task as failed.
     */
    void fail();

protected:

    /*!
     * The name of the agent that initiated the task.
     */
    std::string mAgent;

    /*!
     * The task description.
     */
    std::string mDescription;

    /*!
     * Indicates whether or not the task is processed.
     */
    bool mProcessed;
};


/*!
 * The task exception.
 */
class TaskException : public Exception
{
    using Exception::Exception;
};

} // End of main namespace

#endif /* __KM_CONSOLE_HPP__ */

// Include inline methods
#include <km/Console-inl.hpp>