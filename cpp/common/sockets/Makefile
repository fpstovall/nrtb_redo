#***********************************************
# This file is part of the NRTB project (https://launchpad.net/nrtb).
#
#    NRTB is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    NRTB is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with NRTB.  If not, see <http://www.gnu.org/licenses/>.
#
#***********************************************

switches=-std=gnu++11 -D _GLIBCXX_USE_SCHED_YIELD -D _GLIBCXX_USE_NANOSLEEP

lib:	socket_test
	@./socket_test
	@cp -v base_socket.h ../include/
	@cp -v base_socket.o ../obj/
	@echo build complete

socket_test:	base_socket.o socket_test.cpp
	@rm -f socket_test
	g++ -c -O3 socket_test.cpp -I ../include ${switches} 
	g++ -o socket_test socket_test.o base_socket.o -lpthread ${switches}


base_socket.o:	base_socket.cpp base_socket.h Makefile
	@rm -f base_socket.o
	g++ -c -O3 base_socket.cpp -I ../include ${switches} 

clean:
	@rm -vf *.o ../include/base_socket.h socket_test
	@echo all objects and executables have been erased.

