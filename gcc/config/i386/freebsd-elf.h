/* Definitions for Intel 386 running FreeBSD with ELF format
   Copyright (C) 1996 Free Software Foundation, Inc.
   Contributed by Eric Youngdale.
   Modified for stabs-in-ELF by H.J. Lu.
   Adapted from GNU/Linux version by John Polstra.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#undef TARGET_VERSION
#define TARGET_VERSION fprintf (stderr, " (i386 FreeBSD/ELF)");

/* The svr4 ABI for the i386 says that records and unions are returned
   in memory.  */
/* On FreeBSD, we do not. */
#undef DEFAULT_PCC_STRUCT_RETURN
#define DEFAULT_PCC_STRUCT_RETURN 0

/* This gets defined in tm.h->linux.h->svr4.h, and keeps us from using
   libraries compiled with the native cc, so undef it. */
#undef NO_DOLLAR_IN_LABEL

/* This is how to output an element of a case-vector that is relative.
   This is only used for PIC code.  See comments by the `casesi' insn in
   i386.md for an explanation of the expression this outputs. */
#undef ASM_OUTPUT_ADDR_DIFF_ELT
#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL) \
  fprintf (FILE, "\t.long _GLOBAL_OFFSET_TABLE_+[.-%s%d]\n", LPREFIX, VALUE)

/* Indicate that jump tables go in the text section.  This is
   necessary when compiling PIC code.  */
#define JUMP_TABLES_IN_TEXT_SECTION (flag_pic)

/* Copy this from the svr4 specifications... */
/* Define the register numbers to be used in Dwarf debugging information.
   The SVR4 reference port C compiler uses the following register numbers
   in its Dwarf output code:
	0 for %eax (gnu regno = 0)
	1 for %ecx (gnu regno = 2)
	2 for %edx (gnu regno = 1)
	3 for %ebx (gnu regno = 3)
	4 for %esp (gnu regno = 7)
	5 for %ebp (gnu regno = 6)
	6 for %esi (gnu regno = 4)
	7 for %edi (gnu regno = 5)
   The following three DWARF register numbers are never generated by
   the SVR4 C compiler or by the GNU compilers, but SDB on x86/svr4
   believes these numbers have these meanings.
	8  for %eip    (no gnu equivalent)
	9  for %eflags (no gnu equivalent)
	10 for %trapno (no gnu equivalent)
   It is not at all clear how we should number the FP stack registers
   for the x86 architecture.  If the version of SDB on x86/svr4 were
   a bit less brain dead with respect to floating-point then we would
   have a precedent to follow with respect to DWARF register numbers
   for x86 FP registers, but the SDB on x86/svr4 is so completely
   broken with respect to FP registers that it is hardly worth thinking
   of it as something to strive for compatibility with.
   The version of x86/svr4 SDB I have at the moment does (partially)
   seem to believe that DWARF register number 11 is associated with
   the x86 register %st(0), but that's about all.  Higher DWARF
   register numbers don't seem to be associated with anything in
   particular, and even for DWARF regno 11, SDB only seems to under-
   stand that it should say that a variable lives in %st(0) (when
   asked via an `=' command) if we said it was in DWARF regno 11,
   but SDB still prints garbage when asked for the value of the
   variable in question (via a `/' command).
   (Also note that the labels SDB prints for various FP stack regs
   when doing an `x' command are all wrong.)
   Note that these problems generally don't affect the native SVR4
   C compiler because it doesn't allow the use of -O with -g and
   because when it is *not* optimizing, it allocates a memory
   location for each floating-point variable, and the memory
   location is what gets described in the DWARF AT_location
   attribute for the variable in question.
   Regardless of the severe mental illness of the x86/svr4 SDB, we
   do something sensible here and we use the following DWARF
   register numbers.  Note that these are all stack-top-relative
   numbers.
	11 for %st(0) (gnu regno = 8)
	12 for %st(1) (gnu regno = 9)
	13 for %st(2) (gnu regno = 10)
	14 for %st(3) (gnu regno = 11)
	15 for %st(4) (gnu regno = 12)
	16 for %st(5) (gnu regno = 13)
	17 for %st(6) (gnu regno = 14)
	18 for %st(7) (gnu regno = 15)
*/
#undef DBX_REGISTER_NUMBER
#define DBX_REGISTER_NUMBER(n) \
((n) == 0 ? 0 \
 : (n) == 1 ? 2 \
 : (n) == 2 ? 1 \
 : (n) == 3 ? 3 \
 : (n) == 4 ? 6 \
 : (n) == 5 ? 7 \
 : (n) == 6 ? 5 \
 : (n) == 7 ? 4 \
 : ((n) >= FIRST_STACK_REG && (n) <= LAST_STACK_REG) ? (n)+3 \
 : (-1))

/* Output assembler code to FILE to increment profiler label # LABELNO
   for profiling a function entry.  */

#undef FUNCTION_PROFILER
#define FUNCTION_PROFILER(FILE, LABELNO)  \
{									\
  if (flag_pic)								\
    {									\
      fprintf (FILE, "\tleal %sP%d@GOTOFF(%%ebx),%%edx\n",		\
	       LPREFIX, (LABELNO));					\
      fprintf (FILE, "\tcall *mcount@GOT(%%ebx)\n");			\
    }									\
  else									\
    {									\
      fprintf (FILE, "\tmovl $%sP%d,%%edx\n", LPREFIX, (LABELNO));	\
      fprintf (FILE, "\tcall mcount\n");				\
    }									\
}

#undef SIZE_TYPE
#define SIZE_TYPE "unsigned int"
 
#undef PTRDIFF_TYPE
#define PTRDIFF_TYPE "int"
  
#undef WCHAR_TYPE
#define WCHAR_TYPE "int"
   
#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE BITS_PER_WORD
    
#undef CPP_PREDEFINES
#define CPP_PREDEFINES "-Di386 -Dunix -D__ELF__ -D__FreeBSD__ -Asystem(unix) -Asystem(FreeBSD) -Acpu(i386) -Amachine(i386)"

#undef CPP_SPEC
#define CPP_SPEC "%(cpp_cpu) %{fPIC:-D__PIC__ -D__pic__} %{fpic:-D__PIC__ -D__pic__} %{posix:-D_POSIX_SOURCE}"

#undef	LIB_SPEC
#if 1
/* We no longer link with libc_p.a or libg.a by default. If you
 * want to profile or debug the C library, please add
 * -lc_p or -ggdb to LDFLAGS at the link time, respectively.
 */
#define LIB_SPEC \
  "%{!shared: %{mieee-fp:-lieee} %{p:-lgmon} %{pg:-lgmon} \
     %{!ggdb:-lc} %{ggdb:-lg}}"
#else
#define LIB_SPEC \
  "%{!shared: \
     %{mieee-fp:-lieee} %{p:-lgmon -lc_p} %{pg:-lgmon -lc_p} \
       %{!p:%{!pg:%{!g*:-lc} %{g*:-lg}}}}"
#endif

/* Provide a LINK_SPEC appropriate for FreeBSD.  Here we provide support
   for the special GCC options -static and -shared, which allow us to
   link things in one of these three modes by applying the appropriate
   combinations of options at link-time. We like to support here for
   as many of the other GNU linker options as possible. But I don't
   have the time to search for those flags. I am sure how to add
   support for -soname shared_object_name. H.J.

   I took out %{v:%{!V:-V}}. It is too much :-(. They can use
   -Wl,-V.

   When the -shared link option is used a final link is not being
   done.  */

#undef	LINK_SPEC
#define LINK_SPEC "-m elf_i386 %{shared:-shared} \
  %{!shared: \
    %{!ibcs: \
      %{!static: \
	%{rdynamic:-export-dynamic} \
	%{!dynamic-linker:-dynamic-linker /usr/libexec/ld-elf.so.1}} \
	%{static:-static}}}"

/* Get perform_* macros to build libgcc.a.  */

/* A C statement to output to the stdio stream FILE an assembler
   command to advance the location counter to a multiple of 1<<LOG
   bytes if it is within MAX_SKIP bytes.

   This is used to align code labels according to Intel recommendations.  */

#ifdef HAVE_GAS_MAX_SKIP_P2ALIGN
#define ASM_OUTPUT_MAX_SKIP_ALIGN(FILE,LOG,MAX_SKIP) \
  if ((LOG) != 0) {\
    if ((MAX_SKIP) == 0) fprintf ((FILE), "\t.p2align %d\n", (LOG)); \
    else fprintf ((FILE), "\t.p2align %d,,%d\n", (LOG), (MAX_SKIP)); \
  }
#endif
