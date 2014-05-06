//
//  infra_config.h
//  Infra configure header
//
//  Created by Tony Huang on 12/7/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

// Infra version
#define RP_INFRA_VERSION_MAJOR   1
#define RP_INFRA_VERSION_MINOR   0
#define RP_INFRA_VERSION_RELEASE 0
#define RP_INFRA_VERSION_STRING  (#RP_INFRA_VERSION_MAJOR "." #RP_INFRA_VERSION_MINOR "." #RP_INFRA_VERSION_RELEASE)

// Platform detection
#if defined(_WIN64) || defined(_WIN32)
#   define RP_INFRA_PLATFORM_WINDOWS
#elif defined(__APPLE__)
#   include <TargetConditionals.h>
#   if defined(TARGET_IPHONE_SIMULATOR)
#       define RP_INFRA_PLATFORM_IOS
#       define RP_INTRA_PLATFORM_IOS_SIMULATOR
#   elif defined(TARGET_OS_IPHONE)
#       define RP_INTRA_PLATFORM_IOS
#   elif defined(TARGET_OS_MAC)
#       define RP_INFRA_PLATFORM_MACOS
#   endif
#elif defined(__linux)
#   define RP_INFRA_PLATFORM_LINUX
#elif defined(__unix)
#   define RP_INFRA_PLATFORM_UNIX
#elif defined(__posix)
#   define RP_INFRA_PLATFORM_POSIX
#endif

#if defined(RP_INFRA_PLATFORM_WINDOWS)
#   define RP_INFRA_PLATFORM_STRING "windows"
#elif defined(RP_INFRA_PLATFORM_IOS_SIMULATOR)
#   define RP_INFRA_PLATFORM_STRING "ios_simulator"
#elif defined(RP_INFRA_PLATFORM_IOS)
#   define RP_INFRA_PLATFORM_STRING "ios"
#elif defined(RP_INFRA_PLATFORM_MACOS)
#   define RP_INFRA_PLATFORM_STRING "macos"
#elif defined(RP_INFRA_PLATFORM_LINUX)
#   define RP_INFRA_PLATFORM_STRING "linux"
#elif defined(RP_INFRA_PLATFORM_UNIX)
#   define RP_INFRA_PLATFORM_STRING "unix"
#elif defined(RP_INFRA_PLATFORM_POSIX)
#   define RP_INFRA_PLATFORM_STRING "posix"
#else
#   error "Unsupported Platform Detected"
#endif

// Configuration for endian
#if defined(__ORDER_LITTLE_ENDIAN__) || defined(__LITTLE_ENDIAN__) || defined(RP_INFRA_PLATFORM_WINDOWS)
#   define RP_INFRA_LITTLE_ENDIAN
#   define RP_INFRA_ENDIAN_STRING "little_endian"
#else
#   define RP_INFRA_BIG_ENDIAN
#   define RP_INFRA_ENDIAN_STRING "big_endian"
#endif

#if defined(RP_INFRA_PLATFORM_WINDOWS)
#	define RP_INFRA_API	__declspec(dllexport)
#	define RP_INFRA_CALLBACK __stdcall
#	define __attribute__(x)
#else
#	define RP_INFRA_API
#	define RP_INFRA_CALLBACK
#endif

#if defined(RP_INFRA_PLATFORM_WINDOWS)
#	define WIN32_DEAN_AND_LEAN
#	define snprintf sprintf_s
#	include <Windows.h>

static inline void sleep(int seconds) {
	Sleep(seconds * 1000);
}
#endif
