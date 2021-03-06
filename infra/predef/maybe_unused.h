#if !defined(_INFRA_PREDEF_MAYBE_UNUSED_H_INCLUDED_)
#define _INFRA_PREDEF_MAYBE_UNUSED_H_INCLUDED_

namespace infra
{
    template<typename T1, typename... Ts>
    constexpr void maybe_unused(const T1&, const Ts& ...) noexcept
    {
        // Just a dummy function...
    }

    template<typename T1, typename... Ts>
    constexpr void maybe_unused() noexcept
    {
        // Just a dummy function...
    }

}  // namespace infra

#endif  // !defined(_INFRA_PREDEF_MAYBE_UNUSED_H_INCLUDED_)
