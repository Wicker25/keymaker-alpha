/*!
 * Title ---- km/Buffer-inl.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_BUFFER_INL_HPP__
#define __KM_BUFFER_INL_HPP__

namespace km { // Begin main namespace

template <class T>
void
SafeAllocator<T>::deallocate(void *p, std::size_t n)
{
    // NOTE: clears the buffer

    # if defined(BOOST_WINDOWS_API)
        SecureZeroMemory((T *) p, n);
    # else
        std::fill_n((volatile T *) p, n, (T) 0);
    # endif

    std::allocator<T>::deallocate((T *) p, n);
}

inline Buffer
Buffer::fromArray(std::uint8_t *ptr, size_type length)
{
    return Buffer(ptr, ptr + length);
}

inline const std::uint8_t *
Buffer::toArray(const_iterator first, const_iterator last)
{
    return &(*first);
}

inline Buffer
Buffer::fromString(const std::string &data)
{
    return Buffer::fromString(data.begin(), data.end());
}

inline std::string
Buffer::toString(const Buffer &buffer)
{
    return Buffer::toString(buffer.begin(), buffer.end());
}

template <class InputIterator>
inline Buffer
Buffer::fromString(InputIterator first, InputIterator last)
{
    Buffer buffer(first, last);
    return buffer;
}

inline std::string
Buffer::toString(const_iterator first, const_iterator last)
{
    return std::string(first, last);
}

template <class T>
inline Buffer
Buffer::fromHex(const T &data)
{
    return Buffer::fromHex(std::begin(data), std::end(data));
}

template <class T>
inline T
Buffer::toHex(const T &data)
{
    return Buffer::toHex<T>(std::begin(data), std::end(data));
}

template <class InputIterator>
Buffer
Buffer::fromHex(InputIterator first, InputIterator last)
{
    Buffer output;
    algorithm::unhex(first, last, std::back_inserter(output));
    return output;
}

template <class T>
inline Buffer
Buffer::fromBase64(const T &data)
{
    return Buffer::fromBase64(std::begin(data), std::end(data));
}

template <class T>
inline T
Buffer::toBase64(const T &data)
{
    return Buffer::toBase64<T>(std::begin(data), std::end(data));
}

template <class InputIterator>
Buffer
Buffer::fromBase64(InputIterator first, InputIterator last)
{
    size_type padding = 0;

    for (int i = 1; i <= 2 && *(last - i) == '='; ++i) {
        ++padding;
    }

    typedef
        transform_width<
            binary_from_base64<InputIterator>, 8, 6, std::uint8_t
        > base64_decoder
    ;

    Buffer buffer(
        base64_decoder((InputIterator) first),
        base64_decoder((InputIterator) last)
    );

    buffer.resize(buffer.size() - padding);
    return buffer;
}

template <class T>
inline T
Buffer::toHex() const
{
    return Buffer::toHex<T>(begin(), end());
}

template <class T>
inline T
Buffer::toBase64() const
{
    return Buffer::toBase64<T>(begin(), end());
}

inline std::string
Buffer::toString() const
{
    return Buffer::toString(begin(), end());
}

inline const std::uint8_t *
Buffer::toArray() const
{
    return Buffer::toArray(begin(), end());
}

inline Buffer &
Buffer::operator=(const char *data)
{
    assign(data, data + std::strlen(data));

    return *this;
}

inline Buffer &
Buffer::operator=(const std::string &data)
{
    assign(data.begin(), data.end());

    return *this;
}


inline bool
operator==(const char *first, const Buffer &second)
{
    return std::equal(second.begin(), second.end(), first, first + std::strlen(first));
}

inline bool
operator==(const std::string &first, const Buffer &second)
{
    return std::equal(second.begin(), second.end(), first.begin(), first.end());
}

inline bool
operator==(const Buffer &first, const char *second)
{
    return std::equal(first.begin(), first.end(), second, second + std::strlen(second));
}

inline bool
operator==(const Buffer &first, const std::string &second)
{
    return std::equal(first.begin(), first.end(), second.begin(), second.end());
}


inline bool
operator!=(const char *first, const Buffer &second)
{
    return !(first == second);
}

inline bool
operator!=(const std::string &first, const Buffer &second)
{
    return !(first == second);
}

inline bool
operator!=(const Buffer &first, const char *second)
{
    return !(first == second);
}

inline bool
operator!=(const Buffer &first, const std::string &second)
{
    return !(first == second);
}


template <typename ... T_ARGS>
inline Buffer
format_buffer(const Buffer &fmt, T_ARGS && ... args)
{
    boost::format _fmt(fmt.toString());
    return format_buffer(_fmt, std::forward<T_ARGS>(args) ...);
}

template <typename T_ARG0, typename ... T_ARGS>
inline Buffer
format_buffer(boost::format &fmt, T_ARG0 &&arg0, T_ARGS && ... args)
{
    fmt % std::forward<T_ARG0>(arg0);
    return format_buffer(fmt, std::forward<T_ARGS>(args) ...);
}

} // End of main namespace

#endif /* __KM_BUFFER_INL_HPP__ */