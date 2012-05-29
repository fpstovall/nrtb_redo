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

lib: lib/common.a

lib/common.a: bin/common_test
	@dmd -ofmodules/common.a -Dddocs -lib */*.d
	@echo common build complete

bin/common_test: common.d Makefile
	@dmd -ofbin/common_test -unittest common.d */*.d
	@bin/common_test

clean:
	@rm -vf *.o modules/common.a bin/common_test
	@echo all objects and executables have been erased.

