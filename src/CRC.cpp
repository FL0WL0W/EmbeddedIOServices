#include "CRC.h"

#ifdef CRC_H
namespace EmbeddedIOServices
{	
    uint32_t CRC::CRC32(const void *data, size_t length)
    {
        uint32_t crc32table[256];
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t b = i;
            for (uint32_t j = 0; j < 8; ++j) {
                b = b & 1 ? 0xEDB88320 ^ (b >> 1) : b >> 1;
            }
            crc32table[i] = b;
        }

        uint32_t crc = -1;

        const uint8_t *dataBytes = reinterpret_cast<const uint8_t *>(data);
        for (size_t i = 0; i < length; ++i) {
            crc = crc32table[(crc ^ dataBytes[i]) & 0xFF] ^ (crc >> 8);
        }

        crc ^= -1;

        return crc;
    }
}
#endif
