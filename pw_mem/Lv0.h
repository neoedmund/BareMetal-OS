#ifndef neoe_mem_Lv0_H
#define neoe_mem_Lv0_H
// package neoe.mem;
#include <stdint.h>


#define neoe_mem_Lv0_alignInByteShift  3


// public abstract class Lv0 {
 typedef struct neoe_mem_Lv0 {

	uint64_t firstStart;
	uint64_t* e820;
	void* realObj;

	/*public*/ /*abstract*/
	void (*write4) (struct neoe_mem_Lv0* self, uint64_t addr, uint64_t v);

	/*public*/ /*abstract*/
	uint64_t (*read4) (struct neoe_mem_Lv0* self, uint64_t addr);

	/*public*/ /*abstract*/
	void (*copyRange) (struct neoe_mem_Lv0* self, uint64_t from, uint64_t to, uint64_t size);

 } neoe_mem_Lv0;


	/*public*/
 void neoe_mem_Lv0_Init(neoe_mem_Lv0* self, uint64_t* e820) ;


	/**
	 * Fix the e820, reserve memory without format.
	 *
	 * @param start
	 * @param len
	 */
	/*public*/
 void neoe_mem_Lv0_reserve(neoe_mem_Lv0* self, uint64_t p1, uint64_t plen) ;


	/*public*/
 void neoe_mem_Lv0_format(neoe_mem_Lv0* self) ;


	/*public*/
 void neoe_mem_Lv0_format0(neoe_mem_Lv0* self, uint64_t start, uint64_t len, uint64_t prev, uint64_t next) ;


	/*static*/
 /*final*/


// }/*cls*/

#endif
