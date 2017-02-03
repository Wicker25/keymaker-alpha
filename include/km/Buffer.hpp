/*!
 * Title ---- km/Buffer.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_BUFFER_HPP__
#define __KM_BUFFER_HPP__

#include <km.hpp>
#include <km/Exception.hpp>

#include <ostream>
#include <iterator>
#include <algorithm>

#include <string>
#include <vector>
#include <memory>
#include <sstream>

#include <cstdint>
#include <cstring>

#include <boost/algorithm/hex.hpp>

#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <openssl/rand.h>

# if defined(BOOST_WINDOWS_API)
#   include <windows.h>
#   include <winbase.h>
# endif

using namespace boost;
using namespace boost::archive::iterators;

namespace km { // Begin main namespace

/*!
 * The memory-safe allocator.
 */
template <class T>
class SafeAllocator : public std::allocator<T>
{

public:

    /*!
     * Imports the parent constructors.
     */
    using std::allocator<T>::allocator;

    /*!
     * Structure used to obtain an allocator for a different type.
     */
    template <class U>
    struct rebind { typedef SafeAllocator<U> other; };

    /*!
     * Deallocates the storage
     */
    void deallocate(void *p, std::size_t n);
};


/*!
 * The memory-safe string.
 */
typedef
    std::basic_string<
        char, std::char_traits<char>, SafeAllocator<char>
    > SafeString
;


/*!
 * The memory-safe string stream.
 */
typedef
    std::basic_stringstream<
        char, std::char_traits<char>, SafeAllocator<char>
    > SafeStringStream
;


/*!
 * The data container for the buffer.
 */
typedef
    std::vector<
        std::uint8_t, SafeAllocator<std::uint8_t>
    > Memory
;


/*!
 * The memory-safe buffer.
 */
class Buffer : public Memory
{

public:

    /*!
     * Imports methods from Memory.
     */
    using Memory::Memory;

    /*!
     * The string iterator.
     */
    typedef std::string::const_iterator string_iterator;

    /*!
     * Base64 encoder.
     */
    typedef
        base64_from_binary<
            transform_width<const_iterator, 6, 8>
        > base64_encoder
    ;


    /*!
     * Builds a new buffer from an array.
     *
     * @param[in] ptr    The array pointer.
     * @param[in] length The array length.
     *
     * @return The buffer.
     */
    static Buffer fromArray(std::uint8_t *ptr, size_type length);

    /*!
     * Converts a buffer into an array.
     *
     * @param[in] first The initial position.
     * @param[in] last  The final position.
     *
     * @return The array.
     */
    static const std::uint8_t *toArray(const_iterator first, const_iterator last);

    /*!
     * Builds a new buffer from a string.
     *
     * @param[in] data The string buffer.
     *
     * @return The buffer.
     */
    static Buffer fromString(const std::string &data);

    /*!
     * Converts a buffer into a string.
     *
     * @param[in] buffer The buffer.
     *
     * @return The string.
     */
    static std::string toString(const Buffer &buffer);

    /*!
     * Builds a new buffer from a string.
     *
     * @param[in] first The initial position.
     * @param[in] last  The final position.
     *
     * @return The buffer.
     */
    template <class InputIterator>
    static Buffer fromString(InputIterator first, InputIterator last);

    /*!
     * Converts a buffer into a string.
     *
     * @param[in] first The initial position.
     * @param[in] last  The final position.
     *
     * @return The string.
     */
    static std::string toString(const_iterator first, const_iterator last);

    /*!
     * Builds a new buffer from a hexadecimal string.
     *
     * @param[in] data The hexadecimal string.
     *
     * @return The buffer.
     */
    template <class T>
    static Buffer fromHex(const T &data);

    /*!
     * Converts a buffer into a hexadecimal string.
     *
     * @param[in] data The data buffer.
     *
     * @return The hexadecimal string.
     */
    template <class T>
    static T toHex(const T &data);

    /*!
     * Builds a new buffer from a hexadecimal string.
     *
     * @param[in] first The initial position.
     * @param[in] last  The final position.
     *
     * @return The buffer
     */
    template <class InputIterator>
    static Buffer fromHex(InputIterator first, InputIterator last);

    /*!
     * Converts a buffer into a hexadecimal string.
     *
     * @param[in] first The initial position in the buffer.
     * @param[in] last  The final position in the buffer.
     *
     * @return The hexadecimal string.
     */
    template <class T>
    static T toHex(const_iterator first, const_iterator last);

    /*!
     * Builds a new buffer from a base64 string.
     *
     * @param[in] data The base64 string.
     *
     * @return The buffer.
     */
    template <class T>
    static Buffer fromBase64(const T &data);

    /*!
     * Converts a buffer into a base64 string.
     *
     * @param[in] data The data buffer.
     *
     * @return The base64 string.
     */
    template <class T>
    static T toBase64(const T &data);

    /*!
     * Builds a new buffer from a base64 string.
     *
     * @param[in] first The initial position.
     * @param[in] last  The final position.
     *
     * @return The buffer.
     */
    template <class InputIterator>
    static Buffer fromBase64(InputIterator first, InputIterator last);

    /*!
     * Converts a buffer into a base64 string.
     *
     * @param[in] first The initial position.
     * @param[in] last  The final position.
     *
     * @return The base64 string.
     */
    template <class T>
    static T toBase64(const_iterator first, const_iterator last);

    /*!
     * Builds a new buffer from a random sequence.
     *
     * @param[in] size The size of the new buffer.
     *
     * @return The buffer.
     */
    static Buffer fromRandom(size_type size);


    /*!
     * Constructor method.
     */
    Buffer();

    /*!
     * Constructor method.
     *
     * @param[in] data The string data.
     */
    Buffer(const char *data);

    /*!
     * Constructor method.
     *
     * @param[in] data The string data.
     */
    Buffer(const std::string &data);

    /*!
     * Converts the buffer into a hexadecimal string.
     *
     * @return The hexadecimal string.
     */
    template <class T>
    T toHex() const;

    /*!
     * Converts the buffer into a base64 string.
     *
     * @return The base64 string.
     */
    template <class T>
    T toBase64() const;

    /*!
     * Converts the buffer into a string.
     *
     * @return The string.
     */
    std::string toString() const;

    /*!
     * Converts the buffer into an array.
     *
     * @return The array.
     */
    const std::uint8_t *toArray() const;

    /*!
     * Operator overloads.
     */
    Buffer &operator=(const char *data);
    Buffer &operator=(const std::string &data);

    /*!
     * Writes the buffer data into an output stream.
     *
     * @param[in] out    The output stream.
     * @param[in] buffer The buffer.
     */
    friend std::ostream &operator<<(std::ostream &out, const Buffer &buffer);

protected:

    /*!
     * The Base64 suffixes.
     */
    static const std::string mBase64Suffix[3];
};


/*!
 * Operator overloads.
 */
bool operator==(const Buffer &first, const char *second);
bool operator==(const Buffer &first, const std::string &second);
bool operator==(const char *first, const Buffer &second);
bool operator==(const std::string &first, const Buffer &second);

bool operator!=(const Buffer &first, const char *second);
bool operator!=(const Buffer &first, const std::string &second);
bool operator!=(const char *first, const Buffer &second);
bool operator!=(const std::string &first, const Buffer &second);


/*!
 * Formats a buffer.
 */
template <typename ... T_ARGS>
Buffer format_buffer(const Buffer &fmt, T_ARGS && ... args);

template <typename T_ARG0, typename ... T_ARGS>
Buffer format_buffer(boost::format &fmt, T_ARG0 &&arg0, T_ARGS && ... args);

Buffer format_buffer(boost::format &fmt);

} // End of main namespace

#endif /* __KM_BUFFER_HPP__ */

// Include inline methods
#include <km/Buffer-inl.hpp>