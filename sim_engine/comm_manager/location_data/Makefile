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

lib:	location_data_test
	@./location_data_test
	@cp -v location_data.h ../../include/
	@cp -v location_data.o ../../obj/
	@echo location_data build complete!

location_data_test:	location_data.o location_data_test.cpp
	@rm -f location_data_test
	g++ -c location_data_test.cpp -I ../../../common/include -I ../../include
	g++ -o location_data_test location_data_test.o location_data.o ../../obj/triplet.o ../../../common/lib/nrtb_common.a ../../../common/lib/nrtb_gpb.a -lprotobuf -lpthread

location_data.o:	location_data.cpp location_data.h Makefile ../../../common/lib/nrtb_common.a
	@rm -f location_data.o
	g++ -c -O3 location_data.cpp -I ../../include -I../../../common/include

clean:
	@rm -vf *.o ../../include/location_data.h location_data_test
	@echo all objects and executables have been erased.

../../../common/lib/nrtb_common.a: 
	@cd ../../../common; make