//
// gsl-lite is based on GSL: Guideline Support Library,
// https://github.com/microsoft/gsl
//
// Copyright (c) 2015 Martin Moene
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#ifndef GSL_GSL_LITE_H_INCLUDED
#define GSL_GSL_LITE_H_INCLUDED

#include <exception>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define  gsl_lite_VERSION "0.13.0"

// gsl-lite backward compatibility:

#ifdef gsl_CONFIG_ALLOWS_SPAN_CONTAINER_CTOR
# define gsl_CONFIG_ALLOWS_UNCONSTRAINED_SPAN_CONTAINER_CTOR  gsl_CONFIG_ALLOWS_SPAN_CONTAINER_CTOR
# pragma message ("gsl_CONFIG_ALLOWS_SPAN_CONTAINER_CTOR is deprecated since gsl-lite 0.7.0; replace with gsl_CONFIG_ALLOWS_UNCONSTRAINED_SPAN_CONTAINER_CTOR, or consider span(with_container, cont).")
#endif

// M-GSL compatibility:

#if defined( GSL_THROW_ON_CONTRACT_VIOLATION )
# define gsl_CONFIG_CONTRACT_VIOLATION_THROWS  1
#endif

#if defined( GSL_TERMINATE_ON_CONTRACT_VIOLATION )
# define gsl_CONFIG_CONTRACT_VIOLATION_THROWS  0
#endif

#if defined( GSL_UNENFORCED_ON_CONTRACT_VIOLATION )
# define gsl_CONFIG_CONTRACT_LEVEL_OFF  1
#endif

// Configuration:

#ifndef  gsl_FEATURE_HAVE_IMPLICIT_MACRO
# define gsl_FEATURE_HAVE_IMPLICIT_MACRO  1
#endif

#ifndef  gsl_FEATURE_HAVE_OWNER_MACRO
# define gsl_FEATURE_HAVE_OWNER_MACRO  1
#endif

#ifndef  gsl_FEATURE_EXPERIMENTAL_RETURN_GUARD
# define gsl_FEATURE_EXPERIMENTAL_RETURN_GUARD  0
#endif

#ifndef  gsl_CONFIG_CONFIRMS_COMPILATION_ERRORS
# define gsl_CONFIG_CONFIRMS_COMPILATION_ERRORS  0
#endif

#ifndef  gsl_CONFIG_ALLOWS_NONSTRICT_SPAN_COMPARISON
# define gsl_CONFIG_ALLOWS_NONSTRICT_SPAN_COMPARISON  1
#endif

#ifndef  gsl_CONFIG_ALLOWS_UNCONSTRAINED_SPAN_CONTAINER_CTOR
# define gsl_CONFIG_ALLOWS_UNCONSTRAINED_SPAN_CONTAINER_CTOR  1
#endif

#if    defined( gsl_CONFIG_CONTRACT_LEVEL_ON )
# define        gsl_CONFIG_CONTRACT_LEVEL_MASK  0x11
#elif  defined( gsl_CONFIG_CONTRACT_LEVEL_OFF )
# define        gsl_CONFIG_CONTRACT_LEVEL_MASK  0x00
#elif  defined( gsl_CONFIG_CONTRACT_LEVEL_EXPECTS_ONLY )
# define        gsl_CONFIG_CONTRACT_LEVEL_MASK  0x01
#elif  defined( gsl_CONFIG_CONTRACT_LEVEL_ENSURES_ONLY )
# define        gsl_CONFIG_CONTRACT_LEVEL_MASK  0x10
#else
# define        gsl_CONFIG_CONTRACT_LEVEL_MASK  0x11
#endif

#if   !defined( gsl_CONFIG_CONTRACT_VIOLATION_THROWS     ) && \
      !defined( gsl_CONFIG_CONTRACT_VIOLATION_TERMINATES )
# define        gsl_CONFIG_CONTRACT_VIOLATION_THROWS_V 0
#elif  defined( gsl_CONFIG_CONTRACT_VIOLATION_THROWS     ) && \
      !defined( gsl_CONFIG_CONTRACT_VIOLATION_TERMINATES )
# define        gsl_CONFIG_CONTRACT_VIOLATION_THROWS_V 1
#elif !defined( gsl_CONFIG_CONTRACT_VIOLATION_THROWS     ) && \
       defined( gsl_CONFIG_CONTRACT_VIOLATION_TERMINATES )
# define        gsl_CONFIG_CONTRACT_VIOLATION_THROWS_V 0
#else
# error only one of gsl_CONFIG_CONTRACT_VIOLATION_THROWS and gsl_CONFIG_CONTRACT_VIOLATION_TERMINATES may be defined.
#endif

// Compiler detection (C++17 is speculative):

#define gsl_CPP11_OR_GREATER  ( __cplusplus >= 201103L )
#define gsl_CPP14_OR_GREATER  ( __cplusplus >= 201402L )
#define gsl_CPP17_OR_GREATER  ( __cplusplus >= 201700L )

// half-open range [lo..hi):
#define gsl_BETWEEN( v, lo, hi ) ( lo <= v && v < hi )

#if defined(_MSC_VER) && !defined(__clang__)
# define gsl_COMPILER_MSVC_VERSION   (_MSC_VER / 100 - 5 - (_MSC_VER < 1900))
#else
# define gsl_COMPILER_MSVC_VERSION   0
# define gsl_COMPILER_NON_MSVC       1
#endif

// Note: simplistic version computation; works for GCC versions on http://godbolt.org/
#if defined(__GNUC__) && !defined(__clang__)
# define gsl_COMPILER_GCC_VERSION  ( 10 * (10 *__GNUC__ + __GNUC_MINOR__) + __GNUC_PATCHLEVEL__)
#else
# define gsl_COMPILER_GCC_VERSION  0
#endif

// Presence of C++11 language features:

#if gsl_CPP11_OR_GREATER || gsl_COMPILER_MSVC_VERSION >= 10
# define gsl_HAVE_AUTO  1
# define gsl_HAVE_NULLPTR  1
#endif

#if gsl_CPP11_OR_GREATER || gsl_COMPILER_MSVC_VERSION >= 12
# define gsl_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG  1
#endif

#if gsl_CPP11_OR_GREATER || gsl_COMPILER_MSVC_VERSION >= 14
# define gsl_HAVE_ALIAS_TEMPLATE  1
# define gsl_HAVE_CONSTEXPR_11  1
# define gsl_HAVE_ENUM_CLASS  1
# define gsl_HAVE_EXPLICIT_CONVERSION  1
# define gsl_HAVE_INITIALIZER_LIST  1
# define gsl_HAVE_IS_DEFAULT  1
# define gsl_HAVE_IS_DELETE  1
# define gsl_HAVE_NOEXCEPT  1
#endif

#if gsl_CPP11_OR_GREATER
// see above
#endif

// Presence of C++14 language features:

#if gsl_CPP14_OR_GREATER
# define gsl_HAVE_CONSTEXPR_14  1
#endif

// Presence of C++17 language features:

#if gsl_CPP17_OR_GREATER
# define gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE  1
#endif

// Presence of C++ library features:

#if gsl_CPP11_OR_GREATER || gsl_COMPILER_MSVC_VERSION >= 11
# define gsl_HAVE_ARRAY  1
#endif

#if gsl_CPP11_OR_GREATER || gsl_COMPILER_MSVC_VERSION >= 14 || (gsl_COMPILER_MSVC_VERSION >= 9 && _HAS_CPP0X)
# define gsl_HAVE_CONTAINER_DATA_METHOD  1
#endif

#if gsl_CPP11_OR_GREATER || gsl_COMPILER_MSVC_VERSION >= 14
# define gsl_HAVE_SIZED_TYPES  1
#endif

#if gsl_CPP11_OR_GREATER || gsl_COMPILER_MSVC_VERSION >= 14 || (gsl_COMPILER_MSVC_VERSION >= 10 && _HAS_CPP0X)
# define gsl_HAVE_SHARED_PTR  1
# define gsl_HAVE_UNIQUE_PTR  1
#endif

#if gsl_CPP11_OR_GREATER || gsl_COMPILER_MSVC_VERSION >= 11
# define gsl_HAVE_TYPE_TRAITS  1
#endif

// For the rest, consider VC12, VC14 as C++11 for GSL Lite:

#if gsl_COMPILER_MSVC_VERSION >= 12
# undef  gsl_CPP11_OR_GREATER
# define gsl_CPP11_OR_GREATER  1
#endif

// C++ feature usage:

#if gsl_HAVE_CONSTEXPR_11
# define gsl_constexpr constexpr
#else
# define gsl_constexpr /*constexpr*/
#endif

#if gsl_HAVE_CONSTEXPR_14
# define gsl_constexpr14 constexpr
#else
# define gsl_constexpr14 /*constexpr*/
#endif

#if gsl_HAVE_EXPLICIT_CONVERSION
# define gsl_explicit explicit
#else
# define gsl_explicit /*explicit*/
#endif

#if gsl_FEATURE_HAVE_IMPLICIT_MACRO
# define implicit
#endif

#if !gsl_HAVE_NOEXCEPT || gsl_CONFIG_CONTRACT_VIOLATION_THROWS_V
# define gsl_noexcept /*noexcept*/
#else
# define gsl_noexcept noexcept
#endif

#define gsl_DIMENSION_OF( a ) ( sizeof(a) / sizeof(0[a]) )

#if gsl_HAVE_ARRAY
# include <array>
#endif

#if gsl_HAVE_TYPE_TRAITS
# include <type_traits>
#endif

#if gsl_HAVE_SIZED_TYPES
# include <cstdint>
#endif

// Other features:

// Note: !defined(__NVCC__) doesn't work with nvcc here:
#define gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR  \
    ( gsl_CONFIG_ALLOWS_UNCONSTRAINED_SPAN_CONTAINER_CTOR && (__NVCC__== 0) )

// GSL API (e.g. for CUDA platform):

#ifndef   gsl_api
# ifdef   __CUDACC__
#  define gsl_api __host__ __device__
# else
#  define gsl_api /*gsl_api*/
# endif
#endif

namespace gsl {

//
// GSL.owner: ownership pointers
//
#if gsl_HAVE_SHARED_PTR
  using std::unique_ptr;
  using std::shared_ptr;
#endif

#if gsl_HAVE_ALIAS_TEMPLATE
  template< class T > using owner = T;
#else
  template< class T > struct owner { typedef T type; };
#endif

#define gsl_HAVE_OWNER_TEMPLATE  gsl_HAVE_ALIAS_TEMPLATE

#if gsl_FEATURE_HAVE_OWNER_MACRO
# if gsl_HAVE_OWNER_TEMPLATE
#  define Owner(t)  ::gsl::owner<t>
# else
#  define Owner(t)  ::gsl::owner<t>::type
# endif
#endif

//
// GSL.assert: assertions
//

#define gsl_ELIDE_CONTRACT_EXPECTS  ( 0 == ( gsl_CONFIG_CONTRACT_LEVEL_MASK & 0x01 ) )
#define gsl_ELIDE_CONTRACT_ENSURES  ( 0 == ( gsl_CONFIG_CONTRACT_LEVEL_MASK & 0x10 ) )

#if gsl_ELIDE_CONTRACT_EXPECTS
# define Expects( x )  /* Expects elided */
#elif gsl_CONFIG_CONTRACT_VIOLATION_THROWS_V
# define Expects( x )  ::gsl::fail_fast_assert( (x), "GSL: Precondition failure at " __FILE__ ": " gsl_STRINGIFY(__LINE__) );
#else
# define Expects( x )  ::gsl::fail_fast_assert( (x) )
#endif

#if gsl_ELIDE_CONTRACT_ENSURES
# define Ensures( x )  /* Ensures elided */
#elif gsl_CONFIG_CONTRACT_VIOLATION_THROWS_V
# define Ensures( x )  ::gsl::fail_fast_assert( (x), "GSL: Postcondition failure at " __FILE__ ": " gsl_STRINGIFY(__LINE__) );
#else
# define Ensures( x )  ::gsl::fail_fast_assert( (x) )
#endif

#define gsl_STRINGIFY(  x )  gsl_STRINGIFY_( x )
#define gsl_STRINGIFY_( x )  #x

struct fail_fast : public std::runtime_error
{
    gsl_api explicit fail_fast( char const * const message )
    : std::runtime_error( message ) {}
};

#if gsl_CONFIG_CONTRACT_VIOLATION_THROWS_V

gsl_api inline void fail_fast_assert( bool cond, char const * const message )
{
    if ( !cond )
        throw fail_fast( message );
}

#else // gsl_CONFIG_CONTRACT_VIOLATION_THROWS_V

gsl_api inline void fail_fast_assert( bool cond )
{
    if ( !cond )
        std::terminate();
}

#endif // gsl_CONFIG_CONTRACT_VIOLATION_THROWS_V

//
// GSL.util: utilities
//

#if gsl_CPP11_OR_GREATER

template< class F >
class final_act
{
public:
    gsl_api explicit final_act( F action ) gsl_noexcept
        : action_( std::move( action ) )
        , invoke_( true ) 
    {}

    gsl_api final_act( final_act && other ) gsl_noexcept
        : action_( std::move( other.action_ ) )
        , invoke_( other.invoke_ ) 
    {
        other.invoke_ = false;
    }

    gsl_api final_act( final_act const  & ) = delete;
    gsl_api final_act & operator=( final_act const & ) = delete;

    gsl_api virtual ~final_act() gsl_noexcept 
    { 
        if ( invoke_ ) 
            action_(); 
    }

protected:
    gsl_api void dismiss() gsl_noexcept
    {
        invoke_ = false;
    }
    
#if gsl_CPP17_OR_GREATER
    gsl_api int uncaught_exceptions() gsl_noexcept
    {
        return std::uncaught_exceptions();
    }
#else
    gsl_api int uncaught_exceptions()
    {
        return std::uncaught_exception() ? 1 : 0;
    }
#endif

private:
    F action_;
    bool invoke_;
};

template< class F >
gsl_api final_act<F> finally( F const & action ) gsl_noexcept
{
    return final_act<F>( action );
}

template< class F >
gsl_api final_act<F> finally( F && action ) gsl_noexcept
{
    return final_act<F>( std::forward<F>( action ) );
}

#if gsl_FEATURE_EXPERIMENTAL_RETURN_GUARD

template< class F >
class final_act_return : public final_act<F>
{
public:
    explicit final_act_return( F && action ) gsl_noexcept
        : final_act<F>( std::move( action ) )
    {}

    final_act_return( final_act_return && other ) gsl_noexcept 
        : final_act<F>( std::move( other ) )
    {}

    final_act_return( final_act_return const & ) = delete;
    final_act_return & operator=( final_act_return const & ) = delete;

    ~final_act_return() override
    {
        if ( this->uncaught_exceptions() )
            this->dismiss();
    }
};

template< class F >
inline final_act_return<F> on_return( F const & action ) gsl_noexcept
{
    return final_act_return<F>( action );
}

template< class F >
inline final_act_return<F> on_return( F && action ) gsl_noexcept
{
    return final_act_return<F>( std::forward<F>( action ) );
}

template< class F >
class final_act_error : public final_act<F>
{
public:
    explicit final_act_error( F && action ) gsl_noexcept
        : final_act<F>( std::move( action ) )
    {}

    final_act_error( final_act_error && other ) gsl_noexcept
        : final_act<F>( std::move( other ) )
    {}

    final_act_error( final_act_error const & ) = delete;
    final_act_error & operator=( final_act_error const & ) = delete;

    ~final_act_error() override
    {
        if ( ! this->uncaught_exceptions() )
            this->dismiss();
    }
};

template< class F >
inline final_act_error<F> on_error( F const & action ) gsl_noexcept
{
    return final_act_error<F>( action );
}

template< class F >
inline final_act_error<F> on_error( F && action ) gsl_noexcept
{
    return final_act_error<F>( std::forward<F>( action ) );
}

#endif // gsl_FEATURE_EXPERIMENTAL_RETURN_GUARD

#else // gsl_CPP11_OR_GREATER

class final_act
{
public:
    typedef void (*Action)();

    gsl_api final_act( Action action )
    : action_( action ) 
    , invoke_( true )
    {}

    gsl_api final_act( final_act const & other )
        : action_( other.action_ )
        , invoke_( other.invoke_ ) 
    {
        other.invoke_ = false;
    }

    gsl_api virtual ~final_act()
    {
        if ( invoke_ ) 
            action_();
    }

protected:
    gsl_api void dismiss()
    {
        invoke_ = false;
    }
    
    gsl_api int uncaught_exceptions()
    {
        return std::uncaught_exception() ? 1 : 0;
    }

private:
    gsl_api final_act & operator=( final_act const & );
    
private:
    Action action_;
    mutable bool invoke_;
};

template< class F >
gsl_api final_act finally( F const & f )
{
    return final_act(( f ));
}

#if gsl_FEATURE_EXPERIMENTAL_RETURN_GUARD

class final_act_return : public final_act
{
public:
    explicit final_act_return( Action action ) 
        : final_act( action )
    {}

    ~final_act_return()
    {
        if ( this->uncaught_exceptions() )
            this->dismiss();
    }

private:
    final_act_return & operator=( final_act_return const & );
};

template< class F >
inline final_act_return on_return( F const & action )
{
    return final_act_return( action );
}

class final_act_error : public final_act
{
public:
    explicit final_act_error( Action action ) 
        : final_act( action )
    {}

    ~final_act_error()
    {
        if ( ! this->uncaught_exceptions() )
            this->dismiss();
    }

private:
    final_act_error & operator=( final_act_error const & );
};

template< class F >
inline final_act_error on_error( F const & action )
{
    return final_act_error( action );
}

#endif // gsl_FEATURE_EXPERIMENTAL_RETURN_GUARD

#endif // gsl_CPP11_OR_GREATER

template< class T, class U >
gsl_api T narrow_cast( U u ) gsl_noexcept
{
    return static_cast<T>( u );
}

struct narrowing_error : public std::exception {};

#if gsl_HAVE_TYPE_TRAITS

namespace details
{
    template< class T, class U >
    struct is_same_signedness : public std::integral_constant<bool, std::is_signed<T>::value == std::is_signed<U>::value>
    {};
}
#endif

template< class T, class U >
gsl_api T narrow( U u )
{
    T t = narrow_cast<T>( u );

    if ( static_cast<U>( t ) != u )
    {
        throw narrowing_error();
    }

#if gsl_HAVE_TYPE_TRAITS
    if ( ! details::is_same_signedness<T, U>::value && ( ( t < T() ) != ( u < U() ) ) )
#else
    // Don't assume T() works:
    if ( ( t < 0 ) != ( u < 0 ) )
#endif
    {
        throw narrowing_error();
    }
    return t;
}

//
// at() - Bounds-checked way of accessing static arrays, std::array, std::vector.
//

template< class T, size_t N >
gsl_api T & at( T(&arr)[N], size_t index )
{
    Expects( index < N );
    return arr[index];
}

#if gsl_HAVE_ARRAY

template< class T, size_t N >
gsl_api T & at( std::array<T, N> & arr, size_t index )
{
    Expects( index < N );
    return arr[index];
}
#endif

template< class Cont >
gsl_api typename Cont::value_type & at( Cont & cont, size_t index )
{
    Expects( index < cont.size() );
    return cont[index];
}

#if gsl_HAVE_INITIALIZER_LIST

template< class T >
gsl_api const T & at( std::initializer_list<T> cont, size_t index )
{
    Expects( index < cont.size() );
    return *( cont.begin() + index );
}
#endif

template< class T >
class span;

template< class T >
gsl_api T & at( span<T> s, size_t index )
{
    return s.at( index );
}

//
// GSL.views: views
//

//
// not_null<> - Wrap any indirection and enforce non-null.
//
template<class T>
class not_null
{
public:
    gsl_api not_null(             T t         ) : ptr_ ( t ){ Expects( ptr_ != NULL ); }
    gsl_api not_null & operator=( T const & t ) { ptr_ = t ;  Expects( ptr_ != NULL ); return *this; }

    gsl_api not_null(             not_null const & other ) : ptr_ ( other.ptr_  ) {}
    gsl_api not_null & operator=( not_null const & other ) { ptr_ = other.ptr_; return *this; }

#if gsl_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG

    template< typename U, typename Dummy =
        typename std::enable_if<std::is_convertible<U, T>::value, void>::type >
    gsl_api not_null( not_null<U> const & other ) : ptr_( other.get() ) {}

    template< typename U, typename Dummy =
        typename std::enable_if<std::is_convertible<U, T>::value, void>::type >
    gsl_api not_null & operator=( not_null<U> const & other )
    {
        ptr_ = other.get();
        return *this;
    }

#else

    template< typename U >
    gsl_api not_null( not_null<U> const & other ) : ptr_( other.get() ) {}

    template< typename U >
    gsl_api not_null & operator=( not_null<U> const & other )
    {
        ptr_ = other.get();
        return *this;
    }
#endif

private:
    // Prevent compilation when initialized with a nullptr or literal 0:
#if gsl_HAVE_NULLPTR
    gsl_api not_null(             std::nullptr_t );
    gsl_api not_null & operator=( std::nullptr_t );
#endif
    gsl_api not_null(             int );
    gsl_api not_null & operator=( int );

public:
    gsl_api T get() const
    {
#if gsl_HAVE_NULLPTR && gsl_COMPILER_MSVC_VERSION > 0
        // The assume() should help the optimizer:
        __assume( ptr_ != nullptr );
#endif
        return ptr_;
    }

    gsl_api      operator T() const { return get(); }
    gsl_api T    operator->() const { return get(); }

    gsl_api bool operator==(T const & rhs) const { return    ptr_ == rhs; }
    gsl_api bool operator!=(T const & rhs) const { return !(*this == rhs); }

private:
    T ptr_;

    gsl_api not_null & operator++();
    gsl_api not_null & operator--();
    gsl_api not_null   operator++( int );
    gsl_api not_null   operator--( int );
    gsl_api not_null & operator+ ( size_t );
    gsl_api not_null & operator+=( size_t );
    gsl_api not_null & operator- ( size_t );
    gsl_api not_null & operator-=( size_t );
};

//
// Byte-specific type.
//
#if gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
  enum class byte : unsigned char {};
#else
  struct byte { typedef unsigned char type; type v; };
#endif

#if gsl_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG
# define gsl_ENABLE_IF_INTEGRAL_T(T)  \
    , class = typename std::enable_if<std::is_integral<T>::value>::type
#else
# define gsl_ENABLE_IF_INTEGRAL_T(T)
#endif

template< class T >
gsl_api inline gsl_constexpr byte to_byte( T v ) gsl_noexcept
{
#if    gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    static_assert( false, "Implement to_byte() using conversion from underlying type." );
#elif  gsl_HAVE_CONSTEXPR_11
    return { static_cast<typename byte::type>( v ) };
#else
    byte b = { static_cast<typename byte::type>( v ) }; return b;
#endif
}

template< class IntegerType  gsl_ENABLE_IF_INTEGRAL_T( IntegerType ) >
gsl_api inline gsl_constexpr IntegerType to_integer( byte b ) gsl_noexcept
{
#if gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    static_assert( false, "Implement to_integer() using conversion from underlying type." );
#else
    return b.v;
#endif
}

gsl_api inline gsl_constexpr unsigned char to_uchar( byte b ) gsl_noexcept
{
    return to_integer<unsigned char>( b );
}

#if ! gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE

inline gsl_constexpr bool operator==( byte l, byte r ) gsl_noexcept
{
    return l.v == r.v;
}

inline gsl_constexpr bool operator!=( byte l, byte r ) gsl_noexcept
{
    return !( l == r );
}

inline gsl_constexpr bool operator< ( byte l, byte r ) gsl_noexcept
{
    return l.v < r.v;
}

inline gsl_constexpr bool operator<=( byte l, byte r ) gsl_noexcept
{
    return !( r < l );
}

inline gsl_constexpr bool operator> ( byte l, byte r ) gsl_noexcept
{
    return ( r < l );
}

inline gsl_constexpr bool operator>=( byte l, byte r ) gsl_noexcept
{
    return !( l < r );
}
#endif

template< class IntegerType  gsl_ENABLE_IF_INTEGRAL_T( IntegerType ) >
gsl_api inline gsl_constexpr14 byte & operator<<=( byte & b, IntegerType shift ) gsl_noexcept
{
#if gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    return b = to_byte( to_uchar( b ) << shift );
#else
    b.v = to_uchar( b ) << shift; return b;
#endif
}

template< class IntegerType  gsl_ENABLE_IF_INTEGRAL_T( IntegerType ) >
gsl_api inline gsl_constexpr byte operator<<( byte b, IntegerType shift ) gsl_noexcept
{
    return to_byte( to_uchar( b ) << shift );
}

template< class IntegerType  gsl_ENABLE_IF_INTEGRAL_T( IntegerType ) >
gsl_api inline gsl_constexpr14 byte & operator>>=( byte & b, IntegerType shift ) gsl_noexcept
{
#if gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    return b = to_byte( to_uchar( b ) >> shift );
#else
    b.v = to_uchar( b ) >> shift; return b;
#endif
}

template< class IntegerType  gsl_ENABLE_IF_INTEGRAL_T( IntegerType ) >
gsl_api inline gsl_constexpr byte operator>>( byte b, IntegerType shift ) gsl_noexcept
{
    return to_byte( to_uchar( b ) >> shift );
}

gsl_api inline gsl_constexpr14 byte & operator|=( byte & l, byte r ) gsl_noexcept
{
#if gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    return l = to_byte( to_uchar( l ) | to_uchar( r ) );
#else
    l.v = to_uchar( l ) | to_uchar( r ); return l;
#endif
}

gsl_api inline gsl_constexpr byte operator|( byte l, byte r ) gsl_noexcept
{
    return to_byte( to_uchar( l ) | to_uchar( r ) );
}

gsl_api inline gsl_constexpr14 byte & operator&=( byte & l, byte r ) gsl_noexcept
{
#if gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    return l = to_byte( to_uchar( l ) & to_uchar( r ) );
#else
    l.v = to_uchar( l ) & to_uchar( r ); return l;
#endif
}

gsl_api inline gsl_constexpr byte operator&( byte l, byte r ) gsl_noexcept
{
    return to_byte( to_uchar( l ) & to_uchar( r ) );
}

gsl_api inline gsl_constexpr14 byte & operator^=( byte & l, byte r ) gsl_noexcept
{
#if gsl_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    return l = to_byte( to_uchar( l ) ^ to_uchar (r ) );
#else
    l.v = to_uchar( l ) ^ to_uchar (r ); return l;
#endif
}

gsl_api inline gsl_constexpr byte operator^( byte l, byte r ) gsl_noexcept
{
    return to_byte( to_uchar( l ) ^ to_uchar( r ) );
}

gsl_api inline gsl_constexpr byte operator~( byte b ) gsl_noexcept 
{ 
    return to_byte( ~to_uchar( b ) ); 
}

// tag to select span constructor taking a container
struct with_container_t{ gsl_constexpr14 with_container_t(){} };
const with_container_t with_container;

//
// span<> - A 1D view of contiguous T's, replace (*,len).
//
template< class T >
class span
{
    template< class U > friend class span;

public:
    typedef size_t size_type;       // deprecated since version 0.12.0
    typedef size_t index_type;      // p0122r3 uses std::ptrdiff_t

    typedef T element_type;
    typedef T & reference;
    typedef T * pointer;
    typedef T const * const_pointer;
    typedef T const & const_reference;

    typedef pointer       iterator;
    typedef const_pointer const_iterator;

#if gsl_BETWEEN( gsl_COMPILER_MSVC_VERSION, 6, 7 )
    typedef std::reverse_iterator< iterator, T >             reverse_iterator;
    typedef std::reverse_iterator< const_iterator, const T > const_reverse_iterator;
#else
    typedef std::reverse_iterator< iterator >                reverse_iterator;
    typedef std::reverse_iterator< const_iterator >          const_reverse_iterator;
#endif

    typedef typename std::iterator_traits< iterator >::difference_type difference_type;

    gsl_api gsl_constexpr14 span()
        : begin_( NULL )
        , end_  ( NULL )
    {
        Expects( size() == 0 );
    }

#if gsl_HAVE_NULLPTR
    gsl_api gsl_constexpr14 span( std::nullptr_t, size_type size_in )
        : begin_( nullptr )
        , end_  ( nullptr )
    {
        Expects( size_in == 0 );
    }
#endif

#if gsl_HAVE_IS_DELETE
    gsl_api gsl_constexpr14 span( reference data_in )
        : span( &data_in, 1 )
    {}

    gsl_api gsl_constexpr14 span( element_type && ) = delete;
#endif

    gsl_api gsl_constexpr14 span( pointer begin_in, pointer end_in )
        : begin_( begin_in )
        , end_  ( end_in )
    {
        Expects( begin_in <= end_in );
    }

    gsl_api gsl_constexpr14 span( pointer data_in, size_type size_in )
        : begin_( data_in )
        , end_  ( data_in + size_in )
    {
        Expects( size_in == 0 || ( size_in > 0 && data_in != NULL ) );
    }

    template< typename U >
    gsl_api gsl_constexpr14 span( U * & data_in, size_type size_in )
        : begin_( data_in )
        , end_  ( data_in + size_in )
    {
        Expects( size_in == 0 || ( size_in > 0 && data_in != NULL ) );
    }

    template< typename U >
    gsl_api gsl_constexpr14 span( U * const & data_in, size_type size_in )
        : begin_( data_in )
        , end_  ( data_in + size_in )
    {
        Expects( size_in == 0 || ( size_in > 0 && data_in != NULL ) );
    }

    template< class U, size_t N >
    gsl_api gsl_constexpr14 span( U (&arr)[N] )
        : begin_( arr )
        , end_  ( arr + N )
    {}

#if gsl_HAVE_ARRAY
    template< class U, size_t N >
    gsl_api gsl_constexpr14 span( std::array< U, N > & arr )
        : begin_( arr.data() )
        , end_  ( arr.data() + N )
    {}
#endif

#if gsl_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG && gsl_HAVE_CONTAINER_DATA_METHOD
# define gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR   1
#endif

#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR
    // SFINAE enable only if Cont has a data() member function
    template< class Cont, typename = decltype(std::declval<Cont>().data()) >
    gsl_api gsl_constexpr14 span( Cont & cont )
        : begin_( cont.data() )
        , end_  ( cont.data() + cont.size() )
    {}
#elif gsl_HAVE_UNCONSTRAINED_SPAN_CONTAINER_CTOR
    template< class Cont >
    gsl_api gsl_constexpr14 span( Cont & cont )
        : begin_( cont.size() == 0 ? NULL : &cont[0] )
        , end_  ( cont.size() == 0 ? NULL : &cont[0] + cont.size() )
    {}
#endif

    template< class Cont >
    gsl_api gsl_constexpr14 span( with_container_t, Cont & cont )
        : begin_( cont.size() == 0 ? NULL : &cont[0] )
        , end_  ( cont.size() == 0 ? NULL : &cont[0] + cont.size() )
    {}

#if gsl_HAVE_IS_DEFAULT
    gsl_api gsl_constexpr14 span( span && ) = default;
    gsl_api gsl_constexpr14 span( span const & ) = default;
#else
    gsl_api gsl_constexpr14 span( span const & other )
        : begin_( other.begin() )
        , end_  ( other.end() )
    {}
#endif

    template< typename U >
    gsl_api gsl_constexpr14 span( span<U> const & other )
        : begin_( other.begin() )
        , end_  ( other.end() )
    {}

#if gsl_HAVE_IS_DEFAULT
    gsl_api span & operator=( span && ) = default;
    gsl_api span & operator=( span const & ) = default;
#else
    gsl_api span & operator=( span other )
    {
        other.swap( *this );
        return *this;
    }
#endif

#if 0
    // Converting from other span ?
    template< typename U > operator=();
#endif

    gsl_api gsl_constexpr14 span first( size_type count ) const gsl_noexcept
    {
        Expects( count <= this->size() );
        return span( this->data(), count );
    }

    gsl_api gsl_constexpr14 span last( size_type count ) const gsl_noexcept
    {
        Expects( count <= this->size() );
        return span( this->data() + this->size() - count, count );
    }

    gsl_api gsl_constexpr14 span subspan( size_type offset ) const gsl_noexcept
    {
        Expects( offset <= this->size() );
        return span( this->data() + offset, this->length() - offset ); 
    }
	
    gsl_api gsl_constexpr14 span subspan( size_type offset, size_type count ) const gsl_noexcept
    {
        Expects( offset <= this->size() && count <= this->size() - offset );
        return span( this->data() + offset, count ); 
    }
	
    gsl_api gsl_constexpr14 iterator begin() const gsl_noexcept
    {
        return iterator( begin_ );
    }

    gsl_api gsl_constexpr14 iterator end() const gsl_noexcept
    {
        return iterator( end_ );
    }

    gsl_api gsl_constexpr14 const_iterator cbegin() const gsl_noexcept
    {
        return const_iterator( begin() );
    }

    gsl_api gsl_constexpr14 const_iterator cend() const gsl_noexcept
    {
        return const_iterator( end() );
    }

    gsl_api gsl_constexpr14 reverse_iterator rbegin() const gsl_noexcept
    {
        return reverse_iterator( end() );
    }

    gsl_api gsl_constexpr14 reverse_iterator rend() const gsl_noexcept
    {
        return reverse_iterator( begin() );
    }

    gsl_api gsl_constexpr14 const_reverse_iterator crbegin() const gsl_noexcept
    {
        return const_reverse_iterator( cend() );
    }

    gsl_api gsl_constexpr14 const_reverse_iterator crend() const gsl_noexcept
    {
        return const_reverse_iterator( cbegin() );
    }

    gsl_api gsl_constexpr14 reference operator[]( size_type index ) const
    {
       return at( index );
    }

    gsl_api gsl_constexpr14 reference operator()( size_type index ) const
    {
       return at( index );
    }

    gsl_api gsl_constexpr14 reference at( size_type index ) const
    {
       Expects( index < size() );
       return begin_[ index ];
    }

    gsl_api gsl_constexpr14 pointer data() const gsl_noexcept
    {
        return begin_;
    }

    gsl_api gsl_constexpr14 bool empty() const gsl_noexcept
    {
        return size() == 0;
    }

    gsl_api gsl_constexpr14 size_type size() const gsl_noexcept
    {
        return end_ - begin_;
    }

    gsl_api gsl_constexpr14 size_type length() const gsl_noexcept
    {
        return size();
    }

    gsl_api gsl_constexpr14 size_type used_length() const gsl_noexcept
    {
        return length();
    }

    gsl_api gsl_constexpr14 size_type bytes() const gsl_noexcept
    {
        return sizeof( element_type ) * size();
    }

    gsl_api gsl_constexpr14 size_type used_bytes() const gsl_noexcept
    {
        return bytes();
    }

    gsl_api void swap( span & other ) gsl_noexcept
    {
        using std::swap;
        swap( begin_, other.begin_ );
        swap( end_  , other.end_   );
    }

    gsl_api span< const byte > as_bytes() const gsl_noexcept
    {
        return span< const byte >( reinterpret_cast<const byte *>( data() ), bytes() );
    }

    gsl_api span< byte > as_writeable_bytes() const gsl_noexcept
    {
        return span< byte >( reinterpret_cast<byte *>( data() ), bytes() );
    }

    template< typename U >
    gsl_api span< U > as_span() const gsl_noexcept
    {
        Expects( ( this->bytes() % sizeof(U) ) == 0 );
        return span< U >( reinterpret_cast<U *>( this->data() ), this->bytes() / sizeof( U ) );
    }

private:
    pointer begin_;
    pointer end_;
};

// span comparison functions

#if gsl_CONFIG_ALLOWS_NONSTRICT_SPAN_COMPARISON

template< class T, class U >
gsl_api gsl_constexpr14 bool operator==( span<T> const & l, span<U> const & r )
{
    return  l.size()  == r.size()
        && (l.begin() == r.begin() || std::equal( l.begin(), l.end(), r.begin() ) );
}

template< class T, class U >
gsl_api gsl_constexpr14 bool operator< ( span<T> const & l, span<U> const & r )
{
    return std::lexicographical_compare( l.begin(), l.end(), r.begin(), r.end() );
}

#else

template< class T >
gsl_api gsl_constexpr14 bool operator==( span<T> const & l, span<T> const & r )
{
    return  l.size()  == r.size()
        && (l.begin() == r.begin() || std::equal( l.begin(), l.end(), r.begin() ) );
}

template< class T >
gsl_api gsl_constexpr14 bool operator< ( span<T> const & l, span<T> const & r )
{
    return std::lexicographical_compare( l.begin(), l.end(), r.begin(), r.end() );
}
#endif

template< class T, class U >
gsl_api gsl_constexpr14 bool operator!=( span<T> const & l, span<U> const & r )
{
    return !( l == r );
}

template< class T, class U >
gsl_api gsl_constexpr14 bool operator<=( span<T> const & l, span<U> const & r )
{
    return !( r < l );
}

template< class T, class U >
gsl_api gsl_constexpr14 bool operator> ( span<T> const & l, span<U> const & r )
{
    return ( r < l );
}

template< class T, class U >
gsl_api gsl_constexpr14 bool operator>=( span<T> const & l, span<U> const & r )
{
    return !( l < r );
}

// span algorithms

namespace detail {

template< class II, class N, class OI >
OI copy_n( II first, N count, OI result )
{
    if ( count > 0 ) 
    {
        *result++ = *first;
        for ( N i = 1; i < count; ++i ) 
        {
            *result++ = *++first;
        }
    }
    return result;
}
}

template< class T, class U >
void copy( span<T> src, span<U> dest )
{
#if gsl_CPP14_OR_GREATER // gsl_HAVE_TYPE_TRAITS (circumvent Travis clang 3.4)
    static_assert( std::is_assignable<U &, T const &>::value, "Cannot assign elements of source span to elements of destination span" );
#endif
    Expects( dest.size() >= src.size() );
    detail::copy_n( src.data(), src.size(), dest.data() );
}

// span creator functions (see ctors)

template< typename T >
gsl_api gsl_constexpr14 span<T> as_span( T * begin, T * end )
{
    return span<T>( begin, end );
}

template< typename T >
gsl_api gsl_constexpr14 span<T> as_span( T * begin, size_t size )
{
    return span<T>( begin, size );
}

template< typename T, size_t N >
gsl_api gsl_constexpr14 span<T> as_span( T (&arr)[N] )
{
    return span<T>( arr, N );
}

#if gsl_HAVE_ARRAY

template< typename T, size_t N >
gsl_api gsl_constexpr14 span<T> as_span( std::array<T,N> & arr )
{
    return span<T>( arr );
}

template< typename T, size_t N >
gsl_api gsl_constexpr14 span<const T> as_span( std::array<T,N> const & arr )
{
    return span<const T>( arr );
}
#endif

#if gsl_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR && gsl_HAVE_AUTO

template< class Cont >
gsl_api gsl_constexpr14 auto as_span( Cont & cont ) -> span< typename Cont::value_type >
{
    return span< typename Cont::value_type >( cont );
}

template< class Cont >
gsl_api gsl_constexpr14 auto as_span( Cont const & cont ) -> span< const typename Cont::value_type >
{
    return span< const typename Cont::value_type >( cont );
}

#else

template< class T >
gsl_api span<T> as_span( std::vector<T> & cont )
{
    return span<T>( with_container, cont );
}

template< class T >
gsl_api span<const T> as_span( std::vector<T> const & cont )
{
    return span<const T>( with_container, cont );
}
#endif

//
// String types:
//

typedef char * zstring;
typedef wchar_t * zwstring;
typedef const char * czstring;
typedef const wchar_t * cwzstring;

typedef span< char > string_span;
typedef span< wchar_t > wstring_span;
typedef span< const char > cstring_span;
typedef span< const wchar_t > cwstring_span;

// to_string() allow (explicit) conversions from string_span to string

gsl_api inline std::string to_string( string_span const & view )
{
    return std::string( view.data(), view.length() );
}

gsl_api inline std::string to_string( cstring_span const & view )
{
    return std::string( view.data(), view.length() );
}

gsl_api inline std::wstring to_string( wstring_span const & view )
{
    return std::wstring( view.data(), view.length() );
}

gsl_api inline std::wstring to_string( cwstring_span const & view )
{
    return std::wstring( view.data(), view.length() );
}

//
// ensure_sentinel()
//
// Provides a way to obtain a span from a contiguous sequence
// that ends with a (non-inclusive) sentinel value.
//
// Will fail-fast if sentinel cannot be found before max elements are examined.
//
namespace detail {

template<class T, class SizeType, const T Sentinel>
gsl_api static span<T> ensure_sentinel( T * seq, SizeType max = std::numeric_limits<SizeType>::max() )
{
    typedef T * pointer;

    pointer cur = seq;

    while ( static_cast<SizeType>( cur - seq ) < max && *cur != Sentinel )
        ++cur;

    Expects( *cur == Sentinel );

    return span<T>( seq, cur - seq );
}
} // namespace detail

//
// ensure_z - creates a string_span for a czstring or cwzstring.
// Will fail fast if a null-terminator cannot be found before
// the limit of size_type.
//

template< class T >
gsl_api inline span<T> ensure_z( T * const & sz, size_t max = std::numeric_limits<size_t>::max() )
{
    return detail::ensure_sentinel<T, size_t, 0>( sz, max );
}

template< class T, size_t N >
gsl_api span<T> ensure_z( T (&sz)[N] )
{
    return ensure_z( &sz[0], N );
}

# if gsl_HAVE_TYPE_TRAITS

template< class Cont >
gsl_api span< typename std::remove_pointer<typename Cont::pointer>::type >
ensure_z( Cont& cont )
{
    return ensure_z( cont.data(), cont.length() );
}
# endif

} // namespace gsl

#endif // GSL_GSL_LITE_H_INCLUDED

// end of file
