/* ELF32/HPPA support

   This file contains ELF32/HPPA relocation support as specified
   in the Stratus FTX/Golf Object File Format (SED-1762) dated
   November 19, 1992.

   Copyright (C) 1990, 91, 92, 93, 94 Free Software Foundation, Inc.

   Written by:

   Center for Software Science
   Department of Computer Science
   University of Utah

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#ifndef _ELF32_HPPA_H
#define _ELF32_HPPA_H

#include "libelf.h"
#include "libhppa.h"
#include "elf/hppa.h"

/* ELF/HPPA relocation types */

typedef enum
  {
    /* Address relocation types
       These relocation types do simple base + offset relocations.  */

    R_PARISC_NONE = 0x00,
    R_PARISC_DIR32 = 0x01,
    R_PARISC_DIR21L = 0x02,
    R_PARISC_DIR17R = 0x03,
    R_PARISC_DIR17F = 0x04,
    R_PARISC_DIR14R = 0x06,

    /* PC-relative relocation types
       Typically used for calls.
       Note PCREL17C and PCREL17F differ only in overflow handling.
       PCREL17C never reports a relocation error.

       When supporting argument relocations, function calls must be
       accompanied by parameter relocation information.  This information is 
       carried in the ten high-order bits of the addend field.  The remaining
       22 bits of of the addend field are sign-extended to form the Addend.

       Note the code to build argument relocations depends on the 
       addend being zero.  A consequence of this limitation is GAS
       can not perform relocation reductions for function symbols.  */
    R_PARISC_PCREL21L = 0x0a,
    R_PARISC_PCREL17R = 0x0b,
    R_PARISC_PCREL17F = 0x0c,
    R_PARISC_PCREL17C = 0x0d,
    R_PARISC_PCREL14R = 0x0e,
    R_PARISC_PCREL14F = 0x0f,

    /* DP-relative relocation types.  */
    R_PARISC_DPREL21L = 0x12,
    R_PARISC_DPREL14R = 0x16,
    R_PARISC_DPREL14F = 0x17,

    /* Data linkage table (DLT) relocation types

       SOM DLT_REL fixup requests are used to for static data references
       from position-independent code within shared libraries.  They are
       similar to the GOT relocation types in some SVR4 implementations.  */

    R_PARISC_DLTREL21L = 0x1a,
    R_PARISC_DLTREL14R = 0x1e,
    R_PARISC_DLTREL14F = 0x1f,

    /* DLT indirect relocation types  */
    R_PARISC_DLTIND21L = 0x22,
    R_PARISC_DLTIND14R = 0x26,
    R_PARISC_DLTIND14F = 0x27,

    /* Base relative relocation types.  Ugh.  These imply lots of state */
    R_PARISC_SETBASE = 0x28,
    R_PARISC_BASEREL32 = 0x29,
    R_PARISC_BASEREL21L = 0x2a,
    R_PARISC_BASEREL17R = 0x2b,
    R_PARISC_BASEREL17F = 0x2c,
    R_PARISC_BASEREL14R = 0x2e,
    R_PARISC_BASEREL14F = 0x2f,

    /* Segment relative relocation types.  */
    R_PARISC_TEXTREL32 = 0x31,
    R_PARISC_DATAREL32 = 0x39,

    /* Plabel relocation types.  */
    R_PARISC_PLABEL32 = 0x41,
    R_PARISC_PLABEL21L = 0x42,
    R_PARISC_PLABEL14R = 0x46,

    /* PLT relocations.  */
    R_PARISC_PLTIND21L = 0x82,
    R_PARISC_PLTIND14R = 0x86,
    R_PARISC_PLTIND14F = 0x87,

    /* Misc relocation types.  */
    R_PARISC_COPY = 0x88,
    R_PARISC_GLOB_DAT = 0x89,
    R_PARISC_JMP_SLOT = 0x8a,
    R_PARISC_RELATIVE = 0x8b,
    R_PARISC_STUB_CALL_17 = 0x8c,
    R_PARISC_UNIMPLEMENTED,
  }
elf32_hppa_reloc_type;

#define ELF_HOWTO_TABLE_SIZE	R_PARISC_UNIMPLEMENTED + 1
#define N_PARISC_RELOCS		R_PARISC_UNIMPLEMENTED + 1

/* Define groups of basic relocations.  FIXME:  These should
   be the only basic relocations created by GAS.  The rest
   should be internal to the BFD backend.  

   The idea is both SOM and ELF define these basic relocation
   types so they map into a SOM or ELF specific relocation 
   as appropriate.  This allows GAS to share much more code
   between the two target object formats.  */

#define R_HPPA_NONE			R_PARISC_NONE
#define	R_HPPA				R_PARISC_DIR32
#define	R_HPPA_GOTOFF			R_PARISC_DPREL21L
#define	R_HPPA_PCREL_CALL		R_PARISC_PCREL21L

/* HPPA symbol table extension entry types */
enum elf32_hppa_symextn_types
{
  PARISC_SXT_NULL,
  PARISC_SXT_SYMNDX,
  PARISC_SXT_ARG_RELOC,
};

/* These macros compose and decompose the value of a symextn entry:
 
   entry_type = ELF32_PARISC_SX_TYPE(word);
   entry_value = ELF32_PARISC_SX_VAL(word);
   word = ELF32_PARISC_SX_WORD(type,val);  */

#define ELF32_PARISC_SX_TYPE(p)		((p) >> 24)
#define ELF32_PARISC_SX_VAL(p)		((p) & 0xFFFFFF)
#define ELF32_PARISC_SX_WORD(type,val)	(((type) << 24) + (val & 0xFFFFFF))

/* The following was added facilitate implementation of the .hppa_symextn
   section.  This section is built after the symbol table is built in the
   elf_write_object_contents routine (called from bfd_close).  It is built
   so late because it requires information that is not known until
   the symbol and string table sections have been allocated, and
   the symbol table has been built. */

/* Number of "hand-made" target specific sections.  */
#define ELF_TC_FAKE_SECTIONS 1
#define SYMEXTN_SECTION_NAME ".PARISC.symext"

/* FIXME.  Are these external?  (For example used by GAS?).  If so the
   names need to change to avoid namespace pollution, if not they should
   be moved into elf32-hppa.c.  */
typedef unsigned long symext_entryS;
struct symext_chain
  {
    symext_entryS entry;
    struct symext_chain *next;
  };

typedef struct symext_chain symext_chainS;

elf32_hppa_reloc_type **hppa_elf_gen_reloc_type
  PARAMS ((bfd *, elf32_hppa_reloc_type, int, int));

asymbol * hppa_look_for_stubs_in_section
  PARAMS ((bfd *, bfd *, bfd *, asection *,
	   int *, struct bfd_link_info *));

void elf_hppa_final_processing PARAMS ((void));
#endif /* _ELF32_HPPA_H */
