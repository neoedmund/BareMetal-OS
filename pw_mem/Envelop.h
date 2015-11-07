#ifndef _neoe_mem_Envelop_H
#define _neoe_mem_Envelop_H

// package neoe.mem;
#include <stdint.h>
#include "Lv0.h"
// public class Envelop {
 typedef struct {
	neoe_mem_Lv0* lv0;
	uint64_t start;
 } neoe_mem_Envelop;


	/*final*/
 /*static*/
#define  neoe_mem_Envelop_patch   8


	/*final*/
 /*static*/
 //neoedmund1.0
#define  neoe_mem_Envelop_Signature  0x17303d1772117d10L

	/*final*/
 /*static*/
 // 64= 8(long) * 8
#define    neoe_mem_Envelop_OverheadSize  ( 64 + neoe_mem_Envelop_patch)

	// can set from 1銆渁ny, mind the rate of payload/overhead
	/*final*/
 /*static*/
#define  neoe_mem_Envelop_MinDataSize   neoe_mem_Envelop_OverheadSize

	/*public*/
 /*static*/
 /*final*/
#define   neoe_mem_Envelop_TYPE_FREE   0

	/*public*/
 /*static*/
 /*final*/
#define   neoe_mem_Envelop_TYPE_USE  1

	/*public*/
 /*static*/
 uint64_t neoe_mem_Envelop_fromUserAddress(  uint64_t addr) ;


	/*public*/
 /*static*/
 uint64_t neoe_mem_Envelop_toUserAddress(  uint64_t start) ;




	/*public*/
 void neoe_mem_Envelop_Init(neoe_mem_Envelop* self, neoe_mem_Lv0* lv0, uint64_t start) ;


	/*public*/
 uint64_t neoe_mem_Envelop_alignLow(neoe_mem_Envelop* self, uint64_t size) ;


	/*private*/
 void neoe_mem_Envelop_checkSignature(neoe_mem_Envelop* self) ;


	/*public*/
 uint64_t neoe_mem_Envelop_getCurrentDataLen(neoe_mem_Envelop* self) ;


	/*public*/
 uint64_t neoe_mem_Envelop_getDataAddr(neoe_mem_Envelop* self) ;


	/*public*/
 uint64_t neoe_mem_Envelop_getMaxDataLen(neoe_mem_Envelop* self) ;


	/*public*/
 uint64_t neoe_mem_Envelop_getNextPos(neoe_mem_Envelop* self) ;


	/*public*/
 uint64_t neoe_mem_Envelop_getP1(neoe_mem_Envelop* self) ;


	/*public*/
 uint64_t neoe_mem_Envelop_getP2(neoe_mem_Envelop* self) ;


	/*public*/
 uint64_t neoe_mem_Envelop_getPrevPos(neoe_mem_Envelop* self) ;


	/*private*/
 uint64_t neoe_mem_Envelop_getSignature(neoe_mem_Envelop* self) ;


	/*public*/
 uint64_t neoe_mem_Envelop_getTotalSize1(neoe_mem_Envelop* self) ;


	/*public*/
 uint64_t neoe_mem_Envelop_getType(neoe_mem_Envelop* self) ;


	/*public*/
 void neoe_mem_Envelop_reload(neoe_mem_Envelop* self, uint64_t p1) ;


	/*private*/
 void neoe_mem_Envelop_setMaxDataLen(neoe_mem_Envelop* self, uint64_t v) ;


	/*public*/
 void neoe_mem_Envelop_setNextPos(neoe_mem_Envelop* self, uint64_t v) ;


	/*public*/
 void neoe_mem_Envelop_setP1(neoe_mem_Envelop* self, uint64_t v) ;


	/*public*/
 void neoe_mem_Envelop_setP2(neoe_mem_Envelop* self, uint64_t v) ;


	/*private*/
 void neoe_mem_Envelop_setTotalSize1(neoe_mem_Envelop* self, uint64_t v) ;


	/*private*/
 void neoe_mem_Envelop_setTotalSize2(neoe_mem_Envelop* self, uint64_t v) ;


	/*public*/
 void neoe_mem_Envelop_setType(neoe_mem_Envelop* self, uint64_t type) ;


	/*public*/
 void neoe_mem_Envelop_setSize(neoe_mem_Envelop* self, uint64_t size) ;


// }/*cls*/

#endif

