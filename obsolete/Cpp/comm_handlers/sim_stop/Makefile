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

lib:	sim_stop_test
	@./sim_stop_test
	@cp -v sim_stop.h ../include/
	@cp -v sim_stop.o ../obj/
	@echo sim_stop build complete!

sim_stop_test:	sim_stop.o sim_stop_test.cpp
	@rm -f sim_stop_test
	g++ -c sim_stop_test.cpp -I ../../include -I ../include -std=gnu++0x
	g++ -o sim_stop_test sim_stop_test.o sim_stop.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x

sim_stop.o:	sim_stop.cpp sim_stop.h Makefile
	@rm -f sim_stop.o
	g++ -c -O3 sim_stop.cpp -I ../include -I ../../include -std=gnu++0x

clean:
	@rm -vf *.o ../include/sim_stop.h ../obj/sim_stop.o sim_stop_test
	@echo all objects and executables have been erased.

