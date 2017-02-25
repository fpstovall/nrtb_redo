#***********************************************
#This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
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

proto=../../GPB_proto

lib:	lib/nrtb_gpb.a
	@echo "GPB lib build complete."

lib/nrtb_gpb.a:
	@protoc -I=${proto} --cpp_out=cpp_src/ ${proto}/*.proto
	@cd obj; for file in ../cpp_src/*.cc; do g++ -c $$file -std=gnu++0x; done
	@cp -v cpp_src/*h ../include
	@ar -r ../lib/nrtb_gpb.a obj/*.o

clean:
	@cd cpp_src; for file in *h; do rm -vf ../../include/$$file; done
	@rm -vf ../lib/nrtb_gpb.a
	@rm -f obj/* cpp_src/*
	@echo "GPB cleanup complete."
