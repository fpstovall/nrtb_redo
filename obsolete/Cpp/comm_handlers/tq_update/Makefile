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

lib:	tq_update_test
	@./tq_update_test
	@cp -v tq_update.h ../include/
	@cp -v tq_update.o ../obj/
	@echo tq_update_update build complete!

tq_update_test:	tq_update.o tq_update_test.cpp
	@rm -f tq_update_test
	g++ -c tq_update_test.cpp -I ../../include -I ../include -std=gnu++0x
	g++ -o tq_update_test tq_update_test.o tq_update.o ../obj/triplet.o ../obj/location_data.o ../obj/velocity_data.o ../obj/tq_obj.o ../obj/obj_setup_info.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x

tq_update.o:	tq_update.cpp tq_update.h Makefile
	@rm -f tq_update.o
	g++ -c -O3 tq_update.cpp -I ../include -I../../include -std=gnu++0x

clean:
	@rm -vf *.o ../include/tq_update.h ../obj/tq_update.o tq_update_test
	@echo all objects and executables have been erased.

