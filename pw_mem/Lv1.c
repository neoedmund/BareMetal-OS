// package neoe.mem;
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include "Lv0.h"
#include "Lv1.h"
#include "U.h"
#include "Envelop.h"

#define Envelop neoe_mem_Envelop

	/*public*/
 void neoe_mem_Lv1_Init(neoe_mem_Lv1* self, neoe_mem_Lv0* lv0) {
		self->lv0 = lv0;
		//printf("firstStart1=%" PRIx64 ", addr=%" PRIx64 "\n", self->lv0->firstStart, & self->lv0->firstStart);
		//printf("firstStart2=%" PRIx64 ", addr=%" PRIx64 "\n", lv0->firstStart, & lv0->firstStart);
	}

	/*public*/
 uint64_t neoe_mem_Lv1_alloc(neoe_mem_Lv1* self, uint64_t size) {
		size = neoe_mem_U_alignSize(size);
		uint64_t p1 = self->lv0->firstStart;
		//printf("firstStart=%" PRIx64 ", addr=%" PRIx64 "\n", p1, & self->lv0->firstStart);
		Envelop _b; Envelop* b =  &_b;  neoe_mem_Envelop_Init(&_b, self->lv0, p1);
		while (true) {
			printf("reload %" PRIx64 "\n", p1);
			neoe_mem_Envelop_reload(b, p1);
			if (neoe_mem_Envelop_getType(b) != neoe_mem_Envelop_TYPE_FREE || neoe_mem_Envelop_getMaxDataLen(b) < size) {
				// try next
				p1 = neoe_mem_Envelop_getNextPos(b);
				if (p1 == 0)
					break;
				else
					continue;
			} else {
				uint64_t rest = neoe_mem_Envelop_getMaxDataLen(b) - size;
				rest = neoe_mem_Envelop_alignLow(b, rest);
				if (rest >= neoe_mem_Envelop_OverheadSize + neoe_mem_Envelop_MinDataSize) {
					// divide
					neoe_mem_Lv1_doDivide(self, b, rest);
					neoe_mem_Lv1_doUse(self, b, size);
					break;
				} else {
					// occupy whole one
					neoe_mem_Lv1_doUse(self, b, size);
					break;
				}
			}
		}
		if (p1 == 0) {
			// fail
			neoe_mem_U_klog("alloc,OutOfMemory!\n");
			return 0;
		}
		return neoe_mem_Envelop_toUserAddress(p1);
	}

	/*private*/
 void neoe_mem_Lv1_doUse( neoe_mem_Lv1* self, Envelop* b, uint64_t size) {
		neoe_mem_Envelop_setType(b, neoe_mem_Envelop_TYPE_USE);
		neoe_mem_Envelop_setP1(b, 0);// tail change friendly than head
		neoe_mem_Envelop_setP2(b, size);
	}

	/*private*/
 void neoe_mem_Lv1_doDivide( neoe_mem_Lv1* self, Envelop* b, uint64_t rest) {
		printf("doDivide\n");
		// assert( rest >= neoe_mem_Envelop_OverheadSize + neoe_mem_Envelop_MinDataSize)
		uint64_t nts = neoe_mem_Envelop_getTotalSize1(b) - rest;
		// long nmds = b.getMaxDataLen() - rest;
		neoe_mem_Envelop_setSize(b, nts);
		uint64_t p2 = b->start + neoe_mem_Envelop_getTotalSize1(b);
		neoe_mem_Lv0_format0(self->lv0, p2, rest, b->start, neoe_mem_Envelop_getNextPos(b));
		neoe_mem_Envelop_setNextPos(b, p2);
	}

	/*public*/
 uint64_t neoe_mem_Lv1_enlarge(neoe_mem_Lv1* self, uint64_t addr, uint64_t newsize) {
		// final long newsize0 = newsize;
		newsize = neoe_mem_U_alignSize(newsize);
		uint64_t p1 = neoe_mem_Envelop_fromUserAddress(addr);
		Envelop _b; Envelop* b =  &_b;  neoe_mem_Envelop_Init(&_b, self->lv0, p1);
		uint64_t osize = neoe_mem_Envelop_getCurrentDataLen(b);
		if (newsize <= osize) {// ①
			neoe_mem_U_klog("no need to enlarge!");
			return addr;
		}
		uint64_t osize2 = neoe_mem_Envelop_getMaxDataLen(b);
		if (newsize <= osize2) {
			// enlarge in-place②
			// assert(p0==0)
			neoe_mem_Envelop_setP2(b, 0 + newsize);
			printf("[D]enlarge inplace");
			return addr;
		}

		{ // ③ extender to next
			uint64_t p1s = b->start + neoe_mem_Envelop_getTotalSize1(b);
			if (p1s == neoe_mem_Envelop_getNextPos(b)) { // adj
				Envelop _b2; Envelop* b2 =  &_b2;  neoe_mem_Envelop_Init(&_b2, self->lv0, p1s);
				if (neoe_mem_Envelop_getType(b2) == neoe_mem_Envelop_TYPE_FREE && neoe_mem_Envelop_getMaxDataLen(b) + neoe_mem_Envelop_getMaxDataLen(b2) >= newsize) {// p0=0
					uint64_t nsize = neoe_mem_Envelop_getTotalSize1(b) + neoe_mem_Envelop_getTotalSize1(b2);
					neoe_mem_Envelop_setSize(b, nsize);
					neoe_mem_Envelop_setNextPos(b, neoe_mem_Envelop_getNextPos(b2));
					neoe_mem_Envelop_setP2(b, 0 + newsize);
					uint64_t ndlen = neoe_mem_Envelop_getMaxDataLen(b);
					printf("[D]enlarge to next");
					if (ndlen - newsize >= neoe_mem_Envelop_MinDataSize + neoe_mem_Envelop_OverheadSize) {
						uint64_t rest = ndlen - newsize;
						neoe_mem_Lv1_doDivide(self, b, rest);
					}
					return addr;
				}
			}
		}

		{// ④
			uint64_t p2 = neoe_mem_Lv1_alloc(self, newsize);
			if (p2 == 0) {
				neoe_mem_U_klog("enlarge, but no memory!");
				return 0;
			}

			self->lv0->copyRange(self->lv0, addr, p2, osize);
			neoe_mem_Lv1_free(self, addr);
			return p2;
		}

	}

	/**
	 * 
	 * @param addr
	 * @return debug info, ignore by user
	 */
	/*public*/
 uint64_t neoe_mem_Lv1_free(neoe_mem_Lv1* self, uint64_t addr) {
		uint64_t p1 = neoe_mem_Envelop_fromUserAddress(addr);
		bool mergePrev = neoe_mem_Lv1_canMergePrev(self, p1);
		bool mergeNext = neoe_mem_Lv1_canMergeNext(self, p1);
		if (mergePrev && mergeNext) {
			neoe_mem_Lv1_doMergeBoth(self, p1);
			return 3;
		} else if (mergePrev) {
			neoe_mem_Lv1_doMergePrev(self, p1);
			return 1;
		} else if (mergeNext) {
			neoe_mem_Lv1_doMergeNext(self, p1);
			return 2;
		} else { // no merge
			Envelop localvar_x1;  neoe_mem_Envelop_Init(&localvar_x1, self->lv0, p1);
			neoe_mem_Envelop_setType(&localvar_x1, neoe_mem_Envelop_TYPE_FREE);
			return 0;
		}
	}

	/*private*/
 void neoe_mem_Lv1_doMergeNext(neoe_mem_Lv1* self, uint64_t p1) {
		Envelop _b; Envelop* b =  &_b;  neoe_mem_Envelop_Init(&_b, self->lv0, p1);
		Envelop _b2; Envelop* b2 =  &_b2;  neoe_mem_Envelop_Init(&_b2, self->lv0, p1 + neoe_mem_Envelop_getTotalSize1(b));
		uint64_t nsize = neoe_mem_Envelop_getTotalSize1(b) + neoe_mem_Envelop_getTotalSize1(b2);
		neoe_mem_Envelop_setSize(b, nsize);
		neoe_mem_Envelop_setType(b, neoe_mem_Envelop_TYPE_FREE);
		neoe_mem_Envelop_setNextPos(b, neoe_mem_Envelop_getNextPos(b2));
	}

	/*private*/
 void neoe_mem_Lv1_doMergePrev(neoe_mem_Lv1* self, uint64_t p1) {
		Envelop _b; Envelop* b =  &_b;  neoe_mem_Envelop_Init(&_b, self->lv0, p1);
		uint64_t prevTotalSize = self->lv0->read4(self->lv0, p1 - 8);
		uint64_t nsize = neoe_mem_Envelop_getTotalSize1(b) + prevTotalSize;
		uint64_t p2 = p1 - prevTotalSize;
		Envelop _b2; Envelop* b2 =  &_b2;  neoe_mem_Envelop_Init(&_b2, self->lv0, p2);
		neoe_mem_Envelop_setSize(b2, nsize);
		neoe_mem_Envelop_setType(b2, neoe_mem_Envelop_TYPE_FREE);
		neoe_mem_Envelop_setNextPos(b2, neoe_mem_Envelop_getNextPos(b));
	}

	/*private*/
 void neoe_mem_Lv1_doMergeBoth(neoe_mem_Lv1* self, uint64_t p1) {
		Envelop _b; Envelop* b =  &_b;  neoe_mem_Envelop_Init(&_b, self->lv0, p1);
		uint64_t p3 = p1 + neoe_mem_Envelop_getTotalSize1(b);
		uint64_t nextTotalSize = self->lv0->read4(self->lv0, p3);
		uint64_t prevTotalSize = self->lv0->read4(self->lv0, p1 - 8);
		uint64_t nsize = neoe_mem_Envelop_getTotalSize1(b) + nextTotalSize + prevTotalSize;
		uint64_t p2 = p1 - prevTotalSize;
		Envelop localvar_x1, localvar_x2;
		Envelop* bprev =  &localvar_x1;  neoe_mem_Envelop_Init(&localvar_x1, self->lv0, p2);
		Envelop* bnext =  &localvar_x2;  neoe_mem_Envelop_Init(&localvar_x2, self->lv0, p3);
		neoe_mem_Envelop_setSize(bprev, nsize);
		neoe_mem_Envelop_setType(bprev, neoe_mem_Envelop_TYPE_FREE);
		neoe_mem_Envelop_setNextPos(bprev, neoe_mem_Envelop_getNextPos(bnext));
	}

	/*private*/
 bool neoe_mem_Lv1_canMergePrev(neoe_mem_Lv1* self, uint64_t p1) {
		Envelop _b; Envelop* b =  &_b;  neoe_mem_Envelop_Init(&_b, self->lv0, p1);
		uint64_t p2 = neoe_mem_Envelop_getPrevPos(b);
		if (p2 == 0)
			return false;
		neoe_mem_Envelop_reload(b, p2);
		if (neoe_mem_Envelop_getType(b) != neoe_mem_Envelop_TYPE_FREE)
			return false;
		// printf("%" PRIx64 "+%" PRIx64 "=%" PRIx64 "!=%" PRIx64 "\n", p2, b.getTotalSize1(), p2 +
		// b.getTotalSize1(), p1);
		return p2 + neoe_mem_Envelop_getTotalSize1(b) == p1;
	}

	/*private*/
 bool neoe_mem_Lv1_canMergeNext(neoe_mem_Lv1* self, uint64_t p1) {
		Envelop _b; Envelop* b =  &_b;  neoe_mem_Envelop_Init(&_b, self->lv0, p1);
		uint64_t p2 = neoe_mem_Envelop_getNextPos(b);
		if (p2 == 0)
			return false;
		Envelop _b2; Envelop* b2 =  &_b2;  neoe_mem_Envelop_Init(&_b2, self->lv0, p2);
		if (neoe_mem_Envelop_getType(b2) != neoe_mem_Envelop_TYPE_FREE)
			return false;
		return p1 + neoe_mem_Envelop_getTotalSize1(b) == p2;
	}
// }/*cls*/


