/************************************
 * author: Aysi
 * version: 1.0
 * date: 2019.6.1
 * for memory, stay a profile, design by MTF(Aysi@mtf123.club)
 * **********************************/

#ifndef _TYPES_PLUS_H
#define _TYPES_PLUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types_base.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef unsigned char     UCHAR;    ///< Define char unsigned data type
typedef unsigned char *   PUINT8;   ///< Define 8-bit unsigned pointer data type
typedef unsigned char *   PUCHAR;   ///< Define char unsigned pointer data type
typedef char *            PSTR;     ///< Define string pointer data type
typedef const char *      PCSTR;    ///< Define constant string pointer data type

typedef short             SHORT;    ///< Define short signed data type
typedef short *           PSHORT;   ///< Define short signed pointer data type
typedef unsigned short    USHORT;   ///< Define short unsigned data type
typedef unsigned short *  PUSHORT;  ///< Define short unsigned pointer data type

typedef unsigned char	  BYTE; //This type MUST be 8 bit
typedef unsigned short	  WORD; //These types MUST be 16 bit
typedef unsigned short	  WCHAR; //These types MUST be 16 bit
typedef unsigned long	  DWORD; //These types MUST be 32 bit
typedef long              LONG; //These types MUST be 32 bit
typedef unsigned long     ULONG; //These types MUST be 32 bit

typedef int               INT;      ///< Define integer signed data type
typedef int *             PINT;     ///< Define integer signed pointer data type
typedef unsigned int      UINT;     ///< Define integer unsigned data type
typedef unsigned int *    PUINT;    ///< Define integer unsigned pointer data type

#define bit0 0x01
#define bit1 0x02
#define bit2 0x04
#define bit3 0x08

#define bit4 0x10
#define bit5 0x20
#define bit6 0x40
#define bit7 0x80

#define bit8 0x0100
#define bit9 0x0200
#define bit10 0x0400
#define bit11 0x0800

#define bit12 0x00001000
#define bit13 0x00002000
#define bit14 0x00004000
#define bit15 0x00008000

#define bit16 0x00010000
#define bit17 0x00020000
#define bit18 0x00040000
#define bit19 0x00080000

#define bit20 0x00100000
#define bit21 0x00200000
#define bit22 0x00400000
#define bit23 0x00800000

#define bit24 0x01000000
#define bit25 0x02000000
#define bit26 0x04000000
#define bit27 0x08000000

#define bit28 0x10000000
#define bit29 0x20000000
#define bit30 0x40000000
#define bit31 0x80000000

typedef enum 
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

/** 
  * @brief  HAL Lock structures definition  
  */
typedef enum 
{
  HAL_UNLOCKED = 0x00,
  HAL_LOCKED   = 0x01  
} HAL_LockTypeDef;

typedef uint32_t dev_addr;

/* Exported macro ------------------------------------------------------------*/
#define HAL_MAX_DELAY      0xFFFFFFFFU

#define HAL_IS_BIT_SET(REG, BIT)         (((REG) & (BIT)) != RESET)
#define HAL_IS_BIT_CLR(REG, BIT)         (((REG) & (BIT)) == RESET)

#define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__)               \
                        do{                                                      \
                              (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__); \
                              (__DMA_HANDLE__).Parent = (__HANDLE__);             \
                          } while(0)

#define UNUSED(x) ((void)(x))

/** @brief Reset the Handle's State field.
  * @param __HANDLE__: specifies the Peripheral Handle.
  * @note  This macro can be used for the following purpose: 
  *          - When the Handle is declared as local variable; before passing it as parameter
  *            to HAL_PPP_Init() for the first time, it is mandatory to use this macro 
  *            to set to 0 the Handle's "State" field.
  *            Otherwise, "State" field may have any random value and the first time the function 
  *            HAL_PPP_Init() is called, the low level hardware initialization will be missed
  *            (i.e. HAL_PPP_MspInit() will not be executed).
  *          - When there is a need to reconfigure the low level hardware: instead of calling
  *            HAL_PPP_DeInit() then HAL_PPP_Init(), user can make a call to this macro then HAL_PPP_Init().
  *            In this later function, when the Handle's "State" field is set to 0, it will execute the function
  *            HAL_PPP_MspInit() which will reconfigure the low level hardware.
  * @retval None
  */
#define __HAL_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = 0U)

#ifdef __cplusplus
}
#endif

#endif
