#pragma once
#ifndef REACTIION_AERO_OVERLAY_HPP
#define REACTIION_AERO_OVERLAY_HPP
#pragma warning( disable : 4005 )

#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>

#pragma comment( lib, "dwmapi.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )

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

#if _MSC_VER >= 1900
#define STATIC_VAR( T, NAME, VALUE ) static constexpr T NAME = VALUE;
#else /// Visual Studio 2013 or older, upgrade your vs bro
#define STATIC_VAR( T, NAME, VALUE ) static T NAME = VALUE;
#endif

template<typename T>
void safe_release( T** interface_to_release )
{
    if( interface_to_release && *interface_to_release ) {
        ( *interface_to_release )->Release();
        ( *interface_to_release ) = nullptr;
    }
}

#pragma warning( default : 4005 )
#endif
