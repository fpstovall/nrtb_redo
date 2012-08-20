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

lib: common

common: Makefile common.d ${modules}
	@echo building ${modules}
	@cd nrtb/common; dmd -lib -of../common.a *.d -I../../
	@dmd common.d -unittest -Lnrtb/common.a
	@./common
	@echo common build complete

clean:
	@rm -vf nrtb/common.a common *.o
	@echo all objects and executables have been erased.

modules=`find nrtb/common -type f -name \*.d`