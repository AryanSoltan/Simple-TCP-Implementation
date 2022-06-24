inc_dir = ./Includes
src_dir = ./Src
build_dir = ./Build
cfalgs = -std=c++11
cc = g++

All: Router Host

Router: ${build_dir}/Router.o ${build_dir}/PacketTool.o ${build_dir}/SocketTool.o
	${cc} ${cfalgs} ${build_dir}/Router.o ${build_dir}/PacketTool.o ${build_dir}/SocketTool.o -o Router.out

${build_dir}/Router.o: ${inc_dir}/IncludesAndDefines.hpp ${inc_dir}/PacketTool.hpp ${src_dir}/Router.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/Router.cpp -o ${build_dir}/Router.o

${build_dir}/PacketTool.o: ${inc_dir}/IncludesAndDefines.hpp ${inc_dir}/PacketTool.hpp ${src_dir}/PacketTool.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/PacketTool.cpp -o ${build_dir}/PacketTool.o

${build_dir}/SocketTool.o: ${inc_dir}/IncludesAndDefines.hpp ${inc_dir}/SocketTool.hpp ${src_dir}/SocketTool.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/SocketTool.cpp -o ${build_dir}/SocketTool.o

Host: ${build_dir}/Host.o ${build_dir}/SocketTool.o ${build_dir}/PacketTool.o ${build_dir}/DataRecivier.o
	${cc} ${cfalgs} ${build_dir}/Host.o ${build_dir}/SocketTool.o ${build_dir}/PacketTool.o ${build_dir}/DataRecivier.o -o Host.out

${build_dir}/Host.o: ${inc_dir}/IncludesAndDefines.hpp ${inc_dir}/SocketTool.hpp ${inc_dir}/PacketTool.hpp ${inc_dir}/DataRecivier.hpp ${src_dir}/Host.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/Host.cpp -o ${build_dir}/Host.o

${build_dir}/DataRecivier.o: ${inc_dir}/IncludesAndDefines.hpp ${inc_dir}/DataRecivier.hpp ${src_dir}/DataRecivier.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/DataRecivier.cpp -o ${build_dir}/DataRecivier.o

clear:
	rm Client.out Server.out ${build_dir}/*