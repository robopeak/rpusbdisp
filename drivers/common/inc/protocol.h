/* 
 *  RoboPeak Project
 *  Copyright 2009 - 2013
 *
 *  RP USB Display
 *  Protocol Def
 *  
 *  Initial Version by Shikai Chen
 */

#pragma once


#define RPUSBDISP_DISP_CHANNEL_MAX_SIZE    64 //64bytes
#define RPUSBDISP_STATUS_CHANNEL_MAX_SIZE  32 //32bytes


// -- Display Packets
#define RPUSBDISP_DISPCMD_NOPE             0
#define RPUSBDISP_DISPCMD_FILL             1  
#define RPUSBDISP_DISPCMD_BITBLT           2
#define RPUSBDISP_DISPCMD_RECT             3
#define RPUSBDISP_DISPCMD_COPY_AREA        4
#define RPUSBDISP_DISPCMD_BITBLT_RLE       5


#define RPUSBDISP_OPERATION_COPY            0
#define RPUSBDISP_OPERATION_XOR             1
#define RPUSBDISP_OPERATION_OR              2
#define RPUSBDISP_OPERATION_AND             3

#if defined(_WIN32) || defined(__ICCARM__)
#pragma pack(1)
#endif


#define RPUSBDISP_CMD_MASK                  (0x3F)
#define RPUSBDISP_CMD_FLAG_CLEARDITY        (0x1<<6)
#define RPUSBDISP_CMD_FLAG_START            (0x1<<7)
typedef struct _rpusbdisp_disp_packet_header_t {
#if 0
    _u8 cmd:6; 
    _u8 cleardirty:1;
    _u8 start:1;
#else
    _u8 cmd_flag;
#endif
} __attribute__((packed)) rpusbdisp_disp_packet_header_t;


typedef struct _rpusbdisp_disp_fill_packet_t {
    rpusbdisp_disp_packet_header_t header;
    _u16 color_565;
} __attribute__((packed)) rpusbdisp_disp_fill_packet_t;


typedef struct _rpusbdisp_disp_bitblt_packet_t {
    rpusbdisp_disp_packet_header_t header;
    _u16 x;
    _u16 y;
    _u16 width;
    _u16 height;
    _u8  operation;
} __attribute__((packed)) rpusbdisp_disp_bitblt_packet_t;


typedef struct _rpusbdisp_disp_fillrect_packet_t {
    rpusbdisp_disp_packet_header_t header;
    _u16 left;
    _u16 top;
    _u16 right;
    _u16 bottom;
    _u16 color_565;
    _u8  operation;
} __attribute__((packed)) rpusbdisp_disp_fillrect_packet_t;


typedef struct _rpusbdisp_disp_copyarea_packet_t {
    rpusbdisp_disp_packet_header_t header;
    _u16 sx;
    _u16 sy;
    _u16 dx;
    _u16 dy;
    _u16 width;
    _u16 height;
} __attribute__((packed)) rpusbdisp_disp_copyarea_packet_t;

#if defined(_WIN32) || defined(__ICCARM__)
#pragma pack()
#endif


// RLE Packet Define
#define RPUSBDISP_RLE_BLOCKFLAG_COMMON_BIT        0x80
#define RPUSBDISP_RLE_BLOCKFLAG_SIZE_BIT          0x7f

// -- Status Packets

#define RPUSBDISP_STATUS_TYPE_NORMAL  0


#define RPUSBDISP_DISPLAY_STATUS_DIRTY_FLAG   0x80  //a full screen transfer is required


#define RPUSBDISP_TOUCH_STATUS_NO_TOUCH       0
#define RPUSBDISP_TOUCH_STATUS_PRESSED        1

#if defined(_WIN32) || defined(__ICCARM__)
#pragma pack(1)
#endif


typedef struct _rpusbdisp_status_packet_header_t {
    _u8 packet_type;
} __attribute__((packed)) rpusbdisp_status_packet_header_t;


typedef struct _rpusbdisp_status_normal_packet_t {
    rpusbdisp_status_packet_header_t header;
    _u8 display_status;
    _u8 touch_status;
    _s32 touch_x;
    _s32 touch_y;
} __attribute__((packed)) rpusbdisp_status_normal_packet_t;

#if defined(_WIN32) || defined(__ICCARM__)
#pragma pack()
#endif
