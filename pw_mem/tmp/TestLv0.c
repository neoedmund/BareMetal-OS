// package neoe.mem;

// import java.util.HashMap;
// import java.util.Map;

// public class TestLv0 extends Lv0* {

#include <stdio.h>
#include <inttypes.h>
#include "U.h"
#include "Lv0.h"
#include "TestLv0.h"
#include "uthash.h"  // hashmap!




	/*public*/
 void neoe_mem_TestLv0_Init(neoe_mem_TestLv0* self, uint64_t* e820) {
		self->parent->realObj = self;
		neoe_mem_Lv0_Init(self->parent, e820);
		self->parent->write4   = neoe_mem_TestLv0_write4;
		self->parent->read4   = neoe_mem_TestLv0_read4;
		self->parent->copyRange   = neoe_mem_TestLv0_copyRange;
		self->emuData = NULL;
		
	}

	/*final*/
 /*static*/
	// Map*<Long*, long*[]> emuData =  _localvar_x;  HashMap<Long,long[]>_Init(&_localvar_x, Long*, long*[]>();

	/*@Override*/
	/*public*/
 void neoe_mem_TestLv0_write4(neoe_mem_Lv0* _self, uint64_t addr, uint64_t v) {
		neoe_mem_TestLv0* self = _self->realObj;
		uint64_t addr0 = addr;
		addr >>= neoe_mem_Lv0_alignInByteShift;

		uint64_t blockId = addr / neoe_mem_TestLv0_EmuBlockSizeInUnit;


		//uint64_t[] buf = Map<Long,long[]>_get(self->emuData, blockId);
		Entry* entry;
		HASH_FIND( hh, self->emuData, &blockId, So64 , entry );
		if (entry == NULL) {
			//buf =  _localvar_x;  long_Init(&_localvar_x, neoe_mem_TestLv0_EmuBlockSizeInUnit];
			//Map<Long,long[]>_put(self->emuData, blockId, buf);
			uint64_t* buf = (uint64_t*) malloc( So64  * neoe_mem_TestLv0_EmuBlockSizeInUnit);
			entry = (Entry*)malloc(sizeof(Entry));
			entry->id = blockId;
			entry->buf = buf;
			HASH_ADD( hh, self->emuData, id, So64, entry );
			//neoe_mem_TestLv0_debug(self, String.format("+%x", addr0));
			printf("[d]+%" PRIx64 "\n", addr0);
		}
		uint64_t loc =   (addr % neoe_mem_TestLv0_EmuBlockSizeInUnit);
		entry->buf[loc] = v;
		//printf("[d]write %" PRIx64 "|%" PRIx64 "[%" PRIx64 "]=%" PRIx64 "\n", blockId, addr << neoe_mem_Lv0_alignInByteShift,  loc, v );
	}

	/*private*/
 void neoe_mem_TestLv0_debug(neoe_mem_TestLv0* self, char* s) {
		printf("[D]%s\n" , s);
	}

	/*@Override*/

	/*public*/
 uint64_t neoe_mem_TestLv0_read4(neoe_mem_Lv0* _self, uint64_t addr) {
		neoe_mem_TestLv0* self =   _self->realObj;
		addr >>= neoe_mem_Lv0_alignInByteShift;
		uint64_t blockId = addr / neoe_mem_TestLv0_EmuBlockSizeInUnit;
		//long*[] buf = Map<Long,long[]>_get(self->emuData, blockId);
		Entry* entry;
		HASH_FIND( hh, self->emuData, &blockId, So64 , entry );
		if (entry == NULL) {
			neoe_mem_TestLv0_warn(self, "read before write " /* addr*/);
			//buf =  _localvar_x;  long_Init(&_localvar_x, neoe_mem_TestLv0_EmuBlockSizeInUnit];
			//Map<Long,long[]>_put(self->emuData, blockId, buf);
			uint64_t* buf = (uint64_t*) malloc( So64  * neoe_mem_TestLv0_EmuBlockSizeInUnit);
			entry = (Entry*)malloc(sizeof(Entry));
			entry->id = blockId;
			entry->buf = buf;
			HASH_ADD( hh, self->emuData, id, So64, entry );
			//neoe_mem_TestLv0_debug(self, String.format("!+%x", addr));
			printf("[d]!+%" PRIx64 "\n", addr);
			return 0;
		}
		uint64_t loc =   (addr % neoe_mem_TestLv0_EmuBlockSizeInUnit);
		uint64_t v = entry->buf[loc];
		//printf("[d]read buf[%" PRIx64 "]=%" PRIx64 "\n", loc, v );
		return v;
	}

	/*@Override*/

	/*public*/
 void neoe_mem_TestLv0_copyRange(neoe_mem_Lv0* _self, uint64_t from, uint64_t to, uint64_t size) {
		//neoe_mem_TestLv0* self =  (neoe_mem_TestLv0*) _self->realObj;
		size = neoe_mem_U_alignSize(size);
		//neoe_mem_TestLv0_debug(self, String.format("copyrange[%x→%x=%x]", from, to, size));
		printf("copyrange[%" PRIx64 "→%" PRIx64 "=%" PRIx64 "]\n", from, to, size);
		for (uint64_t i = 0; i < size; i++) {
			uint64_t v = neoe_mem_TestLv0_read4(_self, from + i);
			neoe_mem_TestLv0_write4(_self, to + i, v);
		}
	}

	/*private*/
 void neoe_mem_TestLv0_warn(neoe_mem_TestLv0* self, char* s) {
		printf("[W]%s\n" , s);
	}

// }/*cls*/


