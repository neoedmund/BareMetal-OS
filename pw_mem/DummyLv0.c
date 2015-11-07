#ifndef neoe_mem_DummyLv0_H
#define neoe_mem_DummyLv0_H

// package neoe.mem;

// public class DummyLv0 extends Lv0* {

#include <stdint.h>
#include "std.h"
#include "Lv0.h"
#include "DummyLv0.h"

	/*public*/
 void DummyLv0_Init(neoe_mem_Lv0* self) {
		neoe_mem_Lv0_Init(self, NULL);
		self->write4=neoe_mem_DummyLv0_write4;
		self->read4=neoe_mem_DummyLv0_read4;
		self->copyRange=neoe_mem_DummyLv0_copyRange;
	}

	/*@Override*/

	/*public*/
 void neoe_mem_DummyLv0_write4(neoe_mem_Lv0* self, uint64_t addr, uint64_t v) {
		printf("dummy write\n");

	}

	/*@Override*/

	/*public*/
 uint64_t neoe_mem_DummyLv0_read4(neoe_mem_Lv0* self, uint64_t addr) {
		printf("dummy read\n");
		return 0;
	}

	/*@Override*/

	/*public*/
 void neoe_mem_DummyLv0_copyRange(neoe_mem_Lv0* self, uint64_t from, uint64_t to, uint64_t size) {
		printf("dummy copyRange\n");
	}

// }/*cls*/

#endif
