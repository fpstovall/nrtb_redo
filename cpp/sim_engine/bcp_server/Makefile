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

target=bcp_server

lib:	${target}_test
	@./${target}_test
	@cp -v ${target}.h ../include/
	@cp -v ${target}.o ../obj/
	@ar -r ../lib/sim_engine.a ${target}.o
	@echo build complete

${target}_test:	${target}.o ${target}_test.cpp
	@rm -f ${target}_test
	g++ -c -O3 ${target}_test.cpp ${bargs} 
	g++ -o ${target}_test ${target}_test.o ${target}.o ${largs}


${target}.o:	${target}.cpp ${target}.h Makefile
	@rm -f ${target}.o
	g++ -c -O3 ${target}.cpp ${bargs} 

clean:
	@rm -vf *.o *.log ../include/${target}.h ../obj/${target}.o ${target}_test
	@echo all objects and executables have been erased.

switches=-std=gnu++11 -D _GLIBCXX_USE_SCHED_YIELD -D _GLIBCXX_USE_NANOSLEEP 
libs=../lib/sim_engine.a ../../common/lib/nrtb_common.a -lpthread
include=-I../../common/include -I../include
bargs=${switches} ${include}
largs=${switches} ${libs}


