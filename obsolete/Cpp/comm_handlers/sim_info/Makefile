#***********************************************
# This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
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

lib:	sim_setup_info_test
	@./sim_setup_info_test
	@cp -v sim_setup_info.h ../include/
	@cp -v sim_setup_info.o ../obj/
	@echo sim_setup_info build complete!

sim_setup_info_test:	sim_setup_info.o sim_setup_info_test.cpp
	@rm -f sim_setup_info_test
	g++ -c sim_setup_info_test.cpp -I ../../include -I ../include -std=gnu++0x
	g++ -o sim_setup_info_test sim_setup_info_test.o sim_setup_info.o ../obj/triplet.o ../obj/location_data.o ../obj/velocity_data.o ../obj/obj_setup_info.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x

sim_setup_info.o:	sim_setup_info.cpp sim_setup_info.h Makefile
	@rm -f sim_setup_info.o
	g++ -c -O3 sim_setup_info.cpp -I ../include -I../../include -std=gnu++0x

clean:
	@rm -vf *.o ../include/sim_setup_info.h ../obj/sim_setup_info.o sim_setup_info_test
	@echo all objects and executables have been erased.

