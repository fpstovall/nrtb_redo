build:	base_socket.o Makefile
	@echo build complete
	
base_socket.o:	base_socket.cpp base_socket.h Makefile
	@rm -f base_socket.o
	gcc -c -O3 base_socket.cpp -I ../include
#	gcc -c -g base_socket.cpp -idirafter ../ricks_handy -idirafter ../threads -idirafter ../timer -idirafter /usr/local/include/boost-1_31

clean:
	@rm -vf *.o ../../include3/base_socket.h
	ar -d ../../lib/libricklib3.a base_socket.o
	@echo all objects and executables have been erased.

lib:	base_socket.o ../../include3/base_socket.h
	@ar -r ../../lib/libricklib3.a base_socket.o
	@echo the base_socket class has been added to the library

../../include3/base_socket.h: base_socket.h
	@cp -fv base_socket.h ../../include3
