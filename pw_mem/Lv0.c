#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "Lv0.h"
#include "Envelop.h"

// package neoe.mem;

#define  neoe_mem_Lv0_alignInByteShift  3
	/*public*/
 void neoe_mem_Lv0_Init(neoe_mem_Lv0* self, uint64_t*  e820) {
		self->e820 = e820;
	}

	/**
	 * Fix the e820, reserve memory without format.
	 * 
	 * @param start
	 * @param len
	 */
	/*public*/
 void neoe_mem_Lv0_reserve(neoe_mem_Lv0* self, uint64_t p1, uint64_t plen) {
		// used to reserve low memory, this impl has limit, do not divide, only
		// change the start addr and size
		uint64_t p2 = p1 + plen;
		for (uint32_t i = 0;; i++) {
			uint64_t start = self->e820[i * 2];
			uint64_t len = self->e820[i * 2 + 1];
			if (start == 0 && len == 0)
				break;
			if (start < p2 && start + len > p2) {
				// [q1 , p2 , q2]
				printf("patch e820 entry %d\n" , i);
				self->e820[i * 2] = p2;
				self->e820[i * 2 + 1] = start + len - p2;
			} else if (p1 <= start && p2 >= start + len) {
				// [p1, q1, q2, p2]
				printf("patch(0) e820 entry %d" , i);
				self->e820[i * 2 + 1] = 0;
			}
		}
	}

	/*public*/
 void neoe_mem_Lv0_format(neoe_mem_Lv0* self) {

		self->firstStart = self->e820[0];
		for (uint32_t i = 0;; i++) {
			uint64_t start = self->e820[i * 2];
			uint64_t len = self->e820[i * 2 + 1];
			if (len == 0 && start == 0)
				break;
			if (len < neoe_mem_Envelop_MinDataSize + neoe_mem_Envelop_OverheadSize)
				continue;
			uint64_t prev = 0;
			if (i > 0) {
				prev = self->e820[(i - 1) * 2];
			}
			uint64_t next = 0;
			next = self->e820[(i + 1) * 2];

			neoe_mem_Lv0_format0(self, start, len, prev, next);
			
		}
	}

	/*public*/
 void neoe_mem_Lv0_format0(neoe_mem_Lv0* self, uint64_t start, uint64_t len, uint64_t prev, uint64_t next) {
		printf("[D]format %" PRIx64 ",%" PRIx64 ",%" PRIx64 ",%" PRIx64 "\n", start, len, prev, next);
		if (len < neoe_mem_Envelop_MinDataSize + neoe_mem_Envelop_OverheadSize) {
			/*U->klog*/ printf("bug:format len too small");
		}
		self->write4(self, start, len);
		self->write4(self, start + 8, neoe_mem_Envelop_Signature);
		start += neoe_mem_Envelop_patch;
		self->write4(self, start + 8, prev);
		self->write4(self, start + 16, next);
		self->write4(self, start + 24, 0);
		self->write4(self, start + 32, len - neoe_mem_Envelop_OverheadSize);
		self->write4(self, start + 40, 0);
		self->write4(self, start + 48, 0);
		start -= neoe_mem_Envelop_patch;
		self->write4(self, start + len - 8, len);
	}

	/*static*/
 /*final*/



// }/*cls*/


