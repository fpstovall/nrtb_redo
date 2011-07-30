#***********************************************
#This file is part of the NRTB project (https://launchpad.net/nrtb).
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

lib:	serializer_test
	@./serializer_test
	@cp -v serializer.h ../include
	@cp -v serializer.o ../obj
	@echo build complete

serializer.o:	serializer.h serializer.cpp Makefile
	@rm -f serializer.o
	g++ -c serializer.cpp -I ../include

serializer_test:	serializer.o serializer_test.cpp
	@rm -f serializer_test
	g++ -c serializer_test.cpp -I../include
	g++ -o serializer_test serializer_test.o serializer.o ../obj/base_thread.o -lpthread ../obj/common.o

clean:
	@rm -rvf *.o serializer_test ../include/serializer.h ../obj/serializer.o
	@echo all objects and executables have been erased.
