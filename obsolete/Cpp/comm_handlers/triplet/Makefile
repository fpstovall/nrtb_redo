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

lib:	triplet_test
	@./triplet_test
	@cp -v triplet.h ../include/
	@cp -v triplet.o ../obj/
	@echo triplet build complete!

triplet_test:	triplet.o triplet_test.cpp
	@rm -f triplet_test
	g++ -c triplet_test.cpp -I ../../include -std=gnu++0x
	g++ -o triplet_test triplet_test.o triplet.o ../../lib/nrtb_common.a ../../lib/nrtb_gpb.a -lprotobuf -lpthread -std=gnu++0x

triplet.o:	triplet.cpp triplet.h Makefile
	@rm -f triplet.o
	g++ -c -O3 triplet.cpp -I../../include -std=gnu++0x

clean:
	@rm -vf *.o ../include/triplet.h ../obj/triplet.o triplet_test
	@echo all objects and executables have been erased.

