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

lib:	socket_test
	@./socket_test
	@cp -v base_socket.h ../include/
	@cp -v base_socket.o ../obj/
	@echo build complete

socket_test:	base_socket.o socket_test.cpp
	@rm -f socket_test
	gcc -c socket_test.cpp -I ../include
	gcc -o socket_test socket_test.o base_socket.o ../obj/hires_timer.o ../obj/common.o ../obj/base_thread.o -lpthread -lm


base_socket.o:	base_socket.cpp base_socket.h Makefile
	@rm -f base_socket.o
	gcc -c -O3 base_socket.cpp -I ../include

clean:
	@rm -vf *.o ../include/base_socket.h test-server
	@echo all objects and executables have been erased.

