#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

namespace Au{
namespace Media{
namespace FBX{
    
typedef uint8_t Byte;
typedef uint32_t Word;
typedef uint64_t DoubleWord;
    
uint32_t Offset(const char* begin, const char* cursor)
{
    return static_cast<unsigned int>(cursor - begin);
}

template<typename TYPE>
TYPE Read(const char* data, const char*& cursor, const char* end)
{
    if(Offset(cursor, end) < sizeof(TYPE))
        return 0;
    
    TYPE result = *reinterpret_cast<const TYPE*>(cursor);
    
    cursor += sizeof(TYPE);
    
    return result;
}

bool ReadString(std::string& out, const char* data, const char*& cursor, const char* end,
    bool word_len = false)
{
    const Word len_len = word_len ? 4 : 1;
    if(Offset(cursor, end) < len_len)
        return false;
    
    const Word length = word_len ? Read<Word>(data, cursor, end) : Read<Byte>(data, cursor, end);
    
    if(Offset(cursor, end) < length)
        return false;
    
    std::string str(cursor, cursor + length);
    out = str;
    cursor += length;
    
    return true;
}

void ReadData(std::vector<char>& out, const char* data, const char*& cursor, const char* end)
{
    if(Offset(cursor, end) < 1)
    {
        std::cout << "out of bounds while reading data length" << std::endl;
        return;
    }
    
    const char type = *cursor;
    const char* sbegin = ++cursor;
    
    switch(type)
    {
    // 16 bit int
    case 'Y':
        cursor += 2;
        break;
    // 1 bit bool
    case 'C':
        cursor += 1;
        break;
    // 32 bit int
    case 'I':
        cursor += 4;
        break;
    case 'F':
        cursor += 4;
        break;
    // double
    case 'D':
        cursor += 8;
        break;
    // 64 bit int
    case 'L':
        cursor += 8;
        break;
    // Binary data
    case 'R':
    {
        const Word length = Read<Word>(data, cursor, end);
        cursor += length;
        break;
    }
    case 'b':
        cursor = end;
        break;
    case 'f':
    case 'd':
    case 'l':
    case 'i':
    {
        const Word length = Read<Word>(data, cursor, end);
        const Word encoding = Read<Word>(data, cursor, end);
        const Word comp_len = Read<Word>(data, cursor, end);
        sbegin = cursor;
        if(encoding == 0)
        {
            Word stride = 0;
            switch(type)
            {
            case 'f':
                std::cout << "float array, size: " << length << std::endl;
            case 'i':
                std::cout << "int array, size: " << length << std::endl;
                stride = 4;
                break;
            case 'd':
                std::cout << "double array, size: " << length << std::endl;
            case 'l':
                std::cout << "long array, size: " << length << std::endl;
                stride = 8;
                break;
            }
            
            if(length * stride != comp_len)
            {
                std::cout << "Failed to ReadData, calculated data stride differs from what the file claims" << std::endl;
                return;
            }
        }
        else if(encoding != 1)
        {
            std::cout << "ReadData failed, unknown encoding" << std::endl;
            return;
        }
        cursor += comp_len;
        break;
    }
    case 'S':
    {
        std::string str;
        ReadString(str, data, cursor, end, true);
        std::cout << "data str: " << str << std::endl;
        break;
    }
    default:
        std::cout << "ReadData failed, unexpected type code: " << std::string(&type, 1) << std::endl;
        return;
    }
    
    if(cursor > end)
    {
        std::cout << "ReadData failed, remaining size too small" << std::endl;
        return;
    }
    
    const char* send = cursor;
    out = std::vector<char>(sbegin, send);
}

bool ReadBlock(const char* data, const char*& cursor, const char* end, Word flags)
{
    const Word end_offset = Read<Word>(data, cursor, end);
    
    if(end_offset == 0)
    {
        std::cout << "end_offset is 0" << std::endl;
        return false;
    }
    if(end_offset > Offset(data, end))
    {
        std::cout << "end_offset > Offset(data, end)" << std::endl;
        return false;
    }
    else if(end_offset < Offset(data, cursor))
    {
        std::cout << "end_offset < Offset(data, end)" << std::endl;
        return false;
    }
    const Word prop_count = Read<Word>(data, cursor, end);
    const Word prop_len = Read<Word>(data, cursor, end);
    
    std::string block_name;
    ReadString(block_name, data, cursor, end);
    std::cout << "BLOCK: [" << block_name << "]" << std::endl;
    
    const char* begin_cur = cursor;
    for(unsigned i = 0; i < prop_count; ++i)
    {
        std::vector<char> actual_data;
        ReadData(actual_data, data, cursor, begin_cur + prop_len);
    }
    
    if(Offset(begin_cur, cursor) != prop_len)
    {
        std::cout << "Property length was not reached" << std::endl;
        return false;
    }
    
    const size_t sentinel_block_length = sizeof(Word) * 3 + 1;
    
    if(Offset(data, cursor) < end_offset)
    {
        if(end_offset - Offset(data, cursor) < sentinel_block_length)
        {
            std::cout << "Insufficient padding bytes at block end" << std::endl;
            return false;
        }
        
        while(Offset(data, cursor) < end_offset - sentinel_block_length)
            ReadBlock(data, cursor, data + end_offset - sentinel_block_length, flags);
        
        for(unsigned i = 0; i < sentinel_block_length; ++i)
        {
            if(cursor[i] != '\0')
            {
                std::cout << "13 zero bytes expected" << std::endl;
                return 0;
            }
        }
        cursor += sentinel_block_length;
    }
    
    if(Offset(data, cursor) != end_offset)
    {
        std::cout << "scope length not reached" << std::endl;
        return false;
    }
    
    return true;
}

bool Read(const char* data, unsigned size)
{
    std::cout << "Reading FBX file..." << std::endl;
    if(!data)
    {
        std::cout << "data is null" << std::endl;
        return false;
    }
    if(size < 0x1b)
    {
        std::cout << "size is to small" << std::endl;
        return false;
    }
    if(strncmp(data, "Kaydara FBX Binary", 18))
    {
        std::cout << "Invalid FBX header" << std::endl;
        return false;
    }
    std::cout << "Found FBX header" << std::endl;
    
    const char* cursor = data + 0x15;
    
    const Word flags = Read<Word>(data, cursor, data + size);
    const Byte padding_0 = Read<Byte>(data, cursor, data + size);
    const Byte padding_1 = Read<Byte>(data, cursor, data + size);
    
    while(cursor < data + size)
    {
        if(!ReadBlock(data, cursor, data + size, flags))
            return false;
    }
    
    return true;
}

}
}
}