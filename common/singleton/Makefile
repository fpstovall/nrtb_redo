
build:	singleton_test
	@echo build complete

singleton_test:	singleton_test.cpp Makefile
	@rm -f singleton_test
	g++ -c singleton_test.cpp -idirafter ../../include3 -idirafter /usr/local/include/boost-1_31
#	g++ -c -g singleton_test.cpp -idirafter ../../include3 -idirafter /usr/local/include/boost-1_31
	g++ -o singleton_test singleton_test.o -lricklib3 -lpthread -lssl -L../../lib/

clean:
	@rm -rvf *.o singleton_test ../../include3/singleton.h
	@echo all objects and executables have been erased.

lib:	../../include3/singleton.h

../../include3/singleton.h: singleton.h
	@cp -fv singleton.h ../../include3
