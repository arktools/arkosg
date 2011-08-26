prefix=/usr
exec_prefix=${prefix}

Name: arkosg
Description: The arkOsg library
Version: 0.1.0
Cflags: -I${prefix}/include/arkOsg
Libs: -L${exec_prefix}/lib -larkOsg
