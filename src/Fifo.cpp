#include "Fifo.h"

#ifdef FIFO_H
namespace EmbeddedIOServices
{	
	Fifo::Fifo(size_t size) : Size(size), _buf(std::malloc(Size)), _in(0), _out(0) {}
    Fifo::~Fifo() {  std::free(_buf); }

    size_t Fifo::Length() 
    {
        return _in - _out;
    }

    size_t Fifo::Space() 
    {
	    return Size - (_in - _out);
    }

    size_t Fifo::Write(void *buf, size_t len)
    {
        size_t j = _in % Size;
        len = std::min(len, Size - _in + _out);
        size_t i = std::min(len, Size - j);
        std::memcpy(reinterpret_cast<uint8_t *>(_buf) + j, buf, i);
        std::memcpy(_buf, reinterpret_cast<uint8_t *>(buf) + i, len - i);
        _in += len;
        return len;
    }

    size_t Fifo::Read(void *buf, size_t len)
    {
        len = Peak(buf, len);
        Pop(len);
        return len;
    }

    size_t Fifo::Peak(void *buf, size_t len)
    {
        size_t j = _out % Size;
        len = std::min(len, _in - _out);
        size_t i = std::min(len, Size - j);
        std::memcpy(buf, reinterpret_cast<uint8_t *>(_buf) + j, i);
        std::memcpy(reinterpret_cast<uint8_t *>(buf) + i, _buf, len - i);
        return len;
    }
    
    void Fifo::Pop(size_t len)
    {
        len = std::min(len, _in - _out);
        _out += len;
    }

    void Fifo::Clear()
    {
        _in = 0;
        _out = 0;
    }
}
#endif