#ifndef _neoe_mem_Lv1_H
#define _neoe_mem_Lv1_H
// package neoe.mem;

#include <stdint.h>
#include <stdbool.h>
#include "Lv0.h"
#include "Envelop.h"

// public class Lv1 {
 typedef struct { 
	neoe_mem_Lv0* lv0;
 } neoe_mem_Lv1;   



	/*public*/
 void neoe_mem_Lv1_Init(neoe_mem_Lv1* self, neoe_mem_Lv0* lv0) ;


	/*public*/
 uint64_t neoe_mem_Lv1_alloc(neoe_mem_Lv1* self, uint64_t size) ;


	/*private*/
 void neoe_mem_Lv1_doUse(neoe_mem_Lv1* self, neoe_mem_Envelop* b, uint64_t size) ;


	/*private*/
 void neoe_mem_Lv1_doDivide(neoe_mem_Lv1* self, neoe_mem_Envelop* b, uint64_t rest) ;


	/*public*/
 uint64_t neoe_mem_Lv1_enlarge(neoe_mem_Lv1* self, uint64_t addr, uint64_t newsize) ;


	/**
	 * 
	 * @param addr
	 * @return debug info, ignore by user
	 */
	/*public*/
 uint64_t neoe_mem_Lv1_free(neoe_mem_Lv1* self, uint64_t addr) ;


	/*private*/
 void neoe_mem_Lv1_doMergeNext(neoe_mem_Lv1* self, uint64_t p1) ;


	/*private*/
 void neoe_mem_Lv1_doMergePrev(neoe_mem_Lv1* self, uint64_t p1) ;


	/*private*/
 void neoe_mem_Lv1_doMergeBoth(neoe_mem_Lv1* self, uint64_t p1) ;


	/*private*/
 bool neoe_mem_Lv1_canMergePrev(neoe_mem_Lv1* self, uint64_t p1) ;


	/*private*/
 bool neoe_mem_Lv1_canMergeNext(neoe_mem_Lv1* self, uint64_t p1) ;

// }/*cls*/

#endif

