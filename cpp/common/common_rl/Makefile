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

lib:	common_rl_test
	@./common_rl_test
	@cp -v common.h ../include
	@cp -v common.o ../obj
	@echo build complete

common.o:	common.h common.cpp Makefile
	@rm -f common.o
	g++ -c -O3 common.cpp  -std=gnu++0x
	
common_rl_test:	common.o common_rl_test.cpp
	@rm -f common_rl_test
	g++ -c common_rl_test.cpp -std=gnu++0x
	g++ -o common_rl_test common_rl_test.o common.o -std=gnu++0x

clean:
	@rm -rvf *.o  ../include/common.h ../obj/common.o common_rl_test
	@echo all objects and executables have been erased.
