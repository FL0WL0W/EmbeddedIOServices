#include "stdint.h"
#include <list>
#include <cstring>
#include <algorithm>

#ifndef FIFO_H
#define FIFO_H
namespace EmbeddedIOServices
{
	class Fifo
    {
    protected:
        void *_buf;
        size_t _in;
        size_t _out;

    public:
        const size_t Size;
        Fifo(size_t size);
        ~Fifo();

        size_t Length();
        size_t Space();
        size_t Write(void *buf, size_t len);
        size_t Read(void *buf, size_t len);
        size_t Peak(void *buf, size_t len);
        void Pop(size_t len);
        void Clear();
    };
}
#endif