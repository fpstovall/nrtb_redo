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

lib:	sim_db_wrapper_test
	@./sim_db_wrapper_test
	@cp -v sim_db_wrapper.h ../include/
	@cp -v sim_db_wrapper.o ../obj/
	@echo sim_db_wrapper build complete!

sim_db_wrapper_test:	sim_db_wrapper.o sim_db_wrapper_test.cpp
	@rm -f sim_db_wrapper_test
	g++ -c sim_db_wrapper_test.cpp -I ../../include -I ../include -std=gnu++0x
	g++ -o sim_db_wrapper_test sim_db_wrapper_test.o sim_db_wrapper.o ../obj/ack_nak.o ../obj/obj_setup_info.o ../obj/sim_stop.o  ../obj/tq_obj.o ../obj/location_data.o ../obj/sim_setup_info.o  ../obj/sim_termination.o ../obj/tq_update.o ../obj/velocity_data.o   ../obj/triplet.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x

sim_db_wrapper.o:	sim_db_wrapper.cpp sim_db_wrapper.h Makefile
	@rm -f sim_db_wrapper.o
	g++ -c -O3 sim_db_wrapper.cpp -I ../include -I ../../include -std=gnu++0x

clean:
	@rm -vf *.o ../include/sim_db_wrapper.h ../obj/sim_db_wrapper.o sim_db_wrapper_test
	@echo all objects and executables have been erased.

