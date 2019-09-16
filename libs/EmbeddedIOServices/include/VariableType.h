#include <stdint.h>

#ifndef VARIABLETYPE_H
#define VARIABLETYPE_H
const enum VariableType
{
    VOID = 0,
    UINT8 = 1,
    UINT16 = 2,
    UINT32 = 3,
    UINT64 = 4,
    INT8 = 5,
    INT16 = 6,
    INT32 = 7,
    INT64 = 8,
    FLOAT = 9,
    DOUBLE = 10,
    BOOLEAN = 11
};
constexpr uint8_t VariableTypeSizeOf(VariableType type)
{
    switch(type)
    {
        case VariableType::UINT8: return sizeof(uint8_t);
        case VariableType::UINT16: return sizeof(uint16_t);
        case VariableType::UINT32: return sizeof(uint32_t);
        case VariableType::UINT64: return sizeof(uint64_t);
        case VariableType::INT8: return sizeof(int8_t);
        case VariableType::INT16: return sizeof(int16_t);
        case VariableType::INT32: return sizeof(int32_t);
        case VariableType::INT64: return sizeof(int64_t);
        case VariableType::FLOAT: return sizeof(float);
        case VariableType::DOUBLE: return sizeof(double);
    }
    return 0;
}
#endif