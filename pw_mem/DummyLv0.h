#ifndef _neoe_mem_DummyLv0_H
#define _neoe_mem_DummyLv0_H
// package neoe.mem;

// public class DummyLv0 extends Lv0* {

#include <stdint.h>
#include "Lv0.h"

	/*public*/
 void DummyLv0_Init(neoe_mem_Lv0* self) ;


	/*@Override*/

	/*public*/
 void neoe_mem_DummyLv0_write4(neoe_mem_Lv0* self, uint64_t addr, uint64_t v) ;


	/*@Override*/

	/*public*/
 uint64_t neoe_mem_DummyLv0_read4(neoe_mem_Lv0* self, uint64_t addr) ;


	/*@Override*/

	/*public*/
 void neoe_mem_DummyLv0_copyRange(neoe_mem_Lv0* self, uint64_t from, uint64_t to, uint64_t size) ;


// }/*cls*/

#endif
