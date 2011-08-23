#***********************************************
#This file is part of the NRTB project (https://launchpad.net/nrtb).
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

lib:	transceiver_test
	@./transceiver_test
	@cp -v transceiver.h ../include
	@echo build complete

transceiver_test:	transceiver.h transceiver_test.cpp
	@rm -f transceiver_test
	g++ -c transceiver_test.cpp -I../include
	g++ -o transceiver_test transceiver_test.o ../obj/common.o ../obj/log_setup.o ../obj/serializer.o ../obj/base_thread.o ../obj/base_socket.o ../obj/confreader.o -lpthread -lprotobuf  ../lib/nrtb_gpb.a -lPocoFoundation

clean:
	@rm -rvf *.o transceiver_test ../include/transceiver.h ../obj/transceiver.o
	@echo all objects and executables have been erased.
