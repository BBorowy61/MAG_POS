/**********************************************************************
* File: f2812.cmd -- Linker command file for Boot to Flash bootmode.
* History: 09/08/03 - original (based on DSP28 header files v1.00, D. Alter)
*          02/14/05 - made consistent with DSP/BIOS linkage allocation (D. Alter)
**********************************************************************/

MEMORY
{
PAGE 0:    /* Program Memory */
   L0SARAM     : origin = 0x008000, length = 0x001000     /* 4Kw L0 SARAM */
/*   OTP         : origin = 0x3D7800, length = 0x000400     /* 1Kw on-chip OTP */
/*   FLASH_IJ    : origin = 0x3D8000, length = 0x004000     /* 16Kw on-chip FLASH */
   FLASH_I      : origin = 0x3DA000, length = 0x002000     /* 8Kw on-chip FLASH */
   FLASH_GH    : origin = 0x3DC000, length = 0x008000     /* 32Kw on-chip FLASH */
   FLASH_EF    : origin = 0x3E4000, length = 0x008000     /* 32Kw on-chip FLASH */
   FLASH_CD    : origin = 0x3EC000, length = 0x008000     /* 32Kw on-chip FLASH */
   FLASH_AB    : origin = 0x3F4000, length = 0x003F80     /* 16Kw on-chip FLASH */
   CSM_RSVD    : origin = 0x3F7F80, length = 0x000076     /* Part of FLASHA.  Reserved when CSM is in use. */
   BEGIN_FLASH : origin = 0x3F7FF6, length = 0x000002     /* Part of FLASHA.  Used for "boot to flash" bootloader mode. */
   PASSWORDS   : origin = 0x3F7FF8, length = 0x000008     /* Part of FLASHA.  CSM password locations. */
   H0SARAM     : origin = 0x3F8000, length = 0x002000     /* 8Kw H0 SARAM */
/*   ZONE7       : origin = 0x3FC000, length = 0x003FC0     /* XINTF zone 7 available if MP/MCn=1 */ 
   BOOTROM     : origin = 0x3FF000, length = 0x000FC0     /* boot ROM available if MP/MCn=0 */
   RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM (MP/MCn=0) or XINTF zone 7 (MP/MCn=1) */
   VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM (MP/MCn=0) or XINTF zone 7 (MP/MCn=1) */

 PAGE 1 :   /* Data Memory */
   M0SARAM     : origin = 0x000000, length = 0x000400     /* 1Kw M0 SARAM */
   M1SARAM     : origin = 0x000400, length = 0x000400     /* 1Kw M1 SARAM */
   L1SARAM     : origin = 0x009000, length = 0x001000     /* 4Kw L1 SARAM */
/*   ZONE0       : origin = 0x002000, length = 0x002000     /* not used XINTF zone 0 */
/*   ZONE1       : origin = 0x004000, length = 0x002000     /* ext memory mapped IO XINTF zone 1 */
   ZONE1       : origin = 0x004000, length = 0x00000E     /* ext memory mapped IO only 14 word */
   ZONE26      : origin = 0x080000, length = 0x100000     /* 1Mw XINTF zone 2 & 6 */
/*   ZONE2       : origin = 0x080000, length = 0x080000     /* 0.5Mw XINTF zone 2 */
/*   ZONE6       : origin = 0x100000, length = 0x080000     /* 0.5Mw XINTF zone 6 */
   FLASH_J      : origin = 0x3D8000, length = 0x002000     /* 8Kw on-chip FLASH */
   H0SARAM     : origin = 0x3F8000, length = 0x002000     /* 8Kw H0 SARAM */
}

 
SECTIONS
{
/*** Compiler Required Sections ***/

/* Program memory (PAGE 0) sections */
   .text             : > FLASH_CD,         PAGE = 0
   ramfuncs          : >FLASH_CD,
					   RUN = H0SARAM,
					   LOAD_START(_RamFuncs_loadstart),
                       LOAD_END(_RamFuncs_loadend),
                       RUN_START(_RamFuncs_runstart),
					   PAGE = 0
   .cinit            : > FLASH_GH,         PAGE = 0
   .const            : > FLASH_GH,         PAGE = 0
   .econst           : > FLASH_GH,         PAGE = 0
   .pinit            : > FLASH_GH,         PAGE = 0
   .reset            : > RESET,            PAGE = 0, TYPE = DSECT  /* We are not using the .reset section */
   .switch           : > FLASH_GH,         PAGE = 0

  /* Data Memory (PAGE 1) sections */
   .bss              : > L1SARAM,          PAGE = 1
/*   .ebss             : > H0SARAM,          PAGE = 1*/
   .ebss             : > L0SARAM,          PAGE = 0
   .cio              : > M0SARAM,          PAGE = 1
   .stack            : > M1SARAM,          PAGE = 1
   .sysmem           : > H0SARAM,          PAGE = 1
   .esysmem          : > H0SARAM,          PAGE = 1

/*** User Defined Sections ***/
   codestart         : > BEGIN_FLASH,      PAGE = 0                /* Used by file CodeStartBranch.asm */
   csm_rsvd          : > CSM_RSVD,         PAGE = 0                /* Used by file passwords.asm */
   passwords         : > PASSWORDS,        PAGE = 0                /* Used by file passwords.asm */


/* The Flash API functions can be grouped together as shown below.
      The defined symbols _Flash28_API_LoadStart, _Flash28_API_LoadEnd
      and _Flash28_API_RunStart are used to copy the API functions out
      of flash memory and into SARAM */
 
   Flash28_API:
   {
        -lFlash2812_API_V210.lib(.econst)
        -lFlash2812_API_V210.lib(.text)
   }                   LOAD = FLASH_AB,
                       RUN = L0SARAM,
                       LOAD_START(_Flash28_API_LoadStart),
                       LOAD_END(_Flash28_API_LoadEnd),
                       RUN_START(_Flash28_API_RunStart),
                       PAGE = 0


   bigbuffer         : > ZONE26,           PAGE = 1                /* Used for the data buffer */

/* Allocate IQ math areas: */
/*   IQmath              : > FLASH_CD        PAGE = 0 */

   IQmath	          : >FLASH_CD,
					   RUN = H0SARAM,
					   LOAD_START(_RamMath_loadstart),
                       LOAD_END(_RamMath_loadend),
                       RUN_START(_RamMath_runstart),
					   PAGE = 0

   IQmathTables        : > BOOTROM         PAGE = 0, TYPE = NOLOAD   /* Math Tables In ROM */
}

/******************* end of file ************************/
