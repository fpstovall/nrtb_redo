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

lib:	tq_obj_test
	@./tq_obj_test
	@cp -v tq_obj.h ../include/
	@cp -v tq_obj.o ../obj/
	@echo tq_obj_update build complete!

tq_obj_test:	tq_obj.o tq_obj_test.cpp
	@rm -f tq_obj_test
	g++ -c tq_obj_test.cpp -I ../../include -I ../include -std=gnu++0x
	g++ -o tq_obj_test tq_obj_test.o tq_obj.o ../obj/triplet.o ../obj/location_data.o ../obj/velocity_data.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x

tq_obj.o:	tq_obj.cpp tq_obj.h Makefile
	@rm -f tq_obj.o
	g++ -c -O3 tq_obj.cpp -I ../include -I../../include -std=gnu++0x

clean:
	@rm -vf *.o ../include/tq_obj.h ../obj/tq_obj.o tq_obj_test
	@echo all objects and executables have been erased.

