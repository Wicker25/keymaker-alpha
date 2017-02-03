/*!
 * Title ---- km/Console.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/Console.hpp>

namespace km { // Begin main namespace

Buffer
promptSecret()
{
    Buffer input;

    // Turn off terminal echo
# if defined(BOOST_POSIX_API)
    termios mode;
    tcgetattr(STDIN_FILENO, &mode);

    termios newMode = mode;
    newMode.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newMode);

# elif defined(BOOST_WINDOWS_API)
    HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(console, &mode);
        SetConsoleMode(console, mode & (~ENABLE_ECHO_INPUT));
# else
#   error "Unsupported platform"
# endif

    // Read the input line
    int c;

    while ((c = std::cin.get()) && c != '\n' && c != '\r') {
        input.push_back((std::uint8_t) c);
    }

    // Turn on terminal echo
# if defined(BOOST_POSIX_API)
    tcsetattr(STDIN_FILENO, TCSANOW, &mode);

# elif defined(BOOST_WINDOWS_API)
    SetConsoleMode(console, mode);
# else
#   error "Unsupported platform"
# endif

    std::cout << std::endl;

    return input;
}


bool Console::mDebug = true;

TaskTracker::TaskTracker(const std::string &agent, const std::string &description)
:
    mAgent(agent),
    mDescription(description),
    mProcessed(false)
{
    spdlog::get("km")->info(mDescription);

    if (!Console::getDebug()) {
        return;
    }

    std::cout
        << "\r"
        << "km~[ "
        << termcolor::on_blue
        << std::left
        << std::setw(40)
        << mDescription
        << termcolor::reset
        << " ] "
        << std::flush
    ;
}

TaskTracker::~TaskTracker()
{
    if (!mProcessed) {
        done();
    }

    std::cout
        << "\r"
        << " "
        << std::left
        << std::setw(40 + 4 + 3)
        << ""
        << "\r"
        << std::flush
    ;
}

void
TaskTracker::done()
{
    mProcessed = true;

    if (!Console::getDebug()) {
        return;
    }

    std::cout
        << "\r"
        << "km~[ "
        << termcolor::on_green
        << std::left
        << std::setw(40)
        << mDescription
        << termcolor::reset
        << " ] "
        << std::flush
    ;
}

void
TaskTracker::fail()
{
    mProcessed = true;

    spdlog::get("km")->error(mDescription);

    if (!Console::getDebug()) {
        return;
    }

    std::cout
        << "\r"
        << "km~[ "
        << termcolor::on_red
        << std::left
        << std::setw(40)
        << mDescription
        << termcolor::reset
        << " ] "
        << std::flush
    ;

    std::this_thread::sleep_for(
        std::chrono::milliseconds(500)
    );
}

} // End of main namespace