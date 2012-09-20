#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/fusion/include/at.hpp>
#include <type_traits>
#include <iostream>

BOOST_FUSION_DEFINE_STRUCT(
    (), Foo,
    (int, x)
    (int, y))

BOOST_FUSION_DEFINE_STRUCT(
    (), Bar,
    (int, z)
    (Foo, foo))

template<template<typename> class Func>
struct Helper {
    template<typename T>
    static typename std::enable_if<boost::fusion::traits::is_sequence<T>::value>::type
    print(const char *name, const T & val) {
	typedef boost::mpl::range_c<int, 0, boost::mpl::size<T>::value> range;
	boost::mpl::for_each<range>(Func<T>(val));
    }

    template<typename T>
    static typename std::enable_if<std::is_integral<T>::value>::type
    print(const char *name, const T & val) {
	std::cout << name << ":" << val << ", ";
    }
};

template<typename T>
struct Func {
    const T & val;

    Func(const T & val) : val(val) { }

    template<typename N>
    void operator()(N) const {
	const char *name = boost::fusion::extension::struct_member_name<T, N::value>::call();
	auto &child = boost::fusion::at<N>(val);
	Helper<Func>::print(name, child);       
    }
};

template<typename T>
void doit(const T & val) {
    std::cout << "{ ";
    typedef boost::mpl::range_c<int, 0, boost::mpl::size<T>::value> range;
    boost::mpl::for_each<range>(Func<T>(val));
    std::cout << "} ";
}

int main(int argc, char ** argv) {
    Bar bar = { 1, { 2, 3 } };
    doit(bar);
}
