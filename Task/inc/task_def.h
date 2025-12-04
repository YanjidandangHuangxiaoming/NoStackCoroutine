#ifndef _TASK_DEF_H_
#define _TASK_DEF_H_

#include "stdint.h" // Standard integer type definition header file


#define TASK_ID(FUNC)   TASK_ID_##FUNC
#define TASK_NAME(FUNC) #FUNC

#ifndef NULL
    #define NULL (void*)0
#endif


#ifndef UNUSED
    #define UNUSED(x) (void)(x)
#endif



#endif
