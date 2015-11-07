#ifndef _neoe_mem_Lv0Neos_H
#define _neoe_mem_Lv0Neos_H

#include <stdint.h>

#include "Lv0.h"


typedef struct neoe_mem_Lv0Neos {
	neoe_mem_Lv0* parent;
 } neoe_mem_Lv0Neos;


	/*public*/
 void neoe_mem_Lv0Neos_Init(neoe_mem_Lv0Neos* self, uint64_t* e820) ;


	/*@Override*/
 void neoe_mem_Lv0Neos_write4(neoe_mem_Lv0* self, uint64_t addr, uint64_t v) ;


	/*@Override*/
 uint64_t neoe_mem_Lv0Neos_read4(neoe_mem_Lv0* self, uint64_t addr) ;


	/*@Override*/
 void neoe_mem_Lv0Neos_copyRange(neoe_mem_Lv0* self, uint64_t from, uint64_t to, uint64_t size) ;



#endif

