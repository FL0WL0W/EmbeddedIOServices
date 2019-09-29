#include <stdint.h>
#include "ScalarVariableType.h"

#ifndef SCALARVARIABLE_H
#define SCALARVARIABLE_H
struct ScalarVariable
{
    ScalarVariableType Type;
    uint64_t Value;
};
template<typename K>
constexpr K ScalarVariableTo(ScalarVariable variable)
{
    switch(variable.Type)
    {
        case ScalarVariableType::UINT8: return static_cast<K>(*reinterpret_cast<uint8_t *>(&variable.Value));
        case ScalarVariableType::UINT16: return static_cast<K>(*reinterpret_cast<uint16_t *>(&variable.Value));
        case ScalarVariableType::UINT32: return static_cast<K>(*reinterpret_cast<uint32_t *>(&variable.Value));
        case ScalarVariableType::UINT64: return static_cast<K>(*reinterpret_cast<uint64_t *>(&variable.Value));
        case ScalarVariableType::INT8: return static_cast<K>(*reinterpret_cast<int8_t *>(&variable.Value));
        case ScalarVariableType::INT16: return static_cast<K>(*reinterpret_cast<int16_t *>(&variable.Value));
        case ScalarVariableType::INT32: return static_cast<K>(*reinterpret_cast<int32_t *>(&variable.Value));
        case ScalarVariableType::INT64: return static_cast<K>(*reinterpret_cast<int64_t *>(&variable.Value));
        case ScalarVariableType::FLOAT: return static_cast<K>(*reinterpret_cast<float *>(&variable.Value));
        case ScalarVariableType::DOUBLE: return static_cast<K>(*reinterpret_cast<double *>(&variable.Value));
        case ScalarVariableType::BOOLEAN: return static_cast<K>(*reinterpret_cast<bool *>(&variable.Value));
    }
    return 0;
}
constexpr ScalarVariable ScalarVariableFrom(uint8_t variable)
{
    return { ScalarVariableType::UINT8, *reinterpret_cast<uint64_t *>(&variable) };
}
constexpr ScalarVariable ScalarVariableFrom(uint16_t variable)
{
    return { ScalarVariableType::UINT16, *reinterpret_cast<uint64_t *>(&variable) };
}
constexpr ScalarVariable ScalarVariableFrom(uint32_t variable)
{
    return { ScalarVariableType::UINT32, *reinterpret_cast<uint64_t *>(&variable) };
}
constexpr ScalarVariable ScalarVariableFrom(uint64_t variable)
{
    return { ScalarVariableType::UINT64, variable };
}
constexpr ScalarVariable ScalarVariableFrom(int8_t variable)
{
    return { ScalarVariableType::INT8, *reinterpret_cast<uint64_t *>(&variable) };
}
constexpr ScalarVariable ScalarVariableFrom(int16_t variable)
{
    return { ScalarVariableType::INT16, *reinterpret_cast<uint64_t *>(&variable) };
}
constexpr ScalarVariable ScalarVariableFrom(int32_t variable)
{
    return { ScalarVariableType::INT32, *reinterpret_cast<uint64_t *>(&variable) };
}
constexpr ScalarVariable ScalarVariableFrom(int64_t variable)
{
    return { ScalarVariableType::INT64, *reinterpret_cast<uint64_t *>(&variable) };
}
constexpr ScalarVariable ScalarVariableFrom(float variable)
{
    return { ScalarVariableType::FLOAT, *reinterpret_cast<uint64_t *>(&variable) };
}
constexpr ScalarVariable ScalarVariableFrom(double variable)
{
    return { ScalarVariableType::DOUBLE, *reinterpret_cast<uint64_t *>(&variable) };
}
constexpr ScalarVariable ScalarVariableFrom(bool variable)
{
    return { ScalarVariableType::BOOLEAN, *reinterpret_cast<uint64_t *>(&variable) };
}
#endif