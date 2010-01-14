
build:	plugin_wrapper.o plugin_manager.o
	@echo build complete

plugin_wrapper.o:	plugin_wrapper.h plugin_wrapper.cpp Makefile
	@rm -f plugin_wrapper.o
	g++ -c -O3 plugin_wrapper.cpp -idirafter ../../include3
#	g++ -c -g plugin_wrapper.cpp -idirafter ../../include3 -idirafter /usr/local/include/boost-1_31

plugin_manager.o:	plugin_manager.h plugin_manager.cpp Makefile
	@rm -f plugin_manager.o
	g++ -c -O3 plugin_manager.cpp -idirafter ../../include3
#	g++ -c -g plugin_wrapper.cpp -idirafter ../../include3 -idirafter /usr/local/include/boost-1_31

test:	plugin_wrapper.o plugin_manager.o wrapper_test.cpp
	@rm -f plugin_test
	g++ -c plugin_test.cpp -idirafter ../../include3 -idirafter /usr/local/include/boost-1_31
#	g++ -c -g conftest.cpp -idirafter ../../include3 -idirafter /usr/local/include/boost-1_31
	g++ -o plugtest plugtest.o plugin_wrapper.o plugin_manager.o -l ricklib3  -L../../lib/

clean:
	@rm -rvf *.o plugin_test ../../include3/plugin_wrapper.h ../../include3/plugin_manager.h
	ar -d ../../lib/libricklib3.a plugin_wrapper.o plugin_manager.o
	@echo all objects and executables have been erased.

lib:	test
	@ar -r ../../lib/libricklib3.a plugin_wrapper.o plugin_manager.o
	@echo the plugin_wrapper class has been added to the library
	@cp -fv plugin_wrapper.h plugin_manager.h ../../include3
