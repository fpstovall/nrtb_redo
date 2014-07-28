#***********************************************
# This file is part of the NRTB project (https://github.com/fpstovall/nrtb_alpha).
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
	@cp -v location_data.h ../include/
	@cp -v location_data.o ../obj/
	@echo location_data build complete!

location_data_test:	location_data.o location_data_test.cpp
	@rm -f location_data_test
	g++ -c location_data_test.cpp -I ../../include -I ../include -std=gnu++0x 
	g++ -o location_data_test location_data_test.o location_data.o ../obj/triplet.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x 

location_data.o:	location_data.cpp location_data.h Makefile
	@rm -f location_data.o
	g++ -c -O3 location_data.cpp -I ../include -I../../include -std=gnu++0x 

clean:
	@rm -vf *.o ../include/location_data.h ../obj/location_data.o location_data_test
	@echo all objects and executables have been erased.


