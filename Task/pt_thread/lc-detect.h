#ifndef __LC_DETECT_H__
#define __LC_DETECT_H__

/*
 * 检测编译器是否支持"labels as values"特性
 * GCC和Clang支持此特性，而MSVC不支持
 */

#if (defined(__GNUC__) || defined(__clang__)) && !defined(__STRICT_ANSI__)
    /* GCC或Clang编译器，且未开启严格ANSI模式，通常支持labels as values */
    #define HAS_LABELS_AS_VALUES 1
#else
    /* 其他编译器可能不支持，如MSVC */
    #define HAS_LABELS_AS_VALUES 0
#endif

/* 验证确认编译器确实支持此特性的更精确方法 */
#if HAS_LABELS_AS_VALUES
    /* 编译时测试是否真的支持 */
    #define VALIDATE_LABELS_AS_VALUES() \
        do { \
            void *ptr; \
            test_label: \
            ptr = &&test_label; /* 使用&&操作符获取标签地址 */ \
            (void)ptr; /* 避免未使用变量的警告 */ \
        } while(0)
#else
    /* 不支持的情况下，提供空实现 */
    #define VALIDATE_LABELS_AS_VALUES() do {} while(0)
#endif

#endif /* __LC_DETECT_H__ */