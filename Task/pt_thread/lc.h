#ifndef __LC_H__
#define __LC_H__

/* 包含检测宏 */
#include "lc-detect.h"

#ifdef LC_INCLUDE
    #include LC_INCLUDE
#else
    /* 根据编译器支持情况自动选择实现 */
    #if HAS_LABELS_AS_VALUES
        /* 使用基于"labels as values"的实现，它不依赖switch-case */
        #include "lc-addrlabels.h"
    #else
        /* 回退到基于switch-case的实现 */
        #include "lc-switch.h"
        /* 注意：在switch内部不要使用PT_WAIT_UNTIL宏，否则会导致"case expression not constant"错误 */
    #endif
#endif

#endif
