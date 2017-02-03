/*!
 * Title ---- km/Buffer.cpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#include <km/Buffer.hpp>

namespace km { // Begin main namespace

const std::string
Buffer::mBase64Suffix[] = { "", "==", "=" };

template <>
Buffer
Buffer::toHex<Buffer>(const_iterator first, const_iterator last)
{
    Buffer output;
    algorithm::hex(first, last, std::back_inserter(output));
    return output;
}

template <>
std::string
Buffer::toHex<std::string>(const_iterator first, const_iterator last)
{
    std::string output;
    algorithm::hex(first, last, std::back_inserter(output));
    return output;
}

template <>
Buffer
Buffer::toBase64<Buffer>(const_iterator first, const_iterator last)
{
    Buffer output;
    std::copy(base64_encoder(first), base64_encoder(last), std::back_inserter(output));

    auto padding = (size_type) std::distance(first, last) % 3;

    auto suffix = mBase64Suffix[padding];
    std::copy(std::begin(suffix), std::end(suffix), std::back_inserter(output));

    return output;
}

template <>
std::string
Buffer::toBase64<std::string>(const_iterator first, const_iterator last)
{
    std::string output;
    std::copy(base64_encoder(first), base64_encoder(last), std::back_inserter(output));

    size_type padding = (size_type) std::distance(first, last) % 3;
    output.append(mBase64Suffix[padding]);

    return output;
}

Buffer
Buffer::fromRandom(size_type size)
{
    Buffer buffer(size);

    if (!RAND_bytes((unsigned char *) buffer.data(), (int) size)) {
        throw Exception("Error with 'RAND_bytes'");
    }

    return buffer;
}

Buffer::Buffer() : Memory()
{

}

Buffer::Buffer(const char *data) : Buffer(data, data + std::strlen(data))
{

}

Buffer::Buffer(const std::string &data) : Memory(std::cbegin(data), std::cend(data))
{

}

std::ostream &
operator<<(std::ostream &out, const Buffer &buffer)
{
    SafeStringStream ss;

    ss.flags(out.flags());
    ss.imbue(out.getloc());

    ss.write((const char *) buffer.data(), buffer.size());

    return out << ss.str();
}


Buffer
format_buffer(boost::format &fmt)
{
    typedef
        std::vector<
            char, SafeAllocator<char>
        > SafeVector
    ;

    SafeVector output;

    iostreams::back_insert_device<SafeVector> sink(output);
    iostreams::stream<iostreams::back_insert_device<SafeVector>> os(sink);

    os << fmt << std::flush;

    return Buffer(output.cbegin(), output.cend());
}

} // End of main namespace