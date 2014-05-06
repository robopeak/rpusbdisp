//
//  int_types.h
//  Integer types
//
//  Created by Tony Huang on 12/11/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#pragma once

    //Basic types
    //
#ifdef WIN32
    
    //fake stdint.h for VC only
    
    typedef signed   char     int8_t;
    typedef unsigned char     uint8_t;
    
    typedef __int16           int16_t;
    typedef unsigned __int16  uint16_t;
    
    typedef __int32           int32_t;
    typedef unsigned __int32  uint32_t;
    
    typedef __int64           int64_t;
    typedef unsigned __int64  uint64_t;
    
#else
#   include <stdint.h>
#endif

typedef int8_t  _s8;
typedef uint8_t _u8;
typedef int16_t  _s16;
typedef uint16_t _u16;
typedef int32_t  _s32;
typedef uint32_t _u32;
typedef int64_t  _s64;
typedef uint64_t _u64;

    // The _word_size_t uses actual data bus width of the current CPU
#ifdef _AVR_
    typedef uint8_t         word_size_t;
#elif defined (WIN64)
    typedef uint64_t        word_size_t;
#elif defined (WIN32)
    typedef uint32_t        word_size_t;
#elif defined (__GNUC__)
    typedef unsigned long   word_size_t;
#elif defined (__ICCARM__)
    typedef uint32_t        word_size_t;
#endif
