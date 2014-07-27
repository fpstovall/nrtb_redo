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

switches=-std=gnu++11 -D _GLIBCXX_USE_SCHED_YIELD -D _GLIBCXX_USE_NANOSLEEP

lib:	conftest
	@rm -f conf_test.log
	@./conftest test=1 test2-2 test2=71.837486 test3="jack danials" --doit
	@grep "Run Complete" conf_test.log
	@cp -v confreader.h ../include
	@cp -v confreader.o ../obj
	@echo build complete

confreader.o:	confreader.h confreader.cpp Makefile 
	@rm -f confreader.o
	g++ -c confreader.cpp -I../include ${switches}

conftest:	confreader.o conftest.cpp
	@rm -f conftest
	g++ -c conftest.cpp -I../include ${switches} 
	g++ -o conftest conftest.o confreader.o ../obj/common.o ../obj/logger.o -lpthread ${switches}

clean:
	@rm -rvf *.o conftest ../include/confreader.h ../obj/confreader.o conf_test.log
	@echo all objects and executables have been erased.
