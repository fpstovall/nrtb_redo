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

switches=-std=gnu++11 -D _GLIBCXX_USE_SCHED_YIELD -D _GLIBCXX_USE_NANOSLEEP

lib:	circular_queue_test
	@./circular_queue_test
	@cp -v circular_queue.h ../include
	@echo build complete

circular_queue_test:	circular_queue.h circular_queue_test.cpp
	@rm -f circular_queue_test
	g++ -c circular_queue_test.cpp -I../include ${switches}
	g++ -o circular_queue_test circular_queue_test.o -lpthread ${switches}

clean:
	@rm -rvf *.o circular_queue_test ../include/circular_queue.h *.log ../obj/circular_queue.o
	@echo all objects and executables have been erased.
