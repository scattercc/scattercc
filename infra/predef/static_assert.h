#if !defined(_INFRA_PREDEF_STATIC_ASSERT_H_INCLUDED_)
#define _INFRA_PREDEF_STATIC_ASSERT_H_INCLUDED_

#define INFRA_STATIC_ASSERT(...) \
    static_assert((__VA_ARGS__), "Expect: " #__VA_ARGS__)

#endif  // !defined(_INFRA_PREDEF_STATIC_ASSERT_H_INCLUDED_)
