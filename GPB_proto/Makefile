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

lib/nrtb_gpb.a:
	@protoc -I=proto/ --cpp_out=cpp_src/ proto/*.proto
	@cd obj; for file in ../cpp_src/*.cc; do g++ -c $$file; done
	@cp -v cpp_src/*h include
	@ar -r lib/nrtb_gpb.a obj/*.o
	@echo "GPB lib build complete."

clean:
	@rm -vf lib/* obj/* cpp_src/* include/*
	@echo "GPB cleanup complete."
