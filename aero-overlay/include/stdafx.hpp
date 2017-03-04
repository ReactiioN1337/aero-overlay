#pragma once
#ifndef REACTIION_AERO_OVERLAY_HPP
#define REACTIION_AERO_OVERLAY_HPP
#include <Windows.h>


#include <dwmapi.h>
#pragma comment( lib, "dwmapi.lib" )


/// std::arry
#include <array>
/// std::transform, std::find, ...
#include <algorithm>
/// std::wstring_converter
#include <codecvt>
/// std::fstream
#include <fstream>
/// std::cout, std::cin
#include <iostream>
/// std::map
#include <map>
/// std::shared_ptr, std::unique_ptr
#include <memory>
/// std::random_device
#include <random>
/// std::stringstream, stream << operator for strings
#include <sstream>
/// std::tuple
#include <tuple>
/// std::this_thread::sleep_for
#include <thread>
/// std::vector
#include <vector>

#if _MSC_VER < 1910 /// Visual Studio 2017(RC)
namespace std {
///-------------------------------------------------------------------------------------------------
/// Clamp the given in.
///
/// @author ReactiioN
/// @date   04.03.2017
///
/// @tparam T   Generic type parameter.
/// @param  in      The in.
/// @param  mins    The mins.
/// @param  maxs    The maxs.
///
/// @return A reference to a const T.
///-------------------------------------------------------------------------------------------------
#if _MSC_VER == 1900 /// Visual Studio 2015
template<typename T>
constexpr const T& clamp( const T& in, const T& mins, const T& maxs )
{
    static_assert( std::is_arithmetic<T>::value, "Type T has to be arithmetic" );
    return std::min( std::max( in, mins ), maxs );
}
#else /// Visual Studio 2013 or older, upgrade your vs bro
template<typename T>
const T& clamp( const T& in, const T& mins, const T& maxs )
{
    static_assert( std::is_arithmetic<T>::value, "Type T has to be arithmetic" );
    return std::min( std::max( in, mins ), maxs );
}
#endif
}
#endif

///-------------------------------------------------------------------------------------------------
/// Safe release.
///
/// @author ReactiioN
/// @date   04.03.2017
///
/// @tparam T   Generic type parameter.
/// @param [in,out] interface_to_release    If non-null, the interface to release.
///-------------------------------------------------------------------------------------------------
template<typename T>
void safe_release( T** interface_to_release )
{
    if( interface_to_release && *interface_to_release ) {
        ( *interface_to_release )->Release();
        ( *interface_to_release ) = nullptr;
    }
}

///-------------------------------------------------------------------------------------------------
/// A macro that defines Destructor execute virtual.
///
/// @author ReactiioN
/// @date   04.03.2017
///
/// @param  FN  The function.
///-------------------------------------------------------------------------------------------------
#define DTOR_EXECUTE_VIRTUAL( FN ) [this]{ \
        FN(); \
    }

#endif
