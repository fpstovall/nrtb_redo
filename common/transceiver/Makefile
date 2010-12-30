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

lib:	transceiver_test
	@cp -v transceiver.h ../include
	@cp -v transceiver.o ../obj
	@echo build complete

../include/confreader.h:
	@cp ../confreader; make lib

transceiver.o:	transceiver.h transceiver.cpp Makefile ../include/confreader.h
	@rm -f transceiver.o
	g++ -c transceiver.cpp -I ../include

transceiver_test:	transceiver.o transceiver_test.cpp
	@rm -f transceiver_test
	g++ -c transceiver_test.cpp
	g++ -o transceiver_test transceiver_test.o transceiver.o ../obj/common.o ../obj/log_setup.o -lPocoFoundation -lPocoUtil

clean:
	@rm -rvf *.o transceiver_test ../include/transceiver.h ../obj/transceiver.o
	@echo all objects and executables have been erased.
