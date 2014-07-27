#***********************************************
#This file is part of the NRTB project (https://github.com/fpstovall/nrtb_alpha).
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

lib:	thread_test
	@./thread_test
	@cp -v base_thread.h ../include
	@cp -v base_thread.o ../obj
	@echo build complete

base_thread.o:	base_thread.cpp base_thread.h Makefile
	@rm -f base_thread.o
	g++ -c -O3 base_thread.cpp -I ../include -std=gnu++0x

thread_test:	base_thread.o thread_test.cpp
	@rm -f thread_test
	g++ -c thread_test.cpp -I../include -std=gnu++0x
	g++ -o thread_test thread_test.o base_thread.o ../obj/hires_timer.o ../obj/common.o -lpthread -std=gnu++0x

clean:
	@rm -rvf *.o thread_test ../include/base_thread.h ../obj/base_thread.o
	@echo all objects and executables have been erased.
