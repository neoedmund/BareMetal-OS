; this is a loader compatible with linux16 protocol, with can be loaded by Grub2


USE16

ORG 0x90000

%define CODE_SECTORS	8
%define PDE  0x40000



global  start, _start

data_start:
	jmp	linux_init

times 0x1F1-($-data_start) db 0

setup_sects:
	db	CODE_SECTORS
root_flags:
	dw	0
syssize:
	dw	0
swap_dev:
	dw	0
ram_size:
	dw	0
vid_mode:
	dw	0
root_dev:
	dw	0
boot_flag:
	dw	0xAA55

start:
_start:

	jmp linux_init

LINUX_HEAD:
	db	'HdrS'	; /*Header signature.  */
	dw	0x0203	; /*Header version number.  */

realmode_swtch:
	dw	0, 0	; /*default_switch, SETUPSEG.  */
start_sys_seg:
	dw	0x1000	; /*Obsolete.  */
version_ptr:
	dw	0	; /*Version string ptr.  */
type_of_loader:
	db	0	; /*Filled in by boot loader.  */
loadflags:
	db	1	; /*Please load high.  */
setup_move_size:
	dw	0	; /*Unused.  */
code32_start:
	dd	0x100000	; /*32-bit start address.  */
ramdisk_image:
	dd	0	; /*Loaded ramdisk image address.  */
ramdisk_size:
	dd	0	; /*Size of loaded ramdisk.  */
bootsect_kludge:
	dw	0, 0
heap_end_ptr:
	dw	0
pad1:
	dw	0
cmd_line_ptr:
	dd	0	; /*Command line.  */
ramdisk_max:
	dd	0	; /*Highest allowed ramdisk address.  */


align 16
GDTR32:		; Global Descriptors Table Register
dw gdt32_end - gdt32 - 1	; limit of GDT (size minus one)
dw gdt32-data_start,0x9 	; linear address of GDT

align 16
gdt32:
dw 0x0000, 0x0000, 0x0000, 0x0000	; Null desciptor
dw 0xFFFF, 0x0000, 0x9A00, 0x00CF	; 32-bit code descriptor
dw 0xFFFF, 0x0000, 0x9200, 0x00CF	; 32-bit data descriptor
gdt32_end:


linux_init:
	cli
	cld
;;	xor eax, eax
;;	xor ebx, ebx
;;	xor ecx, ecx
;;	xor edx, edx
;;	xor esi, esi
;;	xor edi, edi
;;	xor ebp, ebp
;;	mov ds, ax
;;	mov es, ax
;;	mov ss, ax
;;	mov fs, ax
;;	mov gs, ax
;;	mov esp, 0x8000	; Set a known free location for the stack
;;
;;	sti

jmp 0x9000:here1-data_start
here1:

ap_modify:
	jmp start16	; This command will be overwritten with 'NOP's before the AP's are started
	nop	; The 'jmp' is only 3 bytes

;;%include "init/smp_ap.asm"	; AP's will start execution at 0x8000 and fall through to this code

align 16
use16
start16:

; Configure serial port
	xor dx, dx	; First serial port
	mov ax, 0000000011100011b	; 9600 baud, no parity, 1 stop bit, 8 data bits
	int 0x14

; Make sure the screen is set to 80x25 color text mode
	mov ax, 0x0003	; Set to normal (80x25 text) video mode
	int 0x10

; Disable blinking
;	mov ax, 0x1003
;	mov bx, 0x0000
;	int 0x10

; Print message
	mov si, msg_initializing
	call print_string_16

	mov ax, 0xB800
	mov ds, ax
	mov al, 'a'
	mov di,0x9E
	mov [di], al


; Check to make sure the CPU supports 64-bit mode... If not then bail out
	mov eax, 0x80000000	; Extended-function 8000000h.
	cpuid	; Is largest extended function
	cmp eax, 0x80000000	; any function > 80000000h?
	jbe no_long_mode	; If not, no long mode.
	mov eax, 0x80000001	; Extended-function 8000001h.
	cpuid	; Now EDX = extended-features flags.
	bt edx, 29	; Test if long mode is supported.
	jnc no_long_mode	; Exit if not supported.

	mov ax, 0xB800
	mov ds, ax
	mov al, 'b'
	mov di,0x9E
	mov [di], al


	call init_isa	; Setup legacy hardware

	mov ax, 0xB800
	mov ds, ax
	mov al, 'c'
	mov di,0x9E
	mov [di], al

; Hide the hardware cursor (interferes with print_string_16 if called earlier)
;	mov ax, 0x0200	; VIDEO - SET CURSOR POSITION
;	mov bx, 0x0000	; Page number
;	mov dx, 0x2000	; Row / Column
;	int 0x10


; At this point we are done with real mode and BIOS interrupts. Jump to 32-bit mode.
;;	xor eax,eax
;;	xor edx,edx
;;	call next
;;next:
;;	pop ax
;;	sub ax, next-GDTR32
;;	mov dx, cs
;;	shl edx, 4
;;	add eax, edx
	mov ax, 0x9000
	mov ds, ax
	lgdt [ds:GDTR32-data_start]	; Load GDT register

	mov eax, cr0
	or al, 0x01	; Set protected mode bit
	mov cr0, eax

;;;.byte   0x66, 0xea
;;;code32: .long   0x1000          // overwritten in Section 4.7
;;;        .word   __KERNEL_CS     // segment 0x10
;;;        // see linux/arch/i386/boot/compressed/head.S:startup_32
;;	jmp 8:start32-data_start+0x90000	; Jump to 32-bit protected mode
JUMP32:
	db  0x66, 0xea
	dd  0x90000 + start32 - data_start
	dw  8

; 16-bit function to print a sting to the screen
print_string_16:	; Output string in SI to screen
	pusha
	mov ah, 0x0E	; http://www.ctyme.com/intr/rb-0106.htm
print_string_16_repeat:
	lodsb	; Get char from string
	cmp al, 0
	je print_string_16_done	; If char is zero, end of string
	int 0x10	; Otherwise, print it
	jmp print_string_16_repeat
print_string_16_done:
	popa
	ret

; Display an error message that the CPU does not support 64-bit mode
no_long_mode:
	mov si, msg_no64
	call print_string_16
	jmp $

%include "init/isa.asm"



align 16

; =============================================================================
; 32-bit mode
USE32

start32:
	cli

	mov eax, 16	; load 4 GB data descriptor
	mov ds, ax	; to all data segment registers
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx
	xor esi, esi
	xor edi, edi
	xor ebp, ebp
	mov esp, PDE-0x1000	; Set a known free location for the stack

	cld

%macro debuglog 2

	push eax
	mov al, %1
	mov [0x000B809C], al
	mov al, %2
	mov [0x000B809E], al
	pop eax

%endmacro

%macro debuglog64 2

	push rax
	mov al, %1
	mov [0x000B809C], al
	mov al, %2
	mov [0x000B809E], al
	pop rax

%endmacro

	debuglog '3', '2'

; Clear out the first 4096 bytes of memory. This will store the 64-bit IDT, GDT, PML4, and PDP
	mov ecx, 1024
	xor eax, eax
	mov edi, eax
	rep stosd

	debuglog 'N', '1'

; Clear memory for the Page Descriptor Entries (PDE - PDE+0x3FFFF)
	mov edi, PDE
	mov ecx, 65536
	rep stosd

	debuglog 'N', '2'




; Copy the GDT to its final location in memory
	mov esi, 0x90000 + gdt64 -data_start
	mov edi, 0x00001000	; GDT address
	mov ecx, (gdt64_end - gdt64)
	rep movsb	; Move it to final pos.

	debuglog 'N', '3'


; Create the Level 4 Page Map. (Maps 4GBs of 2MB pages)
; First create a PML4 entry.
; PML4 is stored at 0x0000000000002000, create the first entry there
; A single PML4 entry can map 512GB with 2MB pages.
	cld
	mov edi, 0x00002000	; Create a PML4 entry for the first 4GB of RAM
	mov eax, 0x00003007
	stosd
	xor eax, eax
	stosd

	mov edi, 0x00002800	; Create a PML4 entry for higher half (starting at 0xFFFF800000000000)
	mov eax, 0x00003007	; The higher half is identity mapped to the lower half
	stosd
	xor eax, eax
	stosd

; Create the PDP entries.
; The first PDP is stored at 0x0000000000003000, create the first entries there
; A single PDP entry can map 1GB with 2MB pages
	mov ecx, 64	; number of PDPE's to make.. each PDPE maps 1GB of physical memory
	mov edi, 0x00003000
	mov eax, PDE+7	; location of first PD
create_pdpe:
	stosd
	push eax
	xor eax, eax
	stosd
	pop eax
	add eax, 0x00001000	; 4K later (512 records x 8 bytes)
	dec ecx
	cmp ecx, 0
	jne create_pdpe

; Create the PD entries.
; PD entries are stored starting at (PDE - PDE+0x3FFFF) (256 KiB)
; This gives us room to map 64 GiB with 2 MiB pages
	mov edi, PDE
	mov eax, 0x000000C7 ; not 8F?	; Bit 7 must be set to 1 as we have 2 MiB pages
	xor ecx, ecx
pd_again:	; Create a 2 MiB page
	stosd
	push eax
	xor eax, eax
	stosd
	pop eax
	add eax, 0x00200000
	inc ecx
	cmp ecx, 2048
	jne pd_again	; Create 2048 2 MiB page maps.


	debuglog 'M', '1'

; Load the GDT
	lgdt [GDTR64-data_start+0x90000]

; Enable extended properties
	mov eax, cr4
	or eax, 0x0000000B0	; PGE (Bit 7), PAE (Bit 5), and PSE (Bit 4)
	mov cr4, eax

; Point cr3 at PML4
	mov eax, 0x00002008	; Write-thru (Bit 3)
	mov cr3, eax

; Enable long mode and SYSCALL/SYSRET
	mov ecx, 0xC0000080	; EFER MSR number
	rdmsr	; Read EFER
	or eax, 0x00000101 	; LME (Bit 8)
	wrmsr	; Write EFER

	debuglog 'B', 'J'


; Enable paging to activate long mode
	mov eax, cr0
	or eax, 0x80000000	; PG (Bit 31)
	mov cr0, eax

	jmp SYS64_CODE_SEL:0x90000+start64-data_start	; Jump to 64-bit mode

align 16

; =============================================================================
; 64-bit mode
USE64

start64:

	debuglog64 '6', '4'

	mov al, 2
	mov ah, 22
	call os_move_cursor

	xor rax, rax	; aka r0
	xor rbx, rbx	; aka r3
	xor rcx, rcx	; aka r1
	xor rdx, rdx	; aka r2
	xor rsi, rsi	; aka r6
	xor rdi, rdi	; aka r7
	xor rbp, rbp	; aka r5
	mov rsp, PDE-0x1000	; aka r4
	xor r8, r8
	xor r9, r9
	xor r10, r10
	xor r11, r11
	xor r12, r12
	xor r13, r13
	xor r14, r14
	xor r15, r15

	mov ds, ax	; Clear the legacy segment registers
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

;;	mov rax, clearcs64-data_start+0x90000	; Do a proper 64-bit jump. Should not be needed as the ...
;;	jmp rax	; jmp SYS64_CODE_SEL:start64 would have sent us ...
;;	nop	; out of compatibility mode and into 64-bit mode
;;clearcs64:
;;	xor rax, rax

	lgdt [GDTR64-data_start+0x90000]	; Reload the GDT

	debuglog64 'A', '1'

;(?)
; Patch Pure64 AP code	; The AP's will be told to start execution at 0x8000
	mov edi, ap_modify -data_start + 0x90000	; We need to remove the BSP Jump call to get the AP's
	mov eax, 0x90909090	; to fall through to the AP Init code
	stosd

; Build the rest of the page tables (4GiB+)
	mov rcx, 0x0000000000000000
	mov rax, 0x00000001000000C7 ; not 8F? /C7
	mov rdi, PDE + 0x4000 ; not: 0x0000000000014000

buildem:
	stosq
	add rax, 0x0000000000200000
	add rcx, 1
	cmp rcx, 30720	; Another 60 GiB (We already mapped 4 GiB)
	jne buildem
	; We have 64 GiB mapped now (fixme, for more?)

	debuglog64 'A', '2'


; Build a temporary IDT
	xor rdi, rdi 	; create the 64-bit IDT (at linear address 0x0000000000000000)

	mov rcx, 32
make_exception_gates: 	; make gates for exception handlers
	mov rax, exception_gate
	push rax	; save the exception gate to the stack for later use
	stosw	; store the low word (15..0) of the address
	mov ax, SYS64_CODE_SEL
	stosw	; store the segment selector
	mov ax, 0x8E00
	stosw	; store exception gate marker
	pop rax	; get the exception gate back
	shr rax, 16
	stosw	; store the high word (31..16) of the address
	shr rax, 16
	stosd	; store the extra high dword (63..32) of the address.
	xor rax, rax
	stosd	; reserved
	dec rcx
	jnz make_exception_gates

	mov rcx, 256-32
make_interrupt_gates: 	; make gates for the other interrupts
	mov rax, interrupt_gate
	push rax	; save the interrupt gate to the stack for later use
	stosw	; store the low word (15..0) of the address
	mov ax, SYS64_CODE_SEL
	stosw	; store the segment selector
	mov ax, 0x8F00
	stosw	; store interrupt gate marker
	pop rax	; get the interrupt gate back
	shr rax, 16
	stosw	; store the high word (31..16) of the address
	shr rax, 16
	stosd	; store the extra high dword (63..32) of the address.
	xor rax, rax
	stosd	; reserved
	dec rcx
	jnz make_interrupt_gates

	; Set up the exception gates for all of the CPU exceptions
	; The following code will be seriously busted if the exception gates are moved above 16MB
	mov word [0x00*16], exception_gate_00
	mov word [0x01*16], exception_gate_01
	mov word [0x02*16], exception_gate_02
	mov word [0x03*16], exception_gate_03
	mov word [0x04*16], exception_gate_04
	mov word [0x05*16], exception_gate_05
	mov word [0x06*16], exception_gate_06
	mov word [0x07*16], exception_gate_07
	mov word [0x08*16], exception_gate_08
	mov word [0x09*16], exception_gate_09
	mov word [0x0A*16], exception_gate_10
	mov word [0x0B*16], exception_gate_11
	mov word [0x0C*16], exception_gate_12
	mov word [0x0D*16], exception_gate_13
	mov word [0x0E*16], exception_gate_14
	mov word [0x0F*16], exception_gate_15
	mov word [0x10*16], exception_gate_16
	mov word [0x11*16], exception_gate_17
	mov word [0x12*16], exception_gate_18
	mov word [0x13*16], exception_gate_19

	mov rdi, 0x21	; Set up Keyboard handler
	mov rax, keyboard
	call create_gate
	mov rdi, 0x22	; Set up Cascade handler
	mov rax, cascade
	call create_gate
	mov rdi, 0x28	; Set up RTC handler
	mov rax, rtc
	call create_gate

	lidt [IDTR64]	; load IDT register

	debuglog64 'A', '4'

; Clear memory 0xf000 - 0xf7ff for the infomap (2048 bytes)
	xor rax, rax
	mov rcx, 256
	mov rdi, 0x000000000000F000
clearmapnext:
	stosq
	dec rcx
	cmp rcx, 0
	jne clearmapnext

	debuglog64 'B', '1'

	call init_acpi	; Find and process the ACPI tables

	debuglog64 'B', '2'

	call init_cpu	; Configure the BSP CPU

	debuglog64 'B', '3'

	call init_pic	; Configure the PIC(s), also activate interrupts

	debuglog64 'B', '4'

; Make sure exceptions are working.
;	xor rax, rax
;	xor rbx, rbx
;	xor rcx, rcx
;	xor rdx, rdx
;	div rax

;; ; Init of SMP
;; 	call init_smp
;;
;; ; Reset the stack to the proper location (was set to 0x8000 previously)
;; 	mov rsi, [os_LocalAPICAddress]	; We would call os_smp_get_id here but the stack is not ...
;; 	add rsi, 0x20	; ... yet defined. It is safer to find the value directly.
;; 	lodsd	; Load a 32-bit value. We only want the high 8 bits
;; 	shr rax, 24	; Shift to the right and AL now holds the CPU's APIC ID
;; 	shl rax, 10	; shift left 10 bits for a 1024byte stack
;; 	add rax, 0x0000000000050400	; stacks decrement when you "push", start at 1024 bytes in
;; 	mov rsp, rax	; Pure64 leaves 0x50000-0x9FFFF free so we use that

	debuglog64 'B', '6'

; Calculate amount of usable RAM from Memory Map
	xor rcx, rcx
	mov rsi, 0x0000000000004000	; E820 Map location
readnextrecord:
	lodsq
	lodsq
	lodsd
	cmp eax, 0	; Are we at the end?
	je endmemcalc
	cmp eax, 1	; Useable RAM
	je goodmem
	cmp eax, 3	; ACPI Reclaimable
	je goodmem
	cmp eax, 6	; BIOS Reclaimable
	je goodmem
	lodsd
	lodsq
	jmp readnextrecord
goodmem:
	sub rsi, 12
	lodsq
	add rcx, rax
	lodsq
	lodsq
	jmp readnextrecord

endmemcalc:
	shr rcx, 20	; Value is in bytes so do a quick divide by 1048576 to get MiB's
	add ecx, 1	; The BIOS will usually report actual memory minus 1
	and ecx, 0xFFFFFFFE	; Make sure it is an even number (in case we added 1 to an even number)
	mov dword [mem_amount], ecx

	debuglog64 'C', '1'

; Convert CPU speed value to string
	xor rax, rax
	mov ax, [cpu_speed]
	mov rdi, speedtempstring
	call os_int_to_string

; Convert CPU amount value to string
	xor rax, rax
	mov ax, [cpu_activated]
	mov rdi, cpu_amount_string
	call os_int_to_string

; Convert RAM amount value to string
	xor rax, rax
	mov eax, [mem_amount]
	mov rdi, memtempstring
	call os_int_to_string

; Build the infomap
	xor rdi, rdi
	mov di, 0x5000
	mov rax, [os_ACPITableAddress]
	stosq
	mov eax, [os_BSP]
	stosd

	mov di, 0x5010
	mov ax, [cpu_speed]
	stosw
	mov ax, [cpu_activated]
	stosw
	mov ax, [cpu_detected]
	stosw

	mov di, 0x5020
	mov ax, [mem_amount]
	stosd

	mov di, 0x5030
	mov al, [os_IOAPICCount]
	stosb

	mov di, 0x5040
	mov rax, [os_HPETAddress]
	stosq

	mov di, 0x5060
	mov rax, [os_LocalAPICAddress]
	stosq
	xor ecx, ecx
	mov cl, [os_IOAPICCount]
	mov rsi, os_IOAPICAddress
nextIOAPIC:
	lodsq
	stosq
	sub cl, 1
	cmp cl, 0
	jne nextIOAPIC

	mov di, 0x5080
	mov eax, [VBEModeInfoBlock.PhysBasePtr]	; Base address of video memory (if graphics mode is set)
	stosd
	mov eax, [VBEModeInfoBlock.XResolution]	; X and Y resolution (16-bits each)
	stosd
	mov al, [VBEModeInfoBlock.BitsPerPixel]	; Color depth
	stosb

; Initialization is now complete... write a message to the screen
	mov rsi, msg_done
	call os_print_string

	debuglog64 'D', '1'

; Print info on CPU and MEM
	mov ax, 0x0004
	call os_move_cursor
	mov rsi, msg_CPU
	call os_print_string
	mov rsi, speedtempstring
	call os_print_string
	mov rsi, msg_mhz
	call os_print_string
	mov rsi, cpu_amount_string
	call os_print_string
	mov rsi, msg_MEM
	call os_print_string
	mov rsi, memtempstring
	call os_print_string
	mov rsi, msg_mb
	call os_print_string


;; neoe: load by grub, not here ; Move the trailing binary to its final location
;; 	mov rsi, 0x8000+6144	; Memory offset to end of pure64.sys
;; 	mov rdi, 0x100000	; Destination address at the 1MiB mark
;; 	mov rcx, 0xa000	; For up to 320Kb
;; 	rep movsq	; Copy 8 bytes at a time

; Print a message that the kernel is being started
	mov ax, 0x0006
	call os_move_cursor
	mov rsi, msg_startingkernel
	call os_print_string

	debuglog64 ' ', ' ' ;  clear debug info

; Clear all registers (skip the stack pointer)
	xor eax, eax	; These 32-bit calls also clear the upper bits of the 64-bit registers
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx
	xor esi, esi
	xor edi, edi
	xor ebp, ebp
	xor r8, r8
	xor r9, r9
	xor r10, r10
	xor r11, r11
	xor r12, r12
	xor r13, r13
	xor r14, r14
	xor r15, r15

normal_start:
	xor eax, eax	; Clear registers used earlier
	xor ebx, ebx
	xor ecx, ecx
	jmp 0x0000000000100000


%include "init/acpi.asm"
%include "init/cpu.asm"
%include "init/pic.asm"
;;%include "init/smp.asm"
%include "syscalls.asm"
%include "interrupt.asm"
%include "sysvar.asm"



;	. = _start + CODE_SECTORS * 512
times CODE_SECTORS * 512 -($-_start) db 0

