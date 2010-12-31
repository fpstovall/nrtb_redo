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

lib:	conftest
	@rm -f conf_test.log
	@./conftest test=1 test2-2 test2=71.837486 test3="jack danials" --doit
	@grep "Run Complete" conf_test.log
	@cp -v confreader.h ../include
	@cp -v confreader.o ../obj
	@echo build complete

../include/common.h: 
	@cd ../common_rl; make lib

../include/log_setup.h:
	@cd ../logger; make lib

confreader.o:	confreader.h confreader.cpp Makefile ../include/common.h  ../include/log_setup.h
	@rm -f confreader.o
	g++ -c confreader.cpp -I ../include

conftest:	confreader.o conftest.cpp
	@rm -f conftest
	g++ -c conftest.cpp
	g++ -o conftest conftest.o confreader.o ../obj/common.o ../obj/log_setup.o -lPocoFoundation -lPocoUtil

clean:
	@rm -rvf *.o conftest ../include/confreader.h ../obj/confreader.o
	@echo all objects and executables have been erased.
