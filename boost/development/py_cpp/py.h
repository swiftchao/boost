//  (C) Copyright David Abrahams 2000. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  The author gratefully acknowleges the support of Dragon Systems, Inc., in
//  producing this work.

#ifndef METHOD_DWA122899_H_
# define METHOD_DWA122899_H_

# include "pyconfig.h"
# include "wrap_python.h"
# include "none.h"
# include "signatures.h"
# include <boost/smart_ptr.hpp>
# include "errors.h"
# include <string>

BOOST_PYTHON_BEGIN_CONVERSION_NAMESPACE // this is a gcc 2.95.2 bug workaround

// This can be instantiated on an enum to provide the to_python/from_python
// conversions, provided the values can fit in a long.
template <class EnumType>
class py_enum_as_int_converters
{
    friend EnumType from_python(PyObject* x, python::type<EnumType>)
    {
        return static_cast<EnumType>(
            from_python(x, python::type<long>()));
    }

    friend EnumType from_python(PyObject* x, python::type<const EnumType&>)
    {
        return static_cast<EnumType>(
            from_python(x, python::type<long>()));
    }

    friend PyObject* to_python(EnumType x)
    {
        return to_python(static_cast<long>(x));
    }
};
BOOST_PYTHON_END_CONVERSION_NAMESPACE

namespace python {
template <class EnumType> class enum_as_int_converters
    : public BOOST_PYTHON_CONVERSION::py_enum_as_int_converters<EnumType> {};

template <class P, class T> class wrapped_pointer;

//#pragma warn_possunwant off
inline void decref_impl(PyObject* p) { Py_DECREF(p); }
inline void xdecref_impl(PyObject* p) { Py_XDECREF(p); }
//#pragma warn_possunwant reset

template <class T>
inline void decref(T* p)
{
	char* const raw_p = reinterpret_cast<char*>(p);
	char* const p_base = raw_p - offsetof(PyObject, ob_refcnt);
	decref_impl(reinterpret_cast<PyObject*>(p_base));
}

template <class T>
inline void xdecref(T* p)
{
	char* const raw_p = reinterpret_cast<char*>(p);
	char* const p_base = raw_p - offsetof(PyObject, ob_refcnt);
	xdecref_impl(reinterpret_cast<PyObject*>(p_base));
}

} // namespace python

BOOST_PYTHON_BEGIN_CONVERSION_NAMESPACE
//
// Converters
//
PyObject* to_python(long);
long from_python(PyObject* p, python::type<long>);
long from_python(PyObject* p, python::type<const long&>);

PyObject* to_python(unsigned long);
unsigned long from_python(PyObject* p, python::type<unsigned long>);
unsigned long from_python(PyObject* p, python::type<const unsigned long&>);

PyObject* to_python(int);
int from_python(PyObject*, python::type<int>);
int from_python(PyObject*, python::type<const int&>);
    
PyObject* to_python(unsigned int);
unsigned int from_python(PyObject*, python::type<unsigned int>);
unsigned int from_python(PyObject*, python::type<const unsigned int&>);

PyObject* to_python(short);
short from_python(PyObject*, python::type<short>);
short from_python(PyObject*, python::type<const short&>);
    
PyObject* to_python(unsigned short);
unsigned short from_python(PyObject*, python::type<unsigned short>);
unsigned short from_python(PyObject*, python::type<const unsigned short&>);

PyObject* to_python(signed char);
signed char from_python(PyObject*, python::type<signed char>);
signed char from_python(PyObject*, python::type<const signed char&>);

PyObject* to_python(unsigned char);
unsigned char from_python(PyObject*, python::type<unsigned char>);
unsigned char from_python(PyObject*, python::type<const unsigned char&>);

PyObject* to_python(float);
float from_python(PyObject*, python::type<float>);
float from_python(PyObject*, python::type<const float&>);
    
PyObject* to_python(double);
double from_python(PyObject*, python::type<double>);
double from_python(PyObject*, python::type<const double&>);
    
PyObject* to_python(bool);
bool from_python(PyObject*, python::type<bool>);
bool from_python(PyObject*, python::type<const bool&>);

PyObject* to_python(void);
void from_python(PyObject*, python::type<void>);

PyObject* to_python(const char* s);
const char* from_python(PyObject*, python::type<const char*>);

PyObject* to_python(const std::string& s);
std::string from_python(PyObject*, python::type<std::string>);
std::string from_python(PyObject*, python::type<const std::string&>);

// For when your C++ function really wants to pass/return a PyObject*
PyObject* to_python(PyObject*);
PyObject* from_python(PyObject*, python::type<PyObject*>);

// Some standard conversions to/from smart pointer types. You can add your own
// from these examples.  These are not generated using the friend technique from
// wrapped_pointer because:
//
//      1. We want to be able to extend conversion to/from WrappedPointers using
//      arbitrary smart pointer types.
//
//      2. It helps with compilation independence. This way, code which creates
//      wrappers for functions accepting and returning smart_ptr<T> does not
//      have to have already seen the invocation of wrapped_type<T>.
//

// Unfortunately, MSVC6 is so incredibly lame that we have to rely on the friend
// technique to auto_generate standard pointer conversions for wrapped
// types. This means that you need to write a non-templated function for each
// specific smart_ptr<T> which you want to convert from_python. For example,
//
// namespace python {
// #ifdef MUST_SUPPORT_MSVC
//
// MyPtr<Foo> from_python(PyObject*p, type<MyPtr<Foo> >)
//     { return smart_ptr_from_python(p, type<MyPtr<Foo> >(), type<Foo>());}
// }
// 
// MyPtr<Bar> from_python(PyObject*p, type<MyPtr<Bar> >)
//     { return smart_ptr_from_python(p, type<MyPtr<Bar> >(), type<Bar>());}
// 
// ... // definitions for MyPtr<Baz>, MyPtr<Mumble>, etc.
//
// #else
//
// // Just once for all MyPtr<T>
// template <class T>
// MyPtr<T> from_python(PyObject*p, type<MyPtr<T> >)
// {
//    return smart_ptr_from_python(p, type<MyPtr<T> >(), type<T>());
// }
//
// #endif
// }

#if !defined(BOOST_MSVC6_OR_EARLIER)
template <class T>
boost::shared_ptr<T> from_python(PyObject*p, python::type<boost::shared_ptr<T> >)
{
    return smart_ptr_from_python(p, python::type<boost::shared_ptr<T> >(), python::type<T>());
}
#endif

#if 0
template <class T>
PyObject* to_python(std::auto_ptr<T> p)
{
    return new python::wrapped_pointer<std::auto_ptr<T>, T>(p);
}

template <class T>
PyObject* to_python(boost::shared_ptr<T> p)
{
    return new python::wrapped_pointer<boost::shared_ptr<T>, T>(p);
}
#endif

//
// inline implementations
//

#ifndef BOOST_MSVC6_OR_EARLIER
inline PyObject* to_python(double d)
{
    return PyFloat_FromDouble(d);
}

inline PyObject* to_python(float f)
{
    return PyFloat_FromDouble(f);
}
#endif // BOOST_MSVC6_OR_EARLIER

inline PyObject* to_python(long l)
{
	return PyInt_FromLong(l);
}

inline PyObject* to_python(int x)
{
	return PyInt_FromLong(x);
}

inline PyObject* to_python(short x)
{
	return PyInt_FromLong(x);
}

inline PyObject* to_python(bool b)
{
	return PyInt_FromLong(b);
}

inline PyObject* to_python(void)
{
    return python::detail::none();
}

inline PyObject* to_python(const char* s)
{
	return PyString_FromString(s);
}

inline std::string from_python(PyObject* p, python::type<const std::string&>)
{
    return from_python(p, python::type<std::string>());
}

inline PyObject* to_python(PyObject* p)
{
    Py_INCREF(p);
    return p;
}

inline PyObject* from_python(PyObject* p, python::type<PyObject*>)
{
    return p;
}

inline const char* from_python(PyObject* p, python::type<const char* const&>)
{
    return from_python(p, python::type<const char*>());
}

inline double from_python(PyObject* p, python::type<const double&>)
{
    return from_python(p, python::type<double>());
}

inline float from_python(PyObject* p, python::type<const float&>)
{
    return from_python(p, python::type<float>());
}

inline int from_python(PyObject* p, python::type<const int&>)
{
    return from_python(p, python::type<int>());
}

inline short from_python(PyObject* p, python::type<const short&>)
{
    return from_python(p, python::type<short>());
}

inline long from_python(PyObject* p, python::type<const long&>)
{
    return from_python(p, python::type<long>());
}

inline bool from_python(PyObject* p, python::type<const bool&>)
{
    return from_python(p, python::type<bool>());
}

inline unsigned int from_python(PyObject* p, python::type<const unsigned int&>)
{
    return from_python(p, python::type<unsigned int>());
}

inline unsigned short from_python(PyObject* p, python::type<const unsigned short&>)
{
    return from_python(p, python::type<unsigned short>());
}

inline signed char from_python(PyObject* p, python::type<const signed char&>)
{
    return from_python(p, python::type<signed char>());
}

inline unsigned char from_python(PyObject* p, python::type<const unsigned char&>)
{
    return from_python(p, python::type<unsigned char>());
}

inline unsigned long from_python(PyObject* p, python::type<const unsigned long&>)
{
    return from_python(p, python::type<unsigned long>());
}


BOOST_PYTHON_END_CONVERSION_NAMESPACE

#endif // METHOD_DWA122899_H_
