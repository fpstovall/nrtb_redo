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

all:
	@echo "============= building all targets ==============="
	@make action=lib doit
	#temp target
	touch ./bin/nrtb_simulation
	@echo temp target created.
	@echo "============= all targets complete ==============="

modules:
	@echo "============= building all targets ==============="
	@make doit
	@echo "============= completed all targets =============="

clean:
	@echo "============= cleaning all targets ==============="
	@make action=clean doit
	@rm -fv ./bin/*
	@echo "========== all targets cleanup complete ==========="

doit:
	cd common; make ${action}
	cd sim_engine; make ${action}

