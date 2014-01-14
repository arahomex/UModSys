
#include "ntverp.h"

#if !defined(VER_PRODUCTBUILD) || VER_PRODUCTBUILD == 6000

typedef EXCEPTION_DISPOSITION (*PEXCEPTION_ROUTINE) ( 
  struct _EXCEPTION_RECORD *ExceptionRecord, 
  PVOID EstablisherFrame, 
  struct _CONTEXT *ContextRecord, 
  PVOID DispatcherContext 
); 

#define UNWIND_HISTORY_TABLE_SIZE 12 

typedef struct _UNWIND_HISTORY_TABLE_ENTRY { 
  ULONG64 ImageBase; 
  PRUNTIME_FUNCTION FunctionEntry; 
} UNWIND_HISTORY_TABLE_ENTRY, *PUNWIND_HISTORY_TABLE_ENTRY; 

typedef struct _UNWIND_HISTORY_TABLE { 
  ULONG Count; 
  UCHAR Search; 
  ULONG64 LowAddress; 
  ULONG64 HighAddress; 
  UNWIND_HISTORY_TABLE_ENTRY Entry[UNWIND_HISTORY_TABLE_SIZE]; 
} UNWIND_HISTORY_TABLE, *PUNWIND_HISTORY_TABLE; 

VOID NTAPI RtlUnwindEx( 
  PVOID TargetFrame, 
  PVOID TargetIp, 
  PEXCEPTION_RECORD ExceptionRecord, 
  PVOID ReturnValue, 
  PCONTEXT OriginalContext, 
  PUNWIND_HISTORY_TABLE HistoryTable 
);

#endif
