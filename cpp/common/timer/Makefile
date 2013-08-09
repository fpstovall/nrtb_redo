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

lib:	timer_test
	@./timer_test
	@cp -v hires_timer.h ../include
	@cp -v hires_timer.o ../obj
	@echo build complete

hires_timer.o:	hires_timer.h hires_timer.cpp Makefile
	@rm -f hires_timer.o
	g++ -c -O3 hires_timer.cpp ${switches}

timer_test:	hires_timer.o timer_test.cpp
	@rm -f timer_test
	g++ -c timer_test.cpp ${switches}
	g++ -o timer_test timer_test.o hires_timer.o ${switches}

clean:
	@rm -rvf *.o timer_test ../include/hires_timer.h ../obj/hires_timer.o
	@echo all objects and executables have been erased.
