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

build:	base_socket.o Makefile
	@echo build complete
	
base_socket.o:	base_socket.cpp base_socket.h Makefile
	@rm -f base_socket.o
	gcc -c -O3 base_socket.cpp -I ../include

clean:
	@rm -vf *.o ../../include3/base_socket.h
	@echo all objects and executables have been erased.

lib:	base_socket.o ../../include3/base_socket.h
#	@ar -r ../../lib/libricklib3.a base_socket.
	@echo the base_socket class has been added to the library

../../include3/base_socket.h: base_socket.h
	@cp -fv base_socket.h ../../include3
