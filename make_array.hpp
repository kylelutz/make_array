// ------------------------------------------------------------------------- //
// Copyright (c) 2013 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
// ------------------------------------------------------------------------- //

#ifndef BOOST_MAKE_ARRAY_HPP
#define BOOST_MAKE_ARRAY_HPP

#include <boost/array.hpp>
#include <boost/config.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES

// implementation for c++03 without variadic templates
#include <boost/preprocessor.hpp>

#ifndef BOOST_MAKE_ARRAY_MAX_ARITY
#define BOOST_MAKE_ARRAY_MAX_ARITY 10
#endif

namespace boost {

// make_array with zero-length
template<class T>
inline array<T, 0> make_array()
{
    return array<T, 0>();
}

#define BOOST_MAKE_ARRAY_DETAIL_DECLARE_TEMPLATE_PARAMETERS(z, i, _) \
    BOOST_PP_COMMA_IF(i) BOOST_PP_CAT(class T, i)

#define BOOST_MAKE_ARRAY_DETAIL_DECLARE_ARGUMENTS(z, i, _) \
    BOOST_PP_COMMA_IF(i) const BOOST_PP_CAT(T, i) BOOST_PP_CAT(&arg, i)

#define BOOST_MAKE_ARRAY_DETAIL_INSERT_ASSIGN(z, i, _) \
    a[i] = static_cast<T0>(BOOST_PP_CAT(arg, i));
  
#define BOOST_MAKE_ARRAY_DETAIL_DEFINE_FUNCTION(n) \
    template<BOOST_PP_REPEAT(n, BOOST_MAKE_ARRAY_DETAIL_DECLARE_TEMPLATE_PARAMETERS, _)> \
    inline array<T0, n> \
    make_array(BOOST_PP_REPEAT(n, BOOST_MAKE_ARRAY_DETAIL_DECLARE_ARGUMENTS, _)) \
    { \
        array<T0, n> a; \
        BOOST_PP_REPEAT(n, BOOST_MAKE_ARRAY_DETAIL_INSERT_ASSIGN, _) \
        return a; \
    }

#define BOOST_MAKE_ARRAY_DETAIL_DEFINE_FUNCTIONS(z, i, _) \
    BOOST_MAKE_ARRAY_DETAIL_DEFINE_FUNCTION(BOOST_PP_ADD(i, 1))

// define the make_array() functions
BOOST_PP_REPEAT(BOOST_MAKE_ARRAY_MAX_ARITY,
                BOOST_MAKE_ARRAY_DETAIL_DEFINE_FUNCTIONS,
                _
)

#undef BOOST_MAKE_ARRAY_DETAIL_DECLARE_TEMPLATE_PARAMETERS
#undef BOOST_MAKE_ARRAY_DETAIL_DECLARE_ARGUMENTS
#undef BOOST_MAKE_ARRAY_DETAIL_INSERT_ASSIGN
#undef BOOST_MAKE_ARRAY_DETAIL_DEFINE_FUNCTION
#undef BOOST_MAKE_ARRAY_DETAIL_DEFINE_FUNCTIONS

} // end boost namespace

#else

// implementation for c++11 with variadic templates
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace detail {

// returns the type of the first value in Args
template<class T, class... Args>
struct get_first_arg
{
    typedef T type;
};

template<class Array>
void push_array(Array &)
{
}

template<class Array, class T, class... Args>
void push_array(Array &array, const T &value, Args&&... args)
{
    const size_t n = Array::static_size - sizeof...(Args) - 1;
    array[n] = static_cast<typename Array::value_type>(value);
    push_array(array, args...);
}

template<class T, class... Args>
inline array<T, sizeof...(Args)>
make_array_impl(Args&&... args)
{
    boost::array<T, sizeof...(Args)> array;
    detail::push_array(array, args...);
    return array;
}

} // end detail namespace

// make_array with zero-length
template<class T>
inline array<T, 0> make_array()
{
    return array<T, 0>();
}

// make_array with explicit type
template<class T, class... Args>
inline array<T, sizeof...(Args)>
make_array(Args&&... args,
           typename disable_if<
               typename is_same<
                   T,
                   typename detail::get_first_arg<Args...>::type
               >::type
           >::type* = 0)
{
    return detail::make_array_impl<T>(args...);
}

// make_array with deduced type
template<class... Args>
inline array<typename detail::get_first_arg<Args...>::type, sizeof...(Args)>
make_array(Args&&... args)
{
    typedef typename detail::get_first_arg<Args...>::type T;

    return detail::make_array_impl<T>(args...);
}

} // end boost namespace

#endif // BOOST_NO_VARIADIC_TEMPLATES

#endif // BOOST_MAKE_ARRAY_HPP
