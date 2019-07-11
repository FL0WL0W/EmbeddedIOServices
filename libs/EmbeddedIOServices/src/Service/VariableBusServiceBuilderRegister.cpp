#include "Service/VariableBusServiceBuilderRegister.h"
#include "VariableBus/BooleanVariableService/BooleanVariableService_Static.h"
#include "VariableBus/BooleanVariableService/BooleanVariableService_Input.h"
#include "VariableBus/FloatVariableService/FloatVariableService_Static.h"
#include "VariableBus/FloatVariableService/FloatVariableService_Input.h"
#include "VariableBus/FloatVariableService/FloatVariableService_PolynomialTranslation.h"
#include "VariableBus/FloatVariableService/FloatVariableService_LookupTableTranslation.h"
#include "VariableBus/FloatVariableService/FloatVariableService_2AxisTableTranslation.h"

using namespace VariableBus;

namespace Service
{
		void VariableBusServiceBuilderRegister::Register(ServiceBuilder *&serviceBuilder)
        {
            serviceBuilder->Register(BUILDER_BOOLEANVARIABLESERVICE_STATIC, BooleanVariableService_Static::BuildBooleanVariableService_Static);
            serviceBuilder->Register(BUILDER_BOOLEANVARIABLESERVICE_INPUT, BooleanVariableService_Input::BuildBooleanVariableService_Input);
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_STATIC, FloatVariableService_Static::BuildFloatVariableService_Static);
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_INPUT, FloatVariableService_Input::BuildFloatVariableService_Input);
#ifdef BUILDER_FLOATVARIABLESERVICE_DEGREE2POLYNOMIALTRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DEGREE2POLYNOMIALTRANSLATION, FloatVariableService_PolynomialTranslation<2>::BuildFloatVariableService_PolynomialTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_DEGREE3POLYNOMIALTRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DEGREE3POLYNOMIALTRANSLATION, FloatVariableService_PolynomialTranslation<3>::BuildFloatVariableService_PolynomialTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_DEGREE4POLYNOMIALTRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DEGREE4POLYNOMIALTRANSLATION, FloatVariableService_PolynomialTranslation<4>::BuildFloatVariableService_PolynomialTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_DEGREE5POLYNOMIALTRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DEGREE5POLYNOMIALTRANSLATION, FloatVariableService_PolynomialTranslation<5>::BuildFloatVariableService_PolynomialTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_DEGREE6POLYNOMIALTRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DEGREE6POLYNOMIALTRANSLATION, FloatVariableService_PolynomialTranslation<6>::BuildFloatVariableService_PolynomialTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_DEGREE7POLYNOMIALTRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DEGREE7POLYNOMIALTRANSLATION, FloatVariableService_PolynomialTranslation<7>::BuildFloatVariableService_PolynomialTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_DEGREE8POLYNOMIALTRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DEGREE8POLYNOMIALTRANSLATION, FloatVariableService_PolynomialTranslation<8>::BuildFloatVariableService_PolynomialTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_DEGREE9POLYNOMIALTRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DEGREE9POLYNOMIALTRANSLATION, FloatVariableService_PolynomialTranslation<9>::BuildFloatVariableService_PolynomialTranslation);
#endif

#ifdef BUILDER_FLOATVARIABLESERVICE_UINT8_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_UINT8_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<uint8_t>::BuildFloatVariableService_LookupTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_UINT16_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_UINT16_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<uint16_t>::BuildFloatVariableService_LookupTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_UINT32_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_UINT32_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<uint32_t>::BuildFloatVariableService_LookupTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_UINT64_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_UINT64_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<uint64_t>::BuildFloatVariableService_LookupTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_INT8_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_INT8_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<int8_t>::BuildFloatVariableService_LookupTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_INT16_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_INT16_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<int16_t>::BuildFloatVariableService_LookupTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_INT32_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_INT32_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<int32_t>::BuildFloatVariableService_LookupTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_INT64_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_INT64_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<int64_t>::BuildFloatVariableService_LookupTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_FLOAT_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_FLOAT_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<float>::BuildFloatVariableService_LookupTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_DOUBLE_LOOKUPTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DOUBLE_LOOKUPTABLETRANSLATION, FloatVariableService_LookupTableTranslation<double>::BuildFloatVariableService_LookupTableTranslation);
#endif

#ifdef BUILDER_FLOATVARIABLESERVICE_UINT8_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_UINT8_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<uint8_t>::BuildFloatVariableService_2AxisTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_UINT16_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_UINT16_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<uint16_t>::BuildFloatVariableService_2AxisTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_UINT32_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_UINT32_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<uint32_t>::BuildFloatVariableService_2AxisTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_UINT64_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_UINT64_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<uint64_t>::BuildFloatVariableService_2AxisTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_INT8_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_INT8_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<int8_t>::BuildFloatVariableService_2AxisTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_INT16_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_INT16_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<int16_t>::BuildFloatVariableService_2AxisTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_INT32_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_INT32_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<int32_t>::BuildFloatVariableService_2AxisTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_INT64_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_INT64_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<int64_t>::BuildFloatVariableService_2AxisTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_FLOAT_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_FLOAT_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<float>::BuildFloatVariableService_2AxisTableTranslation);
#endif
#ifdef BUILDER_FLOATVARIABLESERVICE_DOUBLE_2AXISTABLETRANSLATION
            serviceBuilder->Register(BUILDER_FLOATVARIABLESERVICE_DOUBLE_2AXISTABLETRANSLATION, FloatVariableService_2AxisTableTranslation<double>::BuildFloatVariableService_2AxisTableTranslation);
#endif
        }
}