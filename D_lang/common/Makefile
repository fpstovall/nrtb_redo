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

lib: ../lib/common.a

../lib/common.a: test
	@echo building ${modules}
	@cd nrtb/common; dmd -lib -release -of../../../lib/common.a *.d -I../../
	@echo " ********* common build complete ***********"

test:	Makefile common.d ${modules}
	@echo testing ${modules}
	@cd nrtb/common; dmd -unittest -lib -of../../common.a *.d -I../../
	@dmd -unittest common.a -run common.d

clean:
	@rm -vf ../lib/common.a common *.o *.a
	@echo all objects and executables have been erased.

modules=`find nrtb/common -type f -name \*.d`