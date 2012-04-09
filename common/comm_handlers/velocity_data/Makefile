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

lib:	velocity_data_test
	@./velocity_data_test
	@cp -v velocity_data.h ../include/
	@cp -v velocity_data.o ../obj/
	@echo velocity_data build complete!

velocity_data_test:	velocity_data.o velocity_data_test.cpp
	@rm -f velocity_data_test
	g++ -c velocity_data_test.cpp -I ../../include -I ../include -std=gnu++0x
	g++ -o velocity_data_test velocity_data_test.o velocity_data.o ../obj/triplet.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x

velocity_data.o:	velocity_data.cpp velocity_data.h Makefile
	@rm -f velocity_data.o
	g++ -c -O3 velocity_data.cpp -I ../include -I../../include -std=gnu++0x

clean:
	@rm -vf *.o ../include/velocity_data.h ../obj/velocity_data.o velocity_data_test
	@echo all objects and executables have been erased.

