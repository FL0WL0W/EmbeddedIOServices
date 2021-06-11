#include "stdint.h"

#ifndef ISTREAM_H
#define ISTREAM_H
namespace EmbeddedIOServices
{
	class IStream
	{
	public:
		virtual void Write(void *, size_t) = 0;

		template<typename K>
		void Write(K value)
		{
			Write(&value, sizeof(K));
		}

		virtual size_t Available() = 0;
		virtual void Read(void *, size_t) = 0;
		virtual void Peek(void *, size_t) = 0;

		template<typename K>
		K Read()
		{
			K value;
			Read(&value, sizeof(K));
			return value;
		}

		template<typename K>
		K Peek()
		{
			K value;
			Peek(&value, sizeof(K));
			return value;
		}
	};
}
#endif
