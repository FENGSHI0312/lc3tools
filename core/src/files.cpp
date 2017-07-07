#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "utils.h"

#include "files.h"

std::ostream & utils::operator<<(std::ostream & out, utils::ObjectFileStatement const & in)
{
    // TODO: this is extrememly unportable, namely because it relies on the endianness not changing
    // size of num_bytes + value + orig + line + nullptr
    uint32_t num_bytes = 2 + 1 + 4 + in.line.size();
    char * bytes = new char[num_bytes];
    std::memcpy(bytes    , (char *) (&in.value), 2);
    std::memcpy(bytes + 2, (char *) (&in.orig), 1);
    uint32_t num_chars = in.line.size();
    std::memcpy(bytes + 3, (char *) (&num_chars), 4);
    char const * data = in.line.data();
    std::memcpy(bytes + 7, data, num_chars);
    out.write(bytes, num_bytes);
    delete[] bytes;
    return out;
}

std::istream & utils::operator>>(std::istream & in, utils::ObjectFileStatement & out)
{
    in.read((char *) (&out.value), 2);
    in.read((char *) (&out.orig), 1);
    uint32_t num_chars;
    in.read((char *) (&num_chars), 4);
    char * chars = new char[num_chars];
    in.read(chars, num_chars);
    out.line = std::string(chars);
    return in;
}
