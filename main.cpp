#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/fusion/include/value_at.hpp>
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

struct Printer {
    template<typename T>
    typename std::enable_if<boost::fusion::traits::is_sequence<T>::value>::type
    operator()(const T & val) const {
	boost::fusion::for_each(val, *this);
    }

    template<typename T>
    typename std::enable_if<std::is_integral<T>::value>::type
    operator()(const T & val) const {
	std::cout << val << std::endl;
    }
};

template<template<typename> class Func>
struct Helper {
    template<typename T>
    static typename std::enable_if<boost::fusion::traits::is_sequence<T>::value>::type
    print(const char *name, T*) {
	std::cout << name << " sequence" << std::endl;	
	
	typedef boost::mpl::range_c<int, 0, boost::mpl::size<T>::value> range;
	boost::mpl::for_each<range>(Func<T>());
    }

    template<typename T>
    static typename std::enable_if<std::is_integral<T>::value>::type
    print(const char *name, T*) {
	std::cout << name << " integral" << std::endl;
    }
};

template<typename T>
struct Func {
    template<typename N>
    void operator()(N) const {
	const char *name = boost::fusion::extension::struct_member_name<T, N::value>::call();
	typename boost::fusion::result_of::value_at<T, N>::type *ptr = NULL;
	Helper<Func>::print(name, ptr);
    }
};

template<typename T>
void doit(const T & val) {
    boost::fusion::for_each(val, Printer());

    typedef boost::mpl::range_c<int, 0, boost::mpl::size<T>::value> range;
    boost::mpl::for_each<range>(Func<T>());
}

int main(int argc, char ** argv) {
    Bar bar = { 1, { 2, 3 } };
    doit(bar);
}
