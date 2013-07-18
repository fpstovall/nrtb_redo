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

lib:	logger_test
	@./logger_test
	@cp -v logger.h ../include/
	@cp -v logger.o ../obj/
	@echo build complete

logger_test:	logger.o logger_test.cpp
	@rm -f logger_test
	g++ -c -O3 logger_test.cpp -I ../include ${switches} 
	g++ -o logger_test logger_test.o logger.o ../obj/common.o -lpthread ${switches}


logger.o:	logger.cpp logger.h Makefile
	@rm -f logger.o
	g++ -c -O3 logger.cpp -I ../include ${switches} 

clean:
	@rm -vf *.o ../include/logger.h logger_test
	@echo all objects and executables have been erased.

