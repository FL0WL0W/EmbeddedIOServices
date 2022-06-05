#include "stdint.h"
#include "stdlib.h"

#ifndef CRC_H
#define CRC_H
namespace EmbeddedIOServices
{
	class CRC
    {
    public:
        static uint32_t CRC32(const void *data, size_t length);
    };
}
#endif