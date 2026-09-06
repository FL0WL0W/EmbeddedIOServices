
#if defined(MPC5674F)
#include "MPC5674F.h"
#elif defined(MPC5566)
#include "MPC5566.h"
typedef FLEXCAN2_tag::canbuf_t canbuf_t;
#else
#error "No supported MPC5xxx processor has been selected"
#endif
