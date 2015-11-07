// package neoe.mem;

#include <stdio.h>
#include <stdint.h>
#include "Lv0.h"

	/*public*/
 /*static*/
 void neoe_mem_U_klog( char* s) {
		printf("%s\n", s);
	}

	/*public*/
 /*static*/
 uint64_t neoe_mem_U_alignSize( uint64_t size) {

		uint64_t s2 = size >> neoe_mem_Lv0_alignInByteShift << neoe_mem_Lv0_alignInByteShift;
		if (s2 != size)
			s2 += 1 << neoe_mem_Lv0_alignInByteShift;

		return s2;
	}

// }/*cls*/


