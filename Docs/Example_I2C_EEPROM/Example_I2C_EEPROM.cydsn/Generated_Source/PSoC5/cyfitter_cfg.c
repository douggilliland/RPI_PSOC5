/*******************************************************************************
* Copyright 2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include <cytypes.h>
#include <cydevice.h>
#include <cydevice_trm.h>
#include <cyfitter.h>
#include <CyLib.h>
#include <CyDmac.h>
#include <cyfitter_cfg.h>

/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0
#define CYCLOCKSTART_XTAL_ERROR  1
#define CYCLOCKSTART_32KHZ_ERROR 2
#define CYCLOCKSTART_PLL_ERROR   3

/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc) the system will end up here.  Unless reimplemented by 
*  the customer, an infinite loop will be encountered.
*
* Parameters:  
*   void.
*
*
* Return:
*   void.
*
*******************************************************************************/
#if defined(__GNUC__) || defined(__ARMCC_VERSION)
__attribute__ ((unused))
#endif
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* `#START CyClockStartupError` */

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc)    */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

    /* `#END` */

    /* If nothing else, stop here since the clocks have not started         */
    /* correctly.                                                           */
    while(1) {}
}

/* This file contains the data used to initialize the device. */

#undef CYCODE
#undef CYDATA
#undef CYXDATA
#undef CYFAR
#if defined(__C51__) || defined(__CX51__)
#define CYCODE code
#define CYDATA data
#define CYXDATA xdata
#define CYFAR far
extern void cymemzero(void CYFAR *addr, uint16 size);
extern void cyconfigcpy(uint16 size, const void CYFAR *src, void CYFAR *dest) small;
extern void cyconfigcpycode(uint16 size, const void CYCODE *src, void CYFAR *dest);
#define CYMEMZERO(a,c) cymemzero((a),(c))
#define CYCONFIGCPY(d,s,c) cyconfigcpy((c),(s),(d))
#define CYCONFIGCPYCODE(d,s,c) cyconfigcpycode((c),(s),(d))
#else
#define CYCODE
#define CYDATA
#define CYXDATA
#define CYFAR
#define CYMEMZERO(a,c) cymemset((a),0,(c))
#define CYCONFIGCPY(d,s,c) cymemcpy((d),(s),(c))
#define CYCONFIGCPYCODE(d,s,c) cymemcpy((d),(s),(c))
#endif

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
#define CYPACKED __attribute__ ((packed))
#define CYALIGNED __attribute__ ((aligned))
#if CYDEV_CONFIGURATION_DMA
#define DMAINITSEC __attribute__ ((section (".dma_init")))
#else
#define DMAINITSEC
#endif
#if defined(__ARMCC_VERSION)
#define CY_CFG_MEMORY_BARRIER() __memory_changed()
#else
#define CY_CFG_MEMORY_BARRIER() __sync_synchronize()
#endif
#else
/* Keil C51 and others */
#define CYPACKED
#define CYALIGNED
#define DMAINITSEC
#define CY_CFG_MEMORY_BARRIER()
#endif

typedef struct {
	void CYFAR *address;
	uint16 size;
} CYPACKED cfg_memset_t;

typedef struct {
	void CYFAR *dest;
	const void CYCODE *src;
	uint16 size;
} CYPACKED cfg_memcpy_t;


#if defined(__C51__) || defined(__CX51__)
#define CYMEMZERO8(a,c) cymemzero((a),(c))
#define CYCONFIGCPY8(d,s,c) cyconfigcpy((c),(s),(d))
#define CYCONFIGCPYCODE8(d,s,c) cyconfigcpycode((c),(s),(d))
#else
#if defined(__GNUC__) || defined(__ARMCC_VERSION)
__attribute__ ((unused))
#endif
static void cymemzero8(void *addr, unsigned int count)
{
	volatile uint8 *addr8 = (uint8 *)addr;
	while (count--)
	{
		*addr8 = 0;
		addr8++;
	}
}

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
__attribute__ ((unused))
#endif
static void cymemcpy8(void *dest, const void *src, unsigned int count)
{
	volatile uint8 *dest8 = (uint8 *)dest;
	const uint8 *src8 = (const uint8 *)src;

	while (count--)
	{
		*dest8 = *src8;
		dest8++;
		src8++;
	}
}

#define CYMEMZERO8(a,c) cymemzero8((a),(c))
#define CYCONFIGCPY8(d,s,c) cymemcpy8((d),(s),(c))
#define CYCONFIGCPYCODE8(d,s,c) cymemcpy8((d),(s),(c))
#endif

#define CFG_MEMSET_COUNT (sizeof(cfg_memset_list)/sizeof(*cfg_memset_list))
static const cfg_memset_t CYCODE cfg_memset_list [] = {
	/* address, size */
	{(void CYFAR *)(CYDEV_IO_PRT_PRT0_DR), 32},
	{(void CYFAR *)(CYDEV_IO_PRT_PRT3_DR), 16},
	{(void CYFAR *)(CYDEV_IO_PRT_PRT5_DR), 32},
	{(void CYFAR *)(CYDEV_IO_PRT_PRT12_DR), 16},
	{(void CYFAR *)(CYDEV_IO_PRT_PRT15_DR), 16},
	{(void CYFAR *)(CYDEV_UCFG_B0_P0_U0_BASE), 2688},
	{(void CYFAR *)(CYDEV_UCFG_B0_P5_ROUTE_BASE), 256},
	{(void CYFAR *)(CYDEV_UCFG_B0_P6_U1_BASE), 896},
	{(void CYFAR *)(CYDEV_UCFG_B1_P2_U0_BASE), 2048},
	{(void CYFAR *)(CYDEV_UCFG_DSI0_BASE), 2560},
	{(void CYFAR *)(CYDEV_UCFG_DSI12_BASE), 512},
};

#define CY_CFG_BASE_ADDR_COUNT 16
typedef struct
{
	uint8 offset;
	uint8 value;
} CYPACKED cy_cfg_addrvalue_t;

static const uint32 CYCODE cy_cfg_addr_table[] = {
	0x40006403u, /* Base address: 0x40006400 Count: 3 */
	0x40010504u, /* Base address: 0x40010500 Count: 4 */
	0x40010702u, /* Base address: 0x40010700 Count: 2 */
	0x40010833u, /* Base address: 0x40010800 Count: 51 */
	0x4001093Cu, /* Base address: 0x40010900 Count: 60 */
	0x40010B4Eu, /* Base address: 0x40010B00 Count: 78 */
	0x40010C15u, /* Base address: 0x40010C00 Count: 21 */
	0x40010D3Bu, /* Base address: 0x40010D00 Count: 59 */
	0x40011904u, /* Base address: 0x40011900 Count: 4 */
	0x40014203u, /* Base address: 0x40014200 Count: 3 */
	0x40014509u, /* Base address: 0x40014500 Count: 9 */
	0x40014606u, /* Base address: 0x40014600 Count: 6 */
	0x40014704u, /* Base address: 0x40014700 Count: 4 */
	0x40014C01u, /* Base address: 0x40014C00 Count: 1 */
	0x40015003u, /* Base address: 0x40015000 Count: 3 */
	0x40015101u, /* Base address: 0x40015100 Count: 1 */
};

static const cy_cfg_addrvalue_t CYCODE cy_cfg_data_table[] = {
	{0x2Bu, 0x04u},
	{0x2Cu, 0x0Fu},
	{0x3Cu, 0x08u},
	{0x8Cu, 0x04u},
	{0x9Fu, 0x02u},
	{0xB3u, 0x02u},
	{0xE2u, 0x20u},
	{0xA8u, 0x04u},
	{0xABu, 0x02u},
	{0x00u, 0x64u},
	{0x01u, 0x40u},
	{0x02u, 0x92u},
	{0x03u, 0x9Fu},
	{0x05u, 0x04u},
	{0x07u, 0x7Au},
	{0x08u, 0x64u},
	{0x09u, 0x18u},
	{0x0Au, 0x8Au},
	{0x0Bu, 0x67u},
	{0x0Cu, 0x04u},
	{0x0Du, 0x60u},
	{0x0Eu, 0x79u},
	{0x0Fu, 0x80u},
	{0x10u, 0x25u},
	{0x11u, 0x02u},
	{0x14u, 0x21u},
	{0x15u, 0x01u},
	{0x18u, 0x21u},
	{0x19u, 0x02u},
	{0x1Bu, 0x19u},
	{0x1Du, 0x75u},
	{0x1Eu, 0x02u},
	{0x1Fu, 0x0Au},
	{0x21u, 0x6Du},
	{0x22u, 0xFFu},
	{0x23u, 0x12u},
	{0x24u, 0x01u},
	{0x25u, 0x01u},
	{0x26u, 0x20u},
	{0x28u, 0x40u},
	{0x2Au, 0x9Fu},
	{0x2Du, 0x81u},
	{0x2Eu, 0xFFu},
	{0x2Fu, 0x20u},
	{0x30u, 0xE4u},
	{0x31u, 0xE0u},
	{0x32u, 0x1Fu},
	{0x33u, 0x1Fu},
	{0x35u, 0x1Fu},
	{0x36u, 0xE4u},
	{0x37u, 0xE0u},
	{0x3Au, 0x8Au},
	{0x3Bu, 0xAAu},
	{0x3Cu, 0x8Au},
	{0x3Du, 0xAAu},
	{0x58u, 0x0Cu},
	{0x59u, 0x0Cu},
	{0x5Bu, 0x05u},
	{0x5Eu, 0x06u},
	{0x5Fu, 0x10u},
	{0x04u, 0x90u},
	{0x05u, 0x01u},
	{0x0Cu, 0xA8u},
	{0x0Fu, 0x80u},
	{0x14u, 0x01u},
	{0x15u, 0x07u},
	{0x17u, 0x10u},
	{0x1Cu, 0x30u},
	{0x1Du, 0x41u},
	{0x1Fu, 0x0Au},
	{0x24u, 0x40u},
	{0x25u, 0x10u},
	{0x26u, 0x48u},
	{0x27u, 0x10u},
	{0x2Cu, 0x22u},
	{0x2Eu, 0x09u},
	{0x34u, 0x10u},
	{0x35u, 0x48u},
	{0x37u, 0x02u},
	{0x38u, 0x08u},
	{0x39u, 0x10u},
	{0x3Bu, 0x28u},
	{0x3Cu, 0x40u},
	{0x3Du, 0x29u},
	{0x64u, 0x10u},
	{0x66u, 0x04u},
	{0x6Cu, 0x04u},
	{0x6Du, 0x14u},
	{0x6Fu, 0x01u},
	{0x74u, 0x8Cu},
	{0x75u, 0x18u},
	{0x76u, 0x0Au},
	{0x77u, 0x02u},
	{0x7Du, 0x40u},
	{0x7Fu, 0x10u},
	{0x8Eu, 0x04u},
	{0x90u, 0x80u},
	{0x91u, 0x42u},
	{0x94u, 0x08u},
	{0x97u, 0x08u},
	{0x98u, 0x01u},
	{0x9Du, 0x10u},
	{0x9Eu, 0x06u},
	{0x9Fu, 0x11u},
	{0xA0u, 0x02u},
	{0xA1u, 0x04u},
	{0xA3u, 0x40u},
	{0xA4u, 0x84u},
	{0xA6u, 0x48u},
	{0xA7u, 0x0Au},
	{0xACu, 0x01u},
	{0xADu, 0x40u},
	{0xB1u, 0x20u},
	{0xC0u, 0xD0u},
	{0xC2u, 0xF0u},
	{0xC4u, 0xF0u},
	{0xCAu, 0xD0u},
	{0xCCu, 0xF0u},
	{0xCEu, 0xF0u},
	{0xDEu, 0x50u},
	{0x04u, 0x60u},
	{0x05u, 0x04u},
	{0x06u, 0x40u},
	{0x0Du, 0x10u},
	{0x0Eu, 0x42u},
	{0x0Fu, 0x20u},
	{0x10u, 0x80u},
	{0x12u, 0x40u},
	{0x15u, 0x02u},
	{0x16u, 0x08u},
	{0x17u, 0x48u},
	{0x1Cu, 0x50u},
	{0x1Eu, 0x02u},
	{0x1Fu, 0x20u},
	{0x24u, 0xA0u},
	{0x27u, 0x0Au},
	{0x2Cu, 0x12u},
	{0x2Eu, 0x02u},
	{0x2Fu, 0x08u},
	{0x30u, 0x10u},
	{0x31u, 0x10u},
	{0x36u, 0x02u},
	{0x37u, 0x10u},
	{0x3Cu, 0x28u},
	{0x3Du, 0x81u},
	{0x45u, 0x22u},
	{0x4Du, 0x20u},
	{0x4Fu, 0x04u},
	{0x54u, 0x02u},
	{0x56u, 0x08u},
	{0x57u, 0x40u},
	{0x5Cu, 0x10u},
	{0x5Eu, 0x80u},
	{0x5Fu, 0x03u},
	{0x62u, 0x08u},
	{0x63u, 0x20u},
	{0x67u, 0x41u},
	{0x79u, 0x20u},
	{0x7Bu, 0x08u},
	{0x7Cu, 0x02u},
	{0x7Fu, 0x04u},
	{0x8Bu, 0x40u},
	{0x8Fu, 0x08u},
	{0x91u, 0x43u},
	{0x92u, 0x40u},
	{0x94u, 0x08u},
	{0x97u, 0x04u},
	{0x98u, 0x02u},
	{0x99u, 0x04u},
	{0x9Bu, 0x40u},
	{0x9Eu, 0x04u},
	{0x9Fu, 0x11u},
	{0xA0u, 0x10u},
	{0xA1u, 0x66u},
	{0xA2u, 0x02u},
	{0xA7u, 0x6Au},
	{0xA8u, 0x84u},
	{0xA9u, 0x40u},
	{0xABu, 0x80u},
	{0xACu, 0x02u},
	{0xB1u, 0x01u},
	{0xB3u, 0x42u},
	{0xB5u, 0x05u},
	{0xB7u, 0x22u},
	{0xC0u, 0xF0u},
	{0xC2u, 0xF0u},
	{0xC4u, 0xF0u},
	{0xCAu, 0xF0u},
	{0xCCu, 0xA0u},
	{0xCEu, 0xF0u},
	{0xD0u, 0xA0u},
	{0xD2u, 0x20u},
	{0xD6u, 0x30u},
	{0xD8u, 0x90u},
	{0xDEu, 0xA0u},
	{0xE2u, 0x30u},
	{0xE8u, 0x20u},
	{0xEAu, 0x08u},
	{0x85u, 0x1Fu},
	{0x8Fu, 0x40u},
	{0x91u, 0x01u},
	{0x93u, 0x02u},
	{0x97u, 0x12u},
	{0x99u, 0x12u},
	{0x9Bu, 0x21u},
	{0x9Du, 0x10u},
	{0x9Fu, 0x20u},
	{0xA9u, 0x20u},
	{0xABu, 0x04u},
	{0xADu, 0x20u},
	{0xAFu, 0x08u},
	{0xB1u, 0x40u},
	{0xB3u, 0x40u},
	{0xB5u, 0x3Fu},
	{0xB9u, 0x20u},
	{0xBDu, 0x15u},
	{0xD9u, 0x0Fu},
	{0xDEu, 0x04u},
	{0xDFu, 0x10u},
	{0x00u, 0x20u},
	{0x02u, 0x04u},
	{0x04u, 0x14u},
	{0x05u, 0x01u},
	{0x07u, 0x01u},
	{0x08u, 0x01u},
	{0x0Bu, 0x40u},
	{0x0Cu, 0x02u},
	{0x0Du, 0x80u},
	{0x0Fu, 0x24u},
	{0x10u, 0x08u},
	{0x11u, 0x10u},
	{0x14u, 0x01u},
	{0x15u, 0x08u},
	{0x16u, 0x40u},
	{0x17u, 0x10u},
	{0x1Du, 0x2Au},
	{0x1Eu, 0x01u},
	{0x1Fu, 0x80u},
	{0x20u, 0x04u},
	{0x21u, 0x05u},
	{0x22u, 0x12u},
	{0x25u, 0x44u},
	{0x26u, 0x80u},
	{0x27u, 0x40u},
	{0x2Bu, 0x0Au},
	{0x2Eu, 0x40u},
	{0x2Fu, 0x0Au},
	{0x30u, 0x08u},
	{0x31u, 0x80u},
	{0x32u, 0x82u},
	{0x33u, 0x10u},
	{0x36u, 0x02u},
	{0x37u, 0x10u},
	{0x38u, 0x80u},
	{0x3Au, 0x08u},
	{0x3Cu, 0x30u},
	{0x3Du, 0x18u},
	{0x3Fu, 0x80u},
	{0x45u, 0x08u},
	{0x46u, 0x40u},
	{0x4Cu, 0x44u},
	{0x4Fu, 0x20u},
	{0x56u, 0x48u},
	{0x58u, 0x02u},
	{0x5Au, 0x01u},
	{0x78u, 0x80u},
	{0x7Bu, 0x80u},
	{0x7Cu, 0x80u},
	{0x7Fu, 0x80u},
	{0x8Eu, 0x10u},
	{0xC0u, 0xF0u},
	{0xC2u, 0xF0u},
	{0xC4u, 0xF0u},
	{0xCAu, 0x3Cu},
	{0xCCu, 0xAFu},
	{0xCEu, 0x7Au},
	{0xD0u, 0xC0u},
	{0xDEu, 0x99u},
	{0xA8u, 0x40u},
	{0xACu, 0x10u},
	{0xAEu, 0x01u},
	{0xE8u, 0x01u},
	{0x34u, 0x02u},
	{0x88u, 0x02u},
	{0xCCu, 0x80u},
	{0x0Bu, 0x10u},
	{0x0Fu, 0x60u},
	{0x5Du, 0x08u},
	{0x60u, 0x08u},
	{0x8Cu, 0x08u},
	{0xC2u, 0x0Cu},
	{0xD6u, 0x01u},
	{0xD8u, 0x02u},
	{0xE0u, 0x02u},
	{0x93u, 0x40u},
	{0xAFu, 0x10u},
	{0xB5u, 0x08u},
	{0xB7u, 0x10u},
	{0xE8u, 0x02u},
	{0xEAu, 0x08u},
	{0x8Cu, 0x01u},
	{0xB3u, 0x40u},
	{0xE6u, 0x08u},
	{0xEAu, 0x04u},
	{0xA8u, 0x01u},
	{0x00u, 0x01u},
	{0x0Cu, 0x01u},
	{0x0Eu, 0x01u},
	{0x03u, 0x0Cu},
};


/* UDB_0_3_1_CONFIG Address: CYDEV_UCFG_B0_P5_U1_BASE Size (bytes): 128 */
static const uint8 CYCODE BS_UDB_0_3_1_CONFIG_VAL[] DMAINITSEC = {
	0x02, 0x61, 0x00, 0x0E, 0x20, 0x01, 0x40, 0x00, 0x02, 0x0A, 0x78, 0x01, 0x01, 0x10, 0x00, 0x43, 
	0x18, 0x00, 0x66, 0xFF, 0x08, 0x00, 0x72, 0x00, 0x1C, 0x44, 0x62, 0x12, 0x40, 0x00, 0x20, 0x00, 
	0x00, 0x00, 0x60, 0x02, 0x01, 0x02, 0x1E, 0x00, 0x04, 0x1C, 0x00, 0x20, 0x10, 0x01, 0x6A, 0x00, 
	0x1F, 0x7F, 0x60, 0x7F, 0x1F, 0x80, 0x60, 0x00, 0x22, 0x00, 0x88, 0x8A, 0x99, 0x9A, 0x00, 0x00, 
	0x42, 0x00, 0x00, 0x00, 0x06, 0x0D, 0x0D, 0xDB, 0x35, 0xFF, 0x7F, 0xFF, 0x42, 0xA0, 0x70, 0x00, 
	0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x05, 0x00, 
	0x00, 0x09, 0x40, 0x09, 0x30, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* UDB_0_4_1_CONFIG Address: CYDEV_UCFG_B0_P6_U0_BASE Size (bytes): 128 */
static const uint8 CYCODE BS_UDB_0_4_1_CONFIG_VAL[] DMAINITSEC = {
	0x14, 0x00, 0x08, 0x00, 0x01, 0x2D, 0x02, 0xD2, 0x04, 0x45, 0x00, 0xBA, 0x40, 0x03, 0x00, 0x00, 
	0x02, 0x00, 0x01, 0xFF, 0x40, 0x00, 0x00, 0x00, 0x20, 0x90, 0x04, 0x44, 0x04, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x1F, 0x00, 0x60, 0x0C, 0xA7, 0x10, 0x48, 
	0x20, 0xFF, 0x07, 0xFF, 0x18, 0x00, 0x40, 0xFF, 0x88, 0x00, 0x00, 0x8A, 0x00, 0x8A, 0x55, 0x00, 
	0x43, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x05, 0x16, 0xFF, 0x7F, 0xFF, 0x42, 0xA0, 0x70, 0x00, 
	0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x04, 0x10, 
	0x00, 0x48, 0x40, 0x48, 0x00, 0x48, 0x80, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* IOPINS0_2 Address: CYDEV_IO_PRT_PRT2_DM0 Size (bytes): 8 */
static const uint8 CYCODE BS_IOPINS0_2_VAL[] DMAINITSEC = {
	0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00};

/* IOPINS0_4 Address: CYDEV_IO_PRT_PRT4_DM0 Size (bytes): 8 */
static const uint8 CYCODE BS_IOPINS0_4_VAL[] DMAINITSEC = {
	0x00, 0x00, 0x30, 0x00, 0x30, 0x00, 0x00, 0x00};

/* IOPORT_4 Address: CYDEV_PRTDSI_PRT4_BASE Size (bytes): 7 */
static const uint8 CYCODE BS_IOPORT_4_VAL[] DMAINITSEC = {
	0x20, 0x10, 0x00, 0x00, 0x30, 0x00, 0x00};

#define CFG_MEMCPY_COUNT (sizeof(cfg_memcpy_list)/sizeof(*cfg_memcpy_list))
static const cfg_memcpy_t CYCODE cfg_memcpy_list [] = {
	/* dest, src, size */
	{(void CYFAR *)(CYDEV_UCFG_B0_P5_U1_BASE), BS_UDB_0_3_1_CONFIG_VAL, 128},
	{(void CYFAR *)(CYDEV_UCFG_B0_P6_U0_BASE), BS_UDB_0_4_1_CONFIG_VAL, 128},
};


#if defined(__C51__) || defined(__CX51__)
extern void cfg_write_bytes_code(void CYCODE *table);
extern void cfg_write_bytes(void CYFAR *table);
#else
/* For 32-bit little-endian architectures */
static void cfg_write_bytes32(void)
{
	unsigned int i, j = 0;
	for (i = 0; i < CY_CFG_BASE_ADDR_COUNT; i++)
	{
		uint32 baseAddr = cy_cfg_addr_table[i];
		uint8 count = baseAddr & 0xFFu;
		baseAddr &= 0xFFFFFF00u;
		while (count--)
		{
			CY_SET_XTND_REG8((void CYFAR *)(baseAddr + cy_cfg_data_table[j].offset), cy_cfg_data_table[j].value);
			j++;
		}
	}
}
#endif

static void ClockSetup(void)
{
	reg32  timeout;
	reg32* timeout_p = &timeout;
	reg8   pllLock;

	CY_SET_XTND_REG24((void CYFAR *)(CYDEV_CLKDIST_DCFG0_CFG0), 0x18000E);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_SLOWCLK_ILO_CR0), 0x02);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CLKDIST_CR), 0x08);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_FASTCLK_IMO_CR), 0x03);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_MFGCFG_IMO_TR1), (CY_GET_XTND_REG8(CYDEV_FLSHID_CUST_TABLES_IMO_3MHZ)));
	CY_SET_XTND_REG16((void CYFAR *)(CYDEV_FASTCLK_PLL_P), 0x0008);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CLKDIST_CR), 0x08);
	CY_SET_XTND_REG16((void CYFAR *)(CYDEV_FASTCLK_PLL_CFG0), 0x1051);
	pllLock = 0;
	for (timeout = 304; *timeout_p && (pllLock != 0x03u); timeout--) { pllLock = 0x03u & ((pllLock << 1) | ((CY_GET_XTND_REG8(CYDEV_FASTCLK_PLL_SR) & 0x01u) >> 0)); }
	CY_SET_XTND_REG16((void CYFAR *)(CYDEV_CLKDIST_MSTR0), 0x0103);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CLKDIST_MSTR0), 0x00);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CLKDIST_MSTR0), 0x07);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CLKDIST_BCFG0), 0x00);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CLKDIST_BCFG2), 0x48);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CLKDIST_MSTR0), 0x00);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CLKDIST_LD), 0x02);

	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_PM_ACT_CFG2), ((CY_GET_XTND_REG8(CYDEV_PM_ACT_CFG2) | 0x01)));
}

/* Analog API Functions */
void AnalogSetDefault(void)
{
	CY_SET_XTND_REG8(CYDEV_ANAIF_CFG_PUMP_CR0, 0x44);
}

#define CY_AMUX_UNUSED CYDEV_BOOST_SR

/* This function is called by the start-up code for the selected device. */
void cyfitter_cfg(void)
{
	uint8 CYDATA i;

#ifdef CYGlobalIntDisable
	CYGlobalIntDisable;
#endif

	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CACHE_CC_CTL), ((CYDEV_INSTRUCT_CACHE_ENABLED) ? 0x40 : 0x41));
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_PHUB_CH0_BASIC_CFG), 0x00);
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_PHUB_CH1_BASIC_CFG), 0x00);

	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_ANAIF_CFG_MISC_CR0), 0x01);

	CY_SET_XTND_REG8(CYDEV_MFGCFG_MLOGIC_DBG_DBE, (CY_GET_XTND_REG8(CYDEV_MFGCFG_MLOGIC_DBG_DBE) | 0x01));
	for (i = 0; i < CFG_MEMSET_COUNT; i++)
	{
		const cfg_memset_t CYCODE * CYDATA ms = &cfg_memset_list[i];
		CYMEMZERO(ms->address, ms->size);
	}

	for (i = 0; i < CFG_MEMCPY_COUNT; i++)
	{
		const cfg_memcpy_t CYCODE * CYDATA mc = &cfg_memcpy_list[i];
		void * CYDATA dest = mc->dest;
		const void CYCODE * CYDATA src = mc->src;
		uint16 CYDATA size = mc->size;
		CYCONFIGCPYCODE(dest, src, size);
	}

	cfg_write_bytes32();

	CYMEMZERO8((void CYFAR *)(CYDEV_PRTDSI_PRT0_BASE), 7);
	CYMEMZERO8((void CYFAR *)(CYDEV_PRTDSI_PRT1_BASE), 7);
	CYMEMZERO8((void CYFAR *)(CYDEV_PRTDSI_PRT2_BASE), 7);
	CYMEMZERO8((void CYFAR *)(CYDEV_PRTDSI_PRT3_BASE), 7);
	CYCONFIGCPYCODE8((void CYFAR *)(CYDEV_PRTDSI_PRT4_BASE), (void CYCODE *)(BS_IOPORT_4_VAL), 7);
	CYMEMZERO8((void CYFAR *)(CYDEV_PRTDSI_PRT5_BASE), 7);
	CYMEMZERO8((void CYFAR *)(CYDEV_PRTDSI_PRT6_BASE), 7);
	CYMEMZERO8((void CYFAR *)(CYDEV_PRTDSI_PRT12_BASE), 6);
	CYMEMZERO8((void CYFAR *)(CYDEV_PRTDSI_PRT15_BASE), 7);
	/* BYTES WRITTEN: 62 */

	/* Enable digital routing */
	CY_SET_XTND_REG8(CYDEV_UCFG_BCTL0_BANK_CTL, CY_GET_XTND_REG8(CYDEV_UCFG_BCTL0_BANK_CTL) | 0x02u);
	CY_SET_XTND_REG8(CYDEV_UCFG_BCTL1_BANK_CTL, CY_GET_XTND_REG8(CYDEV_UCFG_BCTL1_BANK_CTL) | 0x02u);

	/* Enable UDB array */
	CY_SET_XTND_REG8(CYDEV_PM_ACT_CFG0, CY_GET_XTND_REG8(CYDEV_PM_ACT_CFG0) | 0x40u);
	CY_SET_XTND_REG8(CYDEV_PM_AVAIL_CR2, CY_GET_XTND_REG8(CYDEV_PM_AVAIL_CR2) | 0x10u);

	CYCONFIGCPYCODE((void CYFAR *)(CYDEV_IO_PRT_PRT2_DM0), (void CYCODE *)(BS_IOPINS0_2_VAL), 8);
	CYCONFIGCPYCODE((void CYFAR *)(CYDEV_IO_PRT_PRT4_DM0), (void CYCODE *)(BS_IOPINS0_4_VAL), 8);
	/* BYTES WRITTEN: 16 */

	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CACHE_CC_CTL), ((CYDEV_INSTRUCT_CACHE_ENABLED) ? 0x00 : 0x01));
	ClockSetup();
	CY_SET_XTND_REG8((void CYFAR *)(CYDEV_CACHE_CC_CTL), ((CYDEV_INSTRUCT_CACHE_ENABLED) ? 0x40 : 0x41));

	AnalogSetDefault();

	/* Configure alternate active mode */
	CYCONFIGCPY((void CYFAR *)CYDEV_PM_STBY_BASE, (void CYFAR *)CYDEV_PM_ACT_BASE, 12);
	CY_SET_XTND_REG8(CYDEV_PM_STBY_CFG0, CY_GET_XTND_REG8(CYDEV_PM_STBY_CFG0) & ~0x02u);	/* Disable CPU */
}
