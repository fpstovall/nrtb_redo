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

lib:	sim_termination_test
	@./sim_termination_test
	@cp -v sim_termination.h ../include/
	@cp -v sim_termination.o ../obj/
	@echo sim_termination build complete!

sim_termination_test:	sim_termination.o sim_termination_test.cpp
	@rm -f sim_termination_test
	g++ -c sim_termination_test.cpp -I ../../include -I ../include -std=gnu++0x
	g++ -o sim_termination_test sim_termination_test.o sim_termination.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x

sim_termination.o:	sim_termination.cpp sim_termination.h Makefile
	@rm -f sim_termination.o
	g++ -c -O3 sim_termination.cpp -I ../include -I../../include -std=gnu++0x

clean:
	@rm -vf *.o ../include/sim_termination.h ../obj/sim_termination.o sim_termination_test
	@echo all objects and executables have been erased.

