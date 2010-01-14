
# makefile for the plugin test routine.

test:	../plugin_wrapper.o ../plugin_manager.o plugintest.cpp
	@rm -f plugintest
	g++ -c plugintest.cpp -idirafter ../../include3 -idirafter /usr/local/include/boost-1_31
#	g++ -c -g conftest.cpp -idirafter ../../include3 -idirafter /usr/local/include/boost-1_31
	g++ -o plugintest plugintest.o ../plugin_wrapper.o ../plugin_manager.o -l ricklib3  -L../../lib/
	./plugintest

clean:
	@rm -rvf *.o plugintest
	@echo all objects and executables have been erased.
