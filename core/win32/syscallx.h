/* **********************************************************
 * Copyright (c) 2011-2020 Google, Inc.  All rights reserved.
 * Copyright (c) 2007-2010 VMware, Inc.  All rights reserved.
 * **********************************************************/

/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of VMware, Inc. nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL VMWARE, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

/* Copyright (c) 2007 Determina Corp. */

/*
 * syscallx.h
 *
 * System call support due to the varying system call numbers across platforms.
 * To have one binary work on multiple Windows we can't have one set of constants.
 *
 * XXX i#2713: With the frequent major win10 updates, adding new data here is getting
 * painful: we should consider moving this to a separate data source.
 *
 * Usage ("w11" means "win10-1511", "w12" means "win10-1607", "w13" means "win10-1703",
 *        "w14" means "win10-1709", "w15" means "win10-1803"):
 * #define SYSCALL(name, act, nargs, arg32, ntsp0, ntsp3, ntsp4, w2k, xp, wow64, xp64,
 *                 w2k3, vista0, vista0_x64, vista1, vista1_x64, w7x86, w7x64,
 *                 w8x86, w8w64, w8x64, w81x86, w81w64, w81x64, w10x86, w10w64, w10x64,
 *                 w11x86, w11w64, w11x64, w12x86, w12w64, w12x64, w13x86, w13w64, w13x64,
 *                 w14x86, w14w64, w14x64, w15x86, w15w64, w15x64)
 * #include "syscallx.h"
 * #undef SYSCALL
 *
 */

/* NOTE - Vista Beta 2 has different syscall numbers than Vista final,
 * the #s here are for for Vista Final (see cvs code attic for Beta
 * 2).
 */

/* We expect x64 2003 and x64 XP to have the same system call numbers but
 * that has not been verified
 */

/* for NT SP4, SP5, SP6, and SP6a
 * Metasploit's table claims SP4 has additional syscalls, though our
 * investigation disagrees (see case 5616) -- even if so they are appended
 * and so don't affect the numbering of any of these.
 */

/* Column descriptions:
 * action?    == does DR need to take action when the app issues this system call?
 * nargs      == number of arguments on x64; should we assume this is always arg32/4?
 * arg32      == argument size in bytes on x86
 * wow64      == index into argument conversion routines (see case 3922)
 *               for xp through win7 (win8 uses top bits of sysnum)
 * all others == system call number for that windows version
 *
 * Argument stack size vs number of args discussion:
 * + We get the arg stack size from the ret immed in the wrapper and compute
 *   nargs from it, so it's what we know for sure;
 * + in callback.c it's the stack size that we need to know, not the #args, b/c we
 *   create our own ret immed;
 * + for x86 conceivably there could be a double-sized arg, so stack size doesn't
 *   have to equal nargs*4.
 * So we'll continue having both columns here.
 */

/* XXX: MS sometimes changes argsz between OS versions (see vista case 6853 for
 * some examples) instead of adding an Ex version: if that happens to any of the
 * syscalls we care about we'll have to augment this table.
 */

/* shorter name for the table */
#define NONE SYSCALL_NOT_PRESENT
/* to make some system calls actionable only in DEBUG (because all we do is log) */
#define ACTION_LOG IF_DEBUG_ELSE(true, false)
#define ACTION_CLIENT IF_CLIENT_INTERFACE_ELSE(true, false)
/* clang-format off */
/*                                                                                                                 vista         vista
 *                                                                                     xp-w7                vista   x64   vista   x64                                      w8.1    w8.1  w8.1    w10     w10   w10    w11     w11  w11     w12     w12  w12     w13     w13  w13     w14     w14  w14   w15     w15  w15
 *      Name                       action?  nargs arg32 ntsp0 ntsp3 ntsp4  2000     xp wow64   xp64   2003   sp0    sp0    sp1    sp1   w7x86 w7x64  w8x86 w8wow64 w8x64    x86   wow64   x64    x86   wow64   x64    x86   wow64  x64     x86   wow64  x64     x86   wow64  x64     x86   wow64  x64   x86   wow64  x64 */
SYSCALL(Continue,                     true,     2, 0x08, 0x13, 0x13, 0x13, 0x1c, 0x020,    0, 0x040, 0x022, 0x037, 0x040, 0x037, 0x040, 0x03c,0x040, 0x16a,0x00041,0x041, 0x16f,0x00042,0x042, 0x175,0x00043,0x043, 0x178,0x00043,0x043, 0x17a,0x00043,0x043, 0x17e,0x00043,0x043, 0x181,0x00043,0x043, 0x181,0x00043,0x043)
SYSCALL(CallbackReturn,               true,     3, 0x0c, 0x0b, 0x0b, 0x0b, 0x13, 0x014,    0, 0x002, 0x016, 0x02b, 0x002, 0x02b, 0x002, 0x02c,0x002, 0x17b,0x00003,0x003, 0x180,0x00004,0x004, 0x187,0x00005,0x005, 0x18a,0x00005,0x005, 0x18c,0x00005,0x005, 0x191,0x00005,0x005, 0x195,0x00005,0x005, 0x195,0x00005,0x005)
SYSCALL(SetContextThread,             true,     2, 0x08, 0x98, 0x99, 0x99, 0xba, 0x0d5,    0, 0x0f6, 0x0dd, 0x125, 0x14f, 0x121, 0x149, 0x13c,0x150, 0x05b,0x00165,0x165, 0x05e,0x00168,0x168, 0x05f,0x0016f,0x16f, 0x05f,0x00172,0x172, 0x05f,0x00178,0x178, 0x05f,0x0017e,0x17e, 0x060,0x00181,0x181, 0x060,0x00183,0x183)
SYSCALL(GetContextThread,             true,     2, 0x08, 0x3c, 0x3c, 0x3c, 0x49, 0x055,    0, 0x09d, 0x059, 0x097, 0x0c9, 0x097, 0x0c7, 0x087,0x0ca, 0x113,0x000dd,0x0dd, 0x116,0x000e0,0x0e0, 0x11b,0x000e3,0x0e3, 0x11d,0x000e4,0x0e4, 0x11f,0x000e6,0x0e6, 0x122,0x000e9,0x0e9, 0x125,0x000ea,0x0ea, 0x125,0x000eb,0x0eb)
SYSCALL(CreateProcess,                true,     8, 0x20, 0x1f, 0x1f, 0x1f, 0x29, 0x02f,    0, 0x082, 0x031, 0x048, 0x0a2, 0x048, 0x0a0, 0x04f,0x09f, 0x155,0x000a9,0x0a9, 0x159,0x000aa,0x0aa, 0x15f,0x000ad,0x0ad, 0x161,0x000ae,0x0ae, 0x163,0x000af,0x0af, 0x166,0x000b2,0x0b2, 0x169,0x000b3,0x0b3, 0x169,0x000b4,0x0b4)
SYSCALL(CreateProcessEx,              true,     9, 0x24, NONE, NONE, NONE, NONE, 0x030,    0, 0x04a, 0x032, 0x049, 0x04a, 0x049, 0x04a, 0x050,0x04a, 0x154,0x0004b,0x04b, 0x158,0x0004c,0x04c, 0x15e,0x0004d,0x04d, 0x160,0x0004d,0x04d, 0x162,0x0004d,0x04d, 0x165,0x0004d,0x04d, 0x168,0x0004d,0x04d, 0x168,0x0004d,0x04d)
SYSCALL(CreateUserProcess,            true,    11, 0x2c, NONE, NONE, NONE, NONE,  NONE,    0,  NONE,  NONE, 0x185, 0x0ac, 0x17f, 0x0aa, 0x05d,0x0aa, 0x145,0x000b5,0x0b5, 0x149,0x000b7,0x0b7, 0x14f,0x000ba,0x0ba, 0x151,0x000bb,0x0bb, 0x153,0x000bd,0x0bd, 0x156,0x000c0,0x0c0, 0x159,0x000c1,0x0c1, 0x159,0x000c2,0x0c2)
SYSCALL(TerminateProcess,             true,     2, 0x08, 0xba, 0xbb, 0xbb, 0xe0, 0x101,    0, 0x029, 0x10a, 0x152, 0x029, 0x14e, 0x029, 0x172,0x029, 0x023,0x0002a,0x02a, 0x023,0x7002b,0x02b, 0x024,0x7002c,0x02c, 0x024,0x7002c,0x02c, 0x024,0x7002c,0x02c, 0x024,0x7002c,0x02c, 0x024,0x7002c,0x02c, 0x024,0x7002c,0x02c)
SYSCALL(CreateThread,                 true,     8, 0x20, 0x24, 0x24, 0x24, 0x2e, 0x035,    0, 0x04b, 0x037, 0x04e, 0x04b, 0x04e, 0x04b, 0x057,0x04b, 0x14d,0x0004c,0x04c, 0x151,0x0004d,0x04d, 0x157,0x0004e,0x04e, 0x159,0x0004e,0x04e, 0x15b,0x0004e,0x04e, 0x15e,0x0004e,0x04e, 0x161,0x0004e,0x04e, 0x161,0x0004e,0x04e)
SYSCALL(CreateThreadEx,               true,    11, 0x2c, NONE, NONE, NONE, NONE,  NONE,    0,  NONE,  NONE, 0x184, 0x0a7, 0x17e, 0x0a5, 0x058,0x0a5, 0x14c,0x000af,0x0af, 0x150,0x000b0,0x0b0, 0x156,0x000b3,0x0b3, 0x158,0x000b4,0x0b4, 0x15a,0x000b6,0x0b6, 0x15d,0x000b9,0x0b9, 0x160,0x000ba,0x0ba, 0x160,0x000bb,0x0bb)
SYSCALL(CreateWorkerFactory,          true,    10, 0x28, NONE, NONE, NONE, NONE,  NONE,    0,  NONE,  NONE, 0x17d, 0x0ae, 0x177, 0x0ac, 0x05f,0x0ac, 0x141,0x000b9,0x0b9, 0x145,0x000bb,0x0bb, 0x14b,0x000be,0x0be, 0x14d,0x000bf,0x0bf, 0x14f,0x000c1,0x0c1, 0x152,0x000c4,0x0c4, 0x155,0x000c5,0x0c5, 0x155,0x000c6,0x0c6)
SYSCALL(TerminateThread,              true,     2, 0x08, 0xbb, 0xbc, 0xbc, 0xe1, 0x102,    0, 0x050, 0x10b, 0x153, 0x050, 0x14f, 0x050, 0x173,0x050, 0x022,0x00051,0x051, 0x022,0x70052,0x052, 0x023,0x70053,0x053, 0x023,0x70053,0x053, 0x023,0x70053,0x053, 0x023,0x70053,0x053, 0x023,0x70053,0x053, 0x023,0x70053,0x053)
SYSCALL(SuspendThread,                true,     2, 0x08, 0xb8, 0xb9, 0xb9, 0xdd, 0x0fe, 0x07, 0x118, 0x107, 0x14f, 0x179, 0x14b, 0x172, 0x16f,0x17b, 0x026,0x70193,0x193, 0x026,0x70198,0x198, 0x027,0x701a0,0x1a0, 0x027,0x701a3,0x1a3, 0x027,0x701a9,0x1a9, 0x027,0x701af,0x1af, 0x028,0x701b2,0x1b2, 0x028,0x701b4,0x1b4)
SYSCALL(ResumeThread,                 true,     2, 0x08, 0x95, 0x96, 0x96, 0xb5, 0x0ce, 0x07, 0x04f, 0x0d6, 0x119, 0x04f, 0x11a, 0x04f, 0x130,0x04f, 0x068,0x70050,0x050, 0x06b,0x70051,0x051, 0x06d,0x70052,0x052, 0x06d,0x70052,0x052, 0x06e,0x70052,0x052, 0x06e,0x70052,0x052, 0x06f,0x70052,0x052, 0x06f,0x70052,0x052)
SYSCALL(QueryInformationThread,  ACTION_CLIENT, 5, 0x14, 0x6e, 0x6e, 0x6e, 0x87, 0x09b,    0, 0x022, 0x0a2, 0x0e5, 0x022, 0x0e5, 0x022, 0x0ec,0x022, 0x0ae,0x00023,0x023, 0x0b1,0x00024,0x024, 0x0b3,0x00025,0x025, 0x0b3,0x00025,0x025, 0x0b5,0x00025,0x025, 0x0b6,0x00025,0x025, 0x0b7,0x00025,0x025, 0x0b7,0x00025,0x025)
SYSCALL(AllocateVirtualMemory,        true,     6, 0x18, 0x0a, 0x0a, 0x0a, 0x10, 0x011,    0, 0x015, 0x012, 0x012, 0x015, 0x012, 0x015, 0x013,0x015, 0x196,0x00016,0x016, 0x19b,0x00017,0x017, 0x1a3,0x00018,0x018, 0x1a6,0x00018,0x018, 0x1a8,0x00018,0x018, 0x1ad,0x00018,0x018, 0x1b1,0x00018,0x018, 0x1b1,0x00018,0x018)
SYSCALL(AllocateVirtualMemoryEx,      true,     7, 0x1c, NONE, NONE, NONE, NONE,  NONE,    0,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE, 0x1cc,0x00074,0x074)
SYSCALL(FreeVirtualMemory,            true,     4, 0x10, 0x3a, 0x3a, 0x3a, 0x47, 0x053,    0, 0x01b, 0x057, 0x093, 0x01b, 0x093, 0x01b, 0x083,0x01b, 0x118,0x0001c,0x01c, 0x11c,0x0001d,0x01d, 0x121,0x0001e,0x01e, 0x123,0x0001e,0x01e, 0x125,0x0001e,0x01e, 0x128,0x0001e,0x01e, 0x12b,0x0001e,0x01e, 0x12b,0x0001e,0x01e)
SYSCALL(ProtectVirtualMemory,         true,     5, 0x14, 0x60, 0x60, 0x60, 0x77, 0x089,    0, 0x04d, 0x08f, 0x0d2, 0x04d, 0x0d2, 0x04d, 0x0d7,0x04d, 0x0c3,0x0004e,0x04e, 0x0c6,0x0004f,0x04f, 0x0c8,0x00050,0x050, 0x0c8,0x00050,0x050, 0x0ca,0x00050,0x050, 0x0cc,0x00050,0x050, 0x0ce,0x00050,0x050, 0x0ce,0x00050,0x050)
SYSCALL(QueryVirtualMemory,           true,     6, 0x18, 0x81, 0x81, 0x81, 0x9c, 0x0b2,    0, 0x020, 0x0ba, 0x0fd, 0x020, 0x0fd, 0x020, 0x10b,0x020, 0x08f,0x00021,0x021, 0x092,0x00022,0x022, 0x094,0x00023,0x023, 0x094,0x00023,0x023, 0x095,0x00023,0x023, 0x096,0x00023,0x023, 0x097,0x00023,0x023, 0x097,0x00023,0x023)
SYSCALL(WriteVirtualMemory,           true,     5, 0x14, 0xc9, 0xcb, 0xcb, 0xf0, 0x115,    0, 0x037, 0x11f, 0x16a, 0x037, 0x166, 0x037, 0x18f,0x037, 0x002,0x00038,0x038, 0x003,0x00039,0x039, 0x004,0x0003a,0x03a, 0x004,0x0003a,0x03a, 0x004,0x0003a,0x03a, 0x004,0x0003a,0x03a, 0x004,0x0003a,0x03a, 0x004,0x0003a,0x03a)
SYSCALL(MapViewOfSection,             true,    10, 0x28, 0x49, 0x49, 0x49, 0x5d, 0x06c,    0, 0x025, 0x071, 0x0b1, 0x025, 0x0b1, 0x025, 0x0a8,0x025, 0x0f3,0x00026,0x026, 0x0f6,0x00027,0x027, 0x0fa,0x00028,0x028, 0x0fa,0x00028,0x028, 0x0fc,0x00028,0x028, 0x0fe,0x00028,0x028, 0x101,0x00028,0x028, 0x101,0x00028,0x028)
SYSCALL(MapViewOfSectionEx,           true,     9, 0x24, NONE, NONE, NONE, NONE,  NONE,    0,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE,  NONE,   NONE, NONE, 0x1cd,0x0010d,0x10d)
SYSCALL(UnmapViewOfSection,           true,     2, 0x08, 0xc1, 0xc2, 0xc2, 0xe7, 0x10b,    0, 0x027, 0x115, 0x160, 0x027, 0x15c, 0x027, 0x181,0x027, 0x013,0x00028,0x028, 0x013,0x00029,0x029, 0x014,0x0002a,0x02a, 0x014,0x0002a,0x02a, 0x014,0x0002a,0x02a, 0x014,0x0002a,0x02a, 0x014,0x0002a,0x02a, 0x014,0x0002a,0x02a)
SYSCALL(UnmapViewOfSectionEx,         true,     3, 0x0c, NONE, NONE, NONE, NONE,  NONE,    0,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE, NONE, 0x014,0x001a2,0x1a2, 0x014,0x001a7,0x1a7, 0x015,0x001af,0x1af, 0x015,0x001b2,0x1b2, 0x015,0x001b8,0x1b8, 0x015,0x001be,0x1be, 0x015,0x001c2,0x1c2, 0x015,0x001c4,0x1c4)
SYSCALL(FlushInstructionCache,        true,     3, 0x0c, 0x36, 0x36, 0x36, 0x42, 0x04e, 0x0c, 0x098, 0x052, 0x08d, 0x0c1, 0x08d, 0x0bf, 0x07d,0x0c2, 0x11e,0xc00d4,0x0d4, 0x122,0xc00d6,0x0d6, 0x127,0xc00d9,0x0d9, 0x129,0xc00da,0x0da, 0x12b,0xc00dc,0x0dc, 0x12e,0xc00df,0x0df, 0x131,0xc00e0,0x0e0, 0x131,0xc00e1,0x0e1)
SYSCALL(FreeUserPhysicalPages,        true,     3, 0x0c, NONE, NONE, NONE, 0x46, 0x052,    0, 0x09c, 0x056, 0x092, 0x0c6, 0x092, 0x0c4, 0x082,0x0c7, 0x119,0x000d9,0x0d9, 0x11d,0x000db,0x0db, 0x122,0x000de,0x0de, 0x124,0x000df,0x0df, 0x126,0x000e1,0x0e1, 0x129,0x000e4,0x0e4, 0x12c,0x000e5,0x0e5, 0x12c,0x000e6,0x0e6)
SYSCALL(MapUserPhysicalPages,         true,     3, 0x0c, NONE, NONE, NONE, 0x5b, 0x06a, 0x0a, 0x0b2, 0x06f, 0x0af, 0x0e7, 0x0af, 0x0e4, 0x0a6,0x0e7, 0x0f5,0xa00f9,0x0f9, 0x0f8,0xa00fc,0x0fc, 0x0fc,0xa0101,0x101, 0x0fc,0xa0104,0x104, 0x0fe,0xa0106,0x106, 0x100,0xa010a,0x10a, 0x103,0xa010b,0x10b, 0x103,0xa010c,0x10c)
SYSCALL(SetInformationVirtualMemory,  true,     6, 0x18, NONE, NONE, NONE, NONE,  NONE,    0,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE, NONE, 0x0c9,0x00177,0x177, 0x0cc,0x0017a,0x17a, 0x0ce,0x00182,0x182, 0x0ce,0x00185,0x185, 0x0d0,0x0018b,0x18b, 0x0d2,0x00191,0x191, 0x0d4,0x00194,0x194, 0x0d4,0x00196,0x196)
SYSCALL(Wow64AllocateVirtualMemory64, true,     7, 0x1c, NONE, NONE, NONE, NONE,  NONE,    0,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE, NONE,  NONE,0x001bb, NONE,  NONE,0x001bf, NONE,  NONE,0x001c7, NONE,  NONE,0x001ca, NONE,  NONE,0x001d0, NONE,  NONE,0x001d6, NONE,  NONE,0x001da, NONE,  NONE,0x001dc, NONE)
SYSCALL(OpenSection,                  true,     3, 0x0c, 0x56, 0x56, 0x56, 0x6c, 0x07d,    0, 0x034, 0x083, 0x0c5, 0x034, 0x0c5, 0x034, 0x0c2,0x034, 0x0d9,0x00035,0x035, 0x0dc,0x00036,0x036, 0x0de,0x00037,0x037, 0x0de,0x00037,0x037, 0x0e0,0x00037,0x037, 0x0e2,0x00037,0x037, 0x0e4,0x00037,0x037, 0x0e4,0x00037,0x037)
SYSCALL(CreateSection,                true,     7, 0x1c, 0x21, 0x21, 0x21, 0x2b, 0x032,    0, 0x047, 0x034, 0x04b, 0x047, 0x04b, 0x047, 0x054,0x047, 0x150,0x00048,0x048, 0x154,0x00049,0x049, 0x15a,0x0004a,0x04a, 0x15c,0x0004a,0x04a, 0x15e,0x0004a,0x04a, 0x161,0x0004a,0x04a, 0x164,0x0004a,0x04a, 0x164,0x0004a,0x04a)
SYSCALL(Close,                        true,     1, 0x04, 0x0f, 0x0f, 0x0f, 0x18, 0x019,    0, 0x00c, 0x01b, 0x02f, 0x00c, 0x030, 0x00c, 0x032,0x00c, 0x174,0x0000d,0x00d, 0x179,0x3000e,0x00e, 0x180,0x3000f,0x00f, 0x183,0x3000f,0x00f, 0x185,0x3000f,0x00f, 0x18a,0x3000f,0x00f, 0x18d,0x3000f,0x00f, 0x18d,0x3000f,0x00f)
SYSCALL(DuplicateObject,              true,     7, 0x1c, 0x2f, 0x2f, 0x2f, 0x3a, 0x044,    0, 0x039, 0x047, 0x081, 0x039, 0x081, 0x039, 0x06f,0x039, 0x12f,0x0003a,0x03a, 0x133,0x0003b,0x03b, 0x138,0x0003c,0x03c, 0x13a,0x0003c,0x03c, 0x13c,0x0003c,0x03c, 0x13f,0x0003c,0x03c, 0x142,0x0003c,0x03c, 0x142,0x0003c,0x03c)
SYSCALL(AlertResumeThread,       ACTION_LOG,    2, 0x08, 0x06, 0x06, 0x06, 0x0b, 0x00c, 0x07, 0x069, 0x00d, 0x00d, 0x06a, 0x00d, 0x06a, 0x00d,0x069, 0x19d,0x7006c,0x06c, 0x1a2,0x7006c,0x06c, 0x1aa,0x7006c,0x06c, 0x1ad,0x7006c,0x06c, 0x1af,0x7006c,0x06c, 0x1b4,0x7006d,0x06d, 0x1b8,0x7006d,0x06d, 0x1b8,0x7006d,0x06d)
SYSCALL(OpenFile,                ACTION_LOG,    6, 0x18, 0x4f, 0x4f, 0x4f, 0x64, 0x074,    0, 0x030, 0x07a, 0x0ba, 0x030, 0x0ba, 0x030, 0x0b3,0x030, 0x0e8,0x00031,0x031, 0x0eb,0x00032,0x032, 0x0ee,0x00033,0x033, 0x0ee,0x00033,0x033, 0x0f0,0x00033,0x033, 0x0f2,0x00033,0x033, 0x0f4,0x00033,0x033, 0x0f4,0x00033,0x033)
SYSCALL(TestAlert,                    false,    0,    0, 0xbc, 0xbd, 0xbd, 0xe2, 0x103, 0x02, 0x11b, 0x10c, 0x154, 0x17c, 0x150, 0x175, 0x174,0x17e, 0x021,0x20196,0x196, 0x021,0x2019b,0x19b, 0x022,0x201a3,0x1a3, 0x022,0x201a6,0x1a6, 0x022,0x201ac,0x1ac, 0x022,0x201b2,0x1b2, 0x022,0x201b6,0x1b6, 0x022,0x201b8,0x1b8)
SYSCALL(RaiseException,                true,    3, 0x0c, 0x84, 0x84, 0x84, 0x9f, 0x0b5,    0, 0x0e1, 0x0bd, 0x100, 0x12b, 0x100, 0x126, 0x10f,0x12f, 0x089,0x00143,0x143, 0x08c,0x00146,0x146, 0x08e,0x0014c,0x14c, 0x08e,0x0014f,0x14f, 0x08f,0x00153,0x153, 0x08f,0x00159,0x159, 0x090,0x0015c,0x15c, 0x090,0x0015e,0x15e)
SYSCALL(CreateFile,                   false,   11, 0x2c, 0x17, 0x17, 0x17, 0x20, 0x025,    0, 0x052, 0x027, 0x03c, 0x052, 0x03c, 0x052, 0x042,0x052, 0x163,0x00053,0x053, 0x168,0x00054,0x054, 0x16e,0x00055,0x055, 0x170,0x00055,0x055, 0x172,0x00055,0x055, 0x175,0x00055,0x055, 0x178,0x00055,0x055, 0x178,0x00055,0x055)
SYSCALL(CreateKey,                    false,    7, 0x1c, 0x19, 0x19, 0x19, 0x23, 0x029,    0, 0x01a, 0x02b, 0x040, 0x01a, 0x040, 0x01a, 0x046,0x01a, 0x15e,0x0001b,0x01b, 0x162,0x0001c,0x01c, 0x168,0x0001d,0x01d, 0x16a,0x0001d,0x01d, 0x16c,0x0001d,0x01d, 0x16f,0x0001d,0x01d, 0x172,0x0001d,0x01d, 0x172,0x0001d,0x01d)
SYSCALL(OpenKey,                      false,    3, 0x0c, 0x51, 0x51, 0x51, 0x67, 0x077,    0, 0x00f, 0x07d, 0x0bd, 0x00f, 0x0bd, 0x00f, 0x0b6,0x00f, 0x0e5,0x00010,0x010, 0x0e8,0x00011,0x011, 0x0eb,0x00012,0x012, 0x0eb,0x00012,0x012, 0x0ed,0x00012,0x012, 0x0ef,0x00012,0x012, 0x0f1,0x00012,0x012, 0x0f1,0x00012,0x012)
SYSCALL(OpenKeyEx,                    false,    4, 0x10, NONE, NONE, NONE, NONE,  NONE,    0,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE, 0x0b7,0x0f2, 0x0e3,0x00104,0x104, 0x0e6,0x00107,0x107, 0x0e9,0x0010c,0x10c, 0x0e9,0x0010f,0x10f, 0x0eb,0x00111,0x111, 0x0ed,0x00115,0x115, 0x0ef,0x00117,0x117, 0x0ef,0x00119,0x119)
SYSCALL(OpenProcess,                  false,    4, 0x10, 0x54, 0x54, 0x54, 0x6a, 0x07a,    0, 0x023, 0x080, 0x0c2, 0x023, 0x0c2, 0x023, 0x0be,0x023, 0x0dd,0x00024,0x024, 0x0e0,0x00025,0x025, 0x0e3,0x00026,0x026, 0x0e3,0x00026,0x026, 0x0e5,0x00026,0x026, 0x0e7,0x00026,0x026, 0x0e9,0x00026,0x026, 0x0e9,0x00026,0x026)
SYSCALL(OpenProcessToken,             false,    3, 0x0c, 0x55, 0x55, 0x55, 0x6b, 0x07b,    0, 0x0be, 0x081, 0x0c3, 0x0f7, 0x0c3, 0x0f3, 0x0bf,0x0f9, 0x0dc,0x0010b,0x10b, 0x0df,0x0010e,0x10e, 0x0e2,0x00114,0x114, 0x0e2,0x00117,0x117, 0x0e4,0x00119,0x119, 0x0e6,0x0011d,0x11d, 0x0e8,0x0011f,0x11f, 0x0e8,0x00121,0x121)
SYSCALL(OpenProcessTokenEx,           false,    4, 0x10, NONE, NONE, NONE, NONE, 0x07c,    0, 0x02d, 0x082, 0x0c4, 0x02d, 0x0c4, 0x02d, 0x0c0,0x02d, 0x0db,0x0002e,0x02e, 0x0de,0x0002f,0x02f, 0x0e1,0x00030,0x030, 0x0e1,0x00030,0x030, 0x0e3,0x00030,0x030, 0x0e5,0x00030,0x030, 0x0e7,0x00030,0x030, 0x0e7,0x00030,0x030)
SYSCALL(OpenThread,                    true,    4, 0x10, 0x59, 0x59, 0x59, 0x6f, 0x080,    0, 0x0c1, 0x086, 0x0c9, 0x0fc, 0x0c9, 0x0f8, 0x0c6,0x0fe, 0x0d5,0x00110,0x110, 0x0d8,0x00113,0x113, 0x0da,0x00119,0x119, 0x0da,0x0011c,0x11c, 0x0dc,0x0011f,0x11f, 0x0de,0x00123,0x123, 0x0e0,0x00125,0x125, 0x0e0,0x00127,0x127)
SYSCALL(OpenThreadToken,              false,    4, 0x10, 0x5a, 0x5a, 0x5a, 0x70, 0x081,    0, 0x021, 0x087, 0x0ca, 0x021, 0x0ca, 0x021, 0x0c7,0x021, 0x0d4,0x00022,0x022, 0x0d7,0x00023,0x023, 0x0d9,0x00024,0x024, 0x0d9,0x00024,0x024, 0x0db,0x00024,0x024, 0x0dd,0x00024,0x024, 0x0df,0x00024,0x024, 0x0df,0x00024,0x024)
SYSCALL(OpenThreadTokenEx,            false,    5, 0x14, NONE, NONE, NONE, NONE, 0x082,    0, 0x02c, 0x088, 0x0cb, 0x02c, 0x0cb, 0x02c, 0x0c8,0x02c, 0x0d3,0x0002d,0x02d, 0x0d6,0x0002e,0x02e, 0x0d8,0x0002f,0x02f, 0x0d8,0x0002f,0x02f, 0x0da,0x0002f,0x02f, 0x0dc,0x0002f,0x02f, 0x0de,0x0002f,0x02f, 0x0de,0x0002f,0x02f)
SYSCALL(QueryAttributesFile,          false,    2, 0x08, 0x63, 0x63, 0x63, 0x7a, 0x08b,    0, 0x03a, 0x091, 0x0d4, 0x03a, 0x0d4, 0x03a, 0x0d9,0x03a, 0x0c1,0x0003b,0x03b, 0x0c4,0x0003c,0x03c, 0x0c6,0x0003d,0x03d, 0x0c6,0x0003d,0x03d, 0x0c8,0x0003d,0x03d, 0x0ca,0x0003d,0x03d, 0x0cc,0x0003d,0x03d, 0x0cc,0x0003d,0x03d)
SYSCALL(QueryFullAttributesFile,      false,    2, 0x08, 0x69, 0x69, 0x69, 0x81, 0x095,    0, 0x0ce, 0x09c, 0x0df, 0x112, 0x0df, 0x10d, 0x0e4,0x113, 0x0b6,0x00125,0x125, 0x0b9,0x00128,0x128, 0x0bb,0x0012e,0x12e, 0x0bb,0x00131,0x131, 0x0bd,0x00134,0x134, 0x0bf,0x00139,0x139, 0x0c0,0x0013c,0x13c, 0x0c0,0x0013e,0x13e)
SYSCALL(SetInformationFile,           false,    5, 0x14, 0xa0, 0xa1, 0xa1, 0xc2, 0x0e0,    0, 0x024, 0x0e9, 0x131, 0x024, 0x12d, 0x024, 0x149,0x024, 0x04e,0x00025,0x025, 0x051,0x00026,0x026, 0x052,0x00027,0x027, 0x052,0x00027,0x027, 0x052,0x00027,0x027, 0x052,0x00027,0x027, 0x053,0x00027,0x027, 0x053,0x00027,0x027)
SYSCALL(SetInformationThread,         false,    4, 0x10, 0xa4, 0xa5, 0xa5, 0xc7, 0x0e5,    0, 0x00a, 0x0ee, 0x136, 0x00a, 0x132, 0x00a, 0x14f,0x00a, 0x048,0x0000b,0x00b, 0x04b,0x0000c,0x00c, 0x04c,0x0000d,0x00d, 0x04c,0x0000d,0x00d, 0x04c,0x0000d,0x00d, 0x04c,0x0000d,0x00d, 0x04d,0x0000d,0x00d, 0x04d,0x0000d,0x00d)
SYSCALL(SetValueKey,                  false,    6, 0x18, 0xb2, 0xb3, 0xb3, 0xd7, 0x0f7,    0, 0x05d, 0x100, 0x148, 0x05d, 0x144, 0x05d, 0x166,0x05d, 0x030,0x0005e,0x05e, 0x031,0x0005f,0x05f, 0x032,0x00060,0x060, 0x032,0x00060,0x060, 0x032,0x00060,0x060, 0x032,0x00060,0x060, 0x033,0x00060,0x060, 0x033,0x00060,0x060)
SYSCALL(WaitForSingleObject,          false,    3, 0x0c, 0xc4, 0xc5, 0xc5, 0xea, 0x10f, 0x0d, 0x001, 0x119, 0x164, 0x001, 0x160, 0x001, 0x187,0x001, 0x00a,0xd0002,0x002, 0x00a,0xd0003,0x003, 0x00b,0xd0004,0x004, 0x00b,0xd0004,0x004, 0x00b,0xd0004,0x004, 0x00b,0xd0004,0x004, 0x00b,0xd0004,0x004, 0x00b,0xd0004,0x004)
/* clang-format on */

/* I moved comments down here to make it easier to paste new columns in: */
/* XXX: OpenSection processing is needed only for ASLR_SHARED, but can't be made dynamic
 */
/* XXX: move AlertResumeThread, which is debug-only, to an strace-like client (update: we
 * do have drstrace now), not needed for core DynamoRIO (at least not that we know of)
 */
/* TestAlert and below are for DR's own use */

#ifdef ISSUE_1229
/* XXX i#1229: to ensure we clean up we'd want to watch SYS_TerminateJobObject
 * but to be complete we'd also have to look at job handles and properties,
 * which gets rather complex, when it's going to be rare that a job is killed
 * from within.  Disabled for now.
 */
/* clang-format off */
SYSCALL(TerminateJobObject,           true,     2, 0x08, NONE, NONE, NONE, 0xdf, 0x100, 0x07, 0x11a, 0x109, 0x151, 0x17b, 0x14d, 0x174, 0x171,0x17d, 0x024,0x70195,0x195, 0x024,0x7019a,0x19a, 0x024,0x7019a,0x1a2, 0x025,0x701a5,0x1a5, 0x025,0x701ab,0x1ab, 0x025,0x701b1,0x1b1, 0x025,0x701b5,0x1b5, 0x025, 0x0701b7, 0x1b7)
/* clang-format on */
#endif

#undef NONE
#undef ACTION_LOG

/* Attic - there is little point in continuing to update the syscall numbers
 * below since they are only used for ignorable system calls which is
 * terminally broken anyways. */
#if 0
    /* we don't intercept these syscalls for correctness or security policies,
     * but we need to come back to a non-cache point for them since they
     * are alertable and callbacks can be delivered during the syscall.
     * this list came from this filter of the syscall names:
     *   grep 'Alert|Wait' | grep -v CreateWaitablePort
     *   (NtContinue is already up above)
     *   plus grep 'Alert|Wait' in ntdll.h (ZwDelayExecution)
     */
    /* FIXME - don't think all of these are in fact alertable, plus many file
     * io syscalls are alertable depending on the options passed when the file
     * handle was created.  There also may other alertable system calls we
     * don't know about and since we don't even use ignore syscalls (with no
     * plan to bring it back) having them in our arrays doesn't serve much
     * purpose. */
    /* FIXME: NT4.0 (only) has two more exported *Wait* routines,
     * NtSetHighWaitLowThread and NtSetLowWaitHighThread, that each
     * have a gap in the system call numbering, but in the ntdll dump
     * they look like int 2b and 2c, resp.  Now, Inside Win2K lists int 2c as
     * "KiSetLowWaitHighThread" (though 2b is what we expect, even on NT,
     * KiCallbackReturn).  Nebbett has more pieces of the story:
     * "three of the four entry points purporting to refer to this system
     * service actually invoke a different routine", int 2c/2b, which does not
     * do what this call is supposed to do -- only NTOSKRNL!NtSet* does the
     * right thing.  Thus, we don't bother to intercept, as we will never
     * see those system calls, right?
     */
/* clang-format off */
/*                                                                                                          vista  vista
 *      Name                       action?  nargs arg32 ntsp0 ntsp3 ntsp4  2000     xp wow64   xp64   2003   sp0    sp1     w7   w7x64 */
SYSCALL(AlertThread,                  false,    1, 0x04, 0x07, 0x07, 0x07, 0x0c, 0x00d, 0x03, 0x06a, 0x00e, 0x00e, 0x00e, 0x00e, 0x06a)
SYSCALL(DelayExecution,               false,    2, 0x08, 0x27, 0x27, 0x27, 0x32, 0x03b, 0x06, 0x031, 0x03d, 0x076, 0x076, 0x062, 0x031)
SYSCALL(ReplyWaitReceivePort,         false,    4, 0x10, 0x8f, 0x90, 0x90, 0xab, 0x0c3,    0, 0x008, 0x0cb, 0x10e, 0x10f, 0x127, 0x008)
SYSCALL(ReplyWaitReceivePortEx,       false,    5, 0x14, NONE, NONE, NONE, 0xac, 0x0c4,    0, 0x028, 0x0cc, 0x10f, 0x110, 0x128, 0x028)
SYSCALL(ReplyWaitReplyPort,           false,    2, 0x08, 0x90, 0x91, 0x91, 0xad, 0x0c5,    0, 0x0e8, 0x0cd, 0x110, 0x111, 0x129, 0x13f)
SYSCALL(ReplyWaitSendChannel,         false,    3, 0x0c, 0xce, 0xd0, 0xcf, 0xf4,  NONE, NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE)
SYSCALL(RequestWaitReplyPort,         false,    3, 0x0c, 0x92, 0x93, 0x93, 0xb0, 0x0c8,    0, 0x01f, 0x0d0, 0x113, 0x114, 0x12b, 0x01f)
SYSCALL(SendWaitReplyChannel,         false,    4, 0x10, 0xcf, 0xd1, 0xd0, 0xf5,  NONE, NONE,  NONE,  NONE,  NONE,  NONE,  NONE,  NONE)
SYSCALL(SetHighWaitLowEventPair,      false,    1, 0x04, 0x9e, 0x9f, 0x9f, 0xc1, 0x0de, 0x03, 0x0fe, 0x0e7, 0x12f, 0x12b, 0x146, 0x158)
SYSCALL(SetLowWaitHighEventPair,      false,    1, 0x04, 0xa9, 0xaa, 0xaa, 0xcc, 0x0eb, 0x03, 0x107, 0x0f4, 0x13c, 0x138, 0x159, 0x167)
SYSCALL(SignalAndWaitForSingleObject, false,    4, 0x10, 0xb5, 0xb6, 0xb6, 0xda, 0x0fa, 0x13, 0x114, 0x103, 0x14b, 0x147, 0x16a, 0x176)
SYSCALL(WaitForDebugEvent,            false,    4, 0x10, NONE, NONE, NONE, NONE, 0x10d,    0, 0x124, 0x117, 0x162, 0x15e, 0x183, 0x18b)
SYSCALL(WaitForKeyedEvent,            false,    4, 0x10, NONE, NONE, NONE, NONE, 0x11a, 0x15, 0x125, 0x124, 0x16f, 0x16b, 0x184, 0x18c)
SYSCALL(WaitForMultipleObjects,       false,    5, 0x14, 0xc3, 0xc4, 0xc4, 0xe9, 0x10e, 0x1d, 0x058, 0x118, 0x163, 0x15f, 0x185, 0x058)
SYSCALL(WaitHighEventPair,            false,    1, 0x04, 0xc5, 0xc6, 0xc6, 0xeb, 0x110, 0x03, 0x126, 0x11a, 0x165, 0x161, 0x189, 0x18e)
SYSCALL(WaitLowEventPair,             false,    1, 0x04, 0xc6, 0xc7, 0xc7, 0xec, 0x111, 0x03, 0x127, 0x11b, 0x166, 0x162, 0x18a, 0x18f)
/* clang-format on */
#endif /* if 0 */
