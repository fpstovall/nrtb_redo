#TODO: need to revamp his to build the triplet component.

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

lib:	sim_db_channel_test
	@./sim_db_channel_test
	@cp -v sim_db_channel.h ../include/
	@cp -v sim_db_channel.o ../obj/
	@echo build complete

../../common/lib/nrtb_common.a:
	@cd ../../commmon; make

sim_db_channel_test:	sim_db_channel.o sim_db_channel_test.cpp ../../common/lib/nrtb_common.a
	@rm -f sim_db_channel_test
	g++ -c sim_db_channel_test.cpp -I ../include
	g++ -o sim_db_channel_test sim_db_channel_test.o sim_db_channel.o -L ../../common/lib/
	nrtb_common.a nrtb_gpb.a -lpthread 


sim_db_channel.o:	sim_db_channel.cpp sim_db_channel.h Makefile
	@rm -f sim_db_channel.o
	g++ -c -O3 sim_db_channel.cpp -I ../../common/include

clean:
	@rm -vf *.o ../include/sim_db_channel.h sim_db_channel_test
	@echo all objects and executables have been erased.

