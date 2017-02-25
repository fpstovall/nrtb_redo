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

lib:	ack_nak_test
	@./ack_nak_test
	@cp -v ack_nak.h ../include/
	@cp -v ack_nak.o ../obj/
	@echo ack_nak build complete!

ack_nak_test:	ack_nak.o ack_nak_test.cpp
	@rm -f ack_nak_test
	g++ -c ack_nak_test.cpp -I ../../include -I ../include -std=gnu++0x 
	g++ -o ack_nak_test ack_nak_test.o ack_nak.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x 

ack_nak.o:	ack_nak.cpp ack_nak.h Makefile
	@rm -f ack_nak.o
	g++ -c -O3 ack_nak.cpp -I ../include -I ../../include -std=gnu++0x 

clean:
	@rm -vf *.o ../include/ack_nak.h ../obj/ack_nak.o ack_nak_test
	@echo all objects and executables have been erased.

