#if !defined(_INFRA_PREDEF_MACRO_H_INCLUDED_)
#define _INFRA_PREDEF_MACRO_H_INCLUDED_

// Convert literal char string to wchat_t string
#define INFRA_CAT(_A_, _B_)     _A_##_B_
#define INFRA_WSTR(_Text_)      INFRA_CAT(L, #_Text_)


#define JUST(...)               __VA_ARGS__


#endif  // !defined(_INFRA_PREDEF_MACRO_H_INCLUDED_)
