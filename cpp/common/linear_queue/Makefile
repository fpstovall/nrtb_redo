#***********************************************
#This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
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

switches=-std=gnu++17 -D _GLIBCXX_USE_SCHED_YIELD -D _GLIBCXX_USE_NANOSLEEP

lib:	linear_queue_test
	@./linear_queue_test
	@cp -v linear_queue.h ../include
	@echo build complete

linear_queue_test:	linear_queue.h linear_queue_test.cpp
	@rm -f linear_queue_test
	g++ -c linear_queue_test.cpp -I../include ${switches}
	g++ -o linear_queue_test linear_queue_test.o -lpthread ${switches}

clean:
	@rm -rvf *.o linear_queue_test ../include/linear_queue.h *.log ../obj/linear_queue.o
	@echo all objects and executables have been erased.
