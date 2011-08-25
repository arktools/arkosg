prefix=/usr
exec_prefix=${prefix}

Name: mavsim
Description: The mavsim library
Version: 0.1.0
Cflags: -I${prefix}/include/mavsim
Libs: -L${exec_prefix}/lib -lmavsimCommunication -lmavsimVisualization
