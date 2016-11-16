//***************************************
// consts
//***************************************

enum {
  SIZE_Bsint32 = sizeof(Bsint32),
  SIZE_Buint32 = sizeof(Buint32),
  SIZE_Bsint16 = sizeof(Bsint16),
  SIZE_Buint16 = sizeof(Buint16),
};

enum ZipVersions {
  zv_Archiver = 25,
  zv_Minimal  = 20
};

enum ZipPlatforms {                  
  zp_DosFat,                // MS-DOS and OS/2 (FAT / VFAT / FAT32 file systems)
  zp_Amiga,                 // Amiga 
  zp_VaxVms,                // VAX/VMS
  zp_Unix,                  // Unix / Linux
  zp_VmCms,                 // VM/CMS
  zp_Atari,                 // Atari ST
  zp_Os2Hpfs,               // OS/2 H.P.F.S.
  zp_Macintosh,             // Macintosh 
  zp_Zsystem,               // Z-System
  zp_Cpm,                   // CP/M 
  zp_Ntfs                   // Windows NTFS
};

enum InternalAttr {
  ia_ROnly        = 0x01,
  ia_Hidd         = 0x02,
  ia_Sys          = 0x04,
  ia_Dir          = 0x10,
  ia_Arch         = 0x20
};

enum Versions {
  ver_Old    = 0x0a,
  ver_Normal = 0x14
};

enum Compressions {
  co_Deflate  = 8,
  co_Store    = 0
};

UMODSYS_CPU_DEF_FIELD_START(fileDirHeader)
  enum { SIGNATURE = 0x06054b50 };
  UMODSYS_CPU_DEF_FIELD_uint32_le(sig, START)
  UMODSYS_CPU_DEF_FIELD_uint16_le(nDisk, sig)
  UMODSYS_CPU_DEF_FIELD_uint16_le(nStartDisk, nDisk)
  UMODSYS_CPU_DEF_FIELD_uint16_le(nDirEntries, nStartDisk)
  UMODSYS_CPU_DEF_FIELD_uint16_le(totalDirEntries, nDirEntries)
  UMODSYS_CPU_DEF_FIELD_uint32_le(dirSize, totalDirEntries)
  UMODSYS_CPU_DEF_FIELD_uint32_le(dirOffset, dirSize)
  UMODSYS_CPU_DEF_FIELD_uint16_le(cmntLen, dirOffset)
UMODSYS_CPU_DEF_FIELD_END(fileDirHeader, cmntLen)

UMODSYS_CPU_DEF_FIELD_START(fileDirEntry)
  enum { SIGNATURE   = 0x02014b50 };
  UMODSYS_CPU_DEF_FIELD_uint32_le(sig, START)
  UMODSYS_CPU_DEF_FIELD_uint16_le(verMade, sig)
  UMODSYS_CPU_DEF_FIELD_uint16_le(verNeed, verMade)
  UMODSYS_CPU_DEF_FIELD_uint16_le(flag, verNeed)
  UMODSYS_CPU_DEF_FIELD_uint16_le(compression, flag)       // co_xxxx
  UMODSYS_CPU_DEF_FIELD_uint16_le(modTime, compression)
  UMODSYS_CPU_DEF_FIELD_uint16_le(modDate, modTime)
  UMODSYS_CPU_DEF_FIELD_uint32_le(crc32, modDate)
  UMODSYS_CPU_DEF_FIELD_uint32_le(cSize, crc32)            // Compressed size
  UMODSYS_CPU_DEF_FIELD_uint32_le(ucSize, cSize)           // Uncompressed size
  UMODSYS_CPU_DEF_FIELD_uint16_le(fnameLen, ucSize)        // Filename string follows header.
  UMODSYS_CPU_DEF_FIELD_uint16_le(xtraLen, fnameLen)       // Extra field follows filename.
  UMODSYS_CPU_DEF_FIELD_uint16_le(cmntLen, xtraLen)        // Comment field follows extra field.
  UMODSYS_CPU_DEF_FIELD_uint16_le(diskStart, cmntLen)
  UMODSYS_CPU_DEF_FIELD_uint16_le(intAttr, diskStart)
  UMODSYS_CPU_DEF_FIELD_uint32_le(extAttr, intAttr)
  UMODSYS_CPU_DEF_FIELD_uint32_le(hdrOffset, extAttr)
  //
  const char *GetName(const BByte* r) const { return reinterpret_cast<const char *>(p_end(r)); }
  const char *GetExtra(const BByte* r) const { return GetName(r) + fnameLen; }
  const char *GetComment(const BByte* r) const { return GetExtra(r) + xtraLen; }
  char *GetName(BByte* r) { return reinterpret_cast<char *>(p_end(r)); }
UMODSYS_CPU_DEF_FIELD_END(fileDirEntry, hdrOffset)

UMODSYS_CPU_DEF_FIELD_START(fileFilePrefix)
  enum { SIGNATURE = 0x04034b50 };
  UMODSYS_CPU_DEF_FIELD_uint32_le(sig, START)
  UMODSYS_CPU_DEF_FIELD_uint16_le(version, sig)
  UMODSYS_CPU_DEF_FIELD_uint16_le(flag, version)
  UMODSYS_CPU_DEF_FIELD_uint16_le(compression, flag)
  UMODSYS_CPU_DEF_FIELD_uint16_le(modTime, compression)
  UMODSYS_CPU_DEF_FIELD_uint16_le(modDate, modTime)
  UMODSYS_CPU_DEF_FIELD_uint32_le(crc32, modDate)
  UMODSYS_CPU_DEF_FIELD_uint32_le(cSize, crc32)
  UMODSYS_CPU_DEF_FIELD_uint32_le(ucSize, cSize)
  UMODSYS_CPU_DEF_FIELD_uint16_le(fnameLen, ucSize)
  UMODSYS_CPU_DEF_FIELD_uint16_le(xtraLen, fnameLen)
UMODSYS_CPU_DEF_FIELD_END(fileFilePrefix, xtraLen)
