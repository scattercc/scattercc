#if !defined(_INFRA_PREDEF_TYPES_H_INCLUDED_)
#define _INFRA_PREDEF_TYPES_H_INCLUDED_

#include "static_assert.h"
#include <cstdint>
#include <type_traits>

namespace infra
{
    typedef std::size_t size_type;

    typedef typename std::make_signed<size_type>::type ssize_type;

    template<size_type _Value>
    using size_type_constant = std::integral_constant<size_type, _Value>;

}  // namespace infra

#endif  // !defined(_INFRA_PREDEF_TYPES_H_INCLUDED_)
