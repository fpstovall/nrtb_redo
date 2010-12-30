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

lib:	transciever_test
	@cp -v transciever.h ../include
	@cp -v transciever.o ../obj
	@echo build complete

../include/confreader.h:
	@cp ../confreader; make lib

transciever.o:	transciever.h transciever.cpp Makefile ../include/confreader.h
	@rm -f transciever.o
	g++ -c transciever.cpp -I ../include

transciever_test:	transciever.o transciever_test.cpp
	@rm -f transciever_test
	g++ -c transciever_test.cpp
	g++ -o transciever_test transciever_test.o transciever.o ../obj/common.o ../obj/log_setup.o -lPocoFoundation -lPocoUtil

clean:
	@rm -rvf *.o transciever_test ../include/transciever.h ../obj/transciever.o
	@echo all objects and executables have been erased.
