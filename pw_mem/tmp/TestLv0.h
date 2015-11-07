#ifndef _neoe_mem_TestLv0_H
#define _neoe_mem_TestLv0_H
// package neoe.mem;

#include <stdint.h>

#include "Lv0.h"
#include "uthash.h"  // hashmap!

// import java.util.HashMap;
// import java.util.Map;

#define So64 sizeof( uint64_t )
#define  neoe_mem_TestLv0_EmuBlockSizeInUnit 256

// public class TestLv0 extends Lv0* {

typedef struct  {
    uint64_t id;
    uint64_t* buf;
    UT_hash_handle hh;         /* makes this structure hashable */
} Entry;

typedef struct neoe_mem_TestLv0{
	neoe_mem_Lv0* parent;  // order is important, must be first one, for upcast

	Entry* emuData;

 } neoe_mem_TestLv0;


	/*public*/
 void neoe_mem_TestLv0_Init(neoe_mem_TestLv0* self, uint64_t* e820) ;


	/*@Override*/
	/*public*/
 void neoe_mem_TestLv0_write4(neoe_mem_Lv0* self, uint64_t addr, uint64_t v) ;


	/*@Override*/

	/*public*/
 uint64_t neoe_mem_TestLv0_read4(neoe_mem_Lv0* self, uint64_t addr) ;


	/*@Override*/

	/*public*/
 void neoe_mem_TestLv0_copyRange(neoe_mem_Lv0* self, uint64_t from, uint64_t to, uint64_t size) ;


	/*private*/
 void neoe_mem_TestLv0_warn(neoe_mem_TestLv0* self, char* s) ;
	/*private*/
 void neoe_mem_TestLv0_debug(neoe_mem_TestLv0* self, char* s) ;


// }/*cls*/

#endif

