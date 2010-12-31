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

lib:	log_test
	@echo "Testing..."
	@rm -f test_output.log
	@./log_test
	@grep "Program run complete." test_output.log
	@cp -fv log_setup.h ../include
	@cp -fv log_setup.o ../obj


log_setup.o:	log_setup.h log_setup.cpp Makefile
	@rm -f log_setup.o
	g++ -c log_setup.cpp -I ../include

log_test:	log_setup.o log_test.cpp Makefile
	@rm -vf log_test
	g++ -c log_test.cpp -I../include 
	g++ -o log_test log_setup.o log_test.o -lPocoFoundation
#	g++ -g common_test.cpp  -idirafter . -o common_test

clean:
	@rm -vf *.o log_test ../include/log_setup.h ../obj/log_setup.o



