
set(S ${SRC}/core)
set(SP ${SRC}/core/${PLATSRC})

include_directories (${INC})

add_library(umodsys.core STATIC
 ${S}/umodsys.core.objects.cpp
 ${S}/umodsys.core.stdtypedefs.cpp
 ${S}/umodsys.core.uniqueptr.cpp
 ${S}/umodsys.core.uniqueptrlist.cpp
 ${S}/umodsys.tl.strings.cpp
 ${S}/umodsys.tl.utf8.cpp
 ${S}/umodsys.tl.utf16.cpp
 ${S}/umodsys.tl.utfcase.cpp
 ${SP}/umodsys.core.syshlp_chars.${PID}.cpp
 ${SP}/umodsys.core.syshlp_charcode.${PID}.cpp
 ${SP}/umodsys.core.syshlp_files.${PID}.cpp
 ${SP}/umodsys.core.syshlp_proc.${PID}.cpp
 ${SP}/umodsys.core.syshlp_thread.${PID}.cpp
 ${SP}/umodsys.core.syshlp_vars.${PID}.cpp
)

