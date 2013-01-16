// ------------------------------------------------------------------------- //
// Copyright (c) 2013 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
// ------------------------------------------------------------------------- //

#define BOOST_TEST_MODULE TestMakeArray
#include <boost/test/unit_test.hpp>

#include <string>

#include <boost/array.hpp>
#include <boost/static_assert.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/is_same.hpp>

#include "make_array.hpp"

BOOST_AUTO_TEST_CASE(make_array)
{
    boost::array<int, 0> a = boost::make_array<int>();
    BOOST_CHECK(a.size() == size_t(0));

    boost::array<float, 1> b = boost::make_array(1.2f);
    BOOST_CHECK(b.size() == size_t(1));
    BOOST_CHECK_EQUAL(b[0], float(1.2f));

    boost::array<float, 2> c = boost::make_array<float>(1, 2);
    BOOST_CHECK(c.size() == size_t(2));
    BOOST_CHECK_EQUAL(c[0], float(1));
    BOOST_CHECK_EQUAL(c[1], float(2));

    boost::array<std::string, 3> d =
        boost::make_array<std::string>("h", "el", "lo");
    BOOST_CHECK(d.size() == size_t(3));
    BOOST_CHECK_EQUAL(d[0], std::string("h"));
    BOOST_CHECK_EQUAL(d[1], std::string("el"));
    BOOST_CHECK_EQUAL(d[2], std::string("lo"));
}

BOOST_AUTO_TEST_CASE(make_array_type)
{
    BOOST_STATIC_ASSERT((
        boost::is_same<
            BOOST_TYPEOF(boost::make_array<int>()),
            boost::array<int, 0>
        >::value
    ));

    BOOST_STATIC_ASSERT((
        boost::is_same<
            BOOST_TYPEOF(boost::make_array(1.2f)),
            boost::array<float, 1>
        >::value
    ));

    BOOST_STATIC_ASSERT((
        boost::is_same<
            BOOST_TYPEOF(boost::make_array<float>(1, 2)),
            boost::array<float, 2>
        >::value
    ));

    BOOST_STATIC_ASSERT((
        boost::is_same<
            BOOST_TYPEOF(boost::make_array<int>(1.2f, 2.2f)),
            boost::array<int, 2>
        >::value
    ));

    BOOST_STATIC_ASSERT((
        boost::is_same<
            BOOST_TYPEOF(boost::make_array(1, 2.1f)),
            boost::array<float, 2>
        >::value
    ));

    BOOST_STATIC_ASSERT((
        boost::is_same<
            BOOST_TYPEOF(boost::make_array(1, 2.1f, 3.2)),
            boost::array<double, 3>
        >::value
    ));

    BOOST_STATIC_ASSERT((
        boost::is_same<
            BOOST_TYPEOF(boost::make_array<std::string>("h", "el", "lo")),
            boost::array<std::string, 3>
        >::value
    ));
}
