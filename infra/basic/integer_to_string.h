#if !defined(_INFRA_BASIC_INTEGER_TO_STRING_H_INCLUDED_)
#define _INFRA_BASIC_INTEGER_TO_STRING_H_INCLUDED_

#include "./string.h"

namespace infra
{
    namespace details
    {
        template<typename TU, TU _Value, bool _IsZero, size_type _Base>
        struct positive_to_string;

        template<typename TU, TU _Value, size_type _Base>
        struct positive_to_string<TU, _Value, /*_IsZero*/true, _Base>
        {
            static constexpr const auto value = make_string("");
        };

        template<typename TU, TU _Value, size_type _Base>
        struct positive_to_string<TU, _Value, /*_IsZero*/false, _Base>
        {
            INFRA_STATIC_ASSERT(_Base > 0);
            INFRA_STATIC_ASSERT(_Base <= 36);
            static constexpr const auto value =
                positive_to_string<TU, _Value / _Base, (_Value / _Base == 0), _Base>::value +
                "0123456789abcdefghijklmnopqrstuvwxyz"[_Value % _Base];
        };



        template<typename TInt, bool _IsZero, bool _IsNegative, size_type _Base>
        struct integer_to_string_helper;

        template<typename TUInt, size_type _Base>
        struct integer_to_string_helper<TUInt, /*_IsZero*/false, /*_IsNegative*/false, _Base>
        {
            template<TUInt _Value>
            static constexpr auto convert()
            {
                INFRA_STATIC_ASSERT(_Value > 0);
                return positive_to_string<
                    TUInt,
                    /*_IsZero*/ _Value,
                    /*_IsNegative*/ false,
                    _Base>::value;
            }
        };

        template<typename TInt, size_type _Base>
        struct integer_to_string_helper<TInt, /*_IsZero*/true, /*_IsNegative*/false, _Base>
        {
            template<TInt _Value>
            static constexpr auto convert()
            {
                INFRA_STATIC_ASSERT(_Value == 0);
                return make_string("0");
            }
        };

        template<typename TInt, size_type _Base>
        struct integer_to_string_helper<TInt, /*_IsZero*/false, /*_IsNegative*/true, _Base>
        {
            template<TInt _Value>
            static constexpr auto convert()
            {
                typedef typename std::make_unsigned<TInt>::type TUInt;
                return make_string("-") +
                    integer_to_string_helper<
                    TUInt,
                    /*_IsZero*/ false,
                    /*_IsNegative*/ false,
                    _Base>
                    ::template convert<(~TUInt(_Value) + 1)>();
            }
        };

    }  // namespace details


    /**
     * Convert an integer to string representation.
     */
    template<typename TInt, TInt _Value, size_type _Base = 10>
    constexpr auto integer_to_string() noexcept
    {
        //INFRA_STATIC_ASSERT(std::is_signed<TInt>::value || std::is_unsigned<TInt>::value);
        return details::integer_to_string_helper<
            TInt,
            /*_IsZero*/ (_Value == 0),
            /*_IsNegative*/ (_Value < 0),
            _Base>
            ::template convert<_Value>();
    }

}  // namespace infra

#endif  // !defined(_INFRA_BASIC_INTEGER_TO_STRING_H_INCLUDED_)
