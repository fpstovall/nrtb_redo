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

target=../bin/simengine

${target}:	simengine.o
	@rm -f ${target}
	g++ -o ${target} simengine.o ${largs}


simengine.o:	simengine.cpp Makefile
	@rm -f simengine.o
	g++ -c -O3 simengine.cpp ${bargs} 

clean:
	@rm -vf *.o ${target}
	@echo all objects and executables have been erased.

lib: ${target}

switches=-std=gnu++11 -D _GLIBCXX_USE_SCHED_YIELD -D _GLIBCXX_USE_NANOSLEEP 
libs=../../common/lib/nrtb_common.a -lpthread
include=-I../../common/include -I../include
bargs=${switches} ${include}
largs=${switches} ${libs}
