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

lib: ./lib/nrtb_common.a 

./lib/nrtb_common.a:
	@echo "============= building common libs ==============="
	@mkdir -p ./lib ./obj ./include
	@make action=lib doit
	@ar -r ./lib/nrtb_common.a ./obj/*.o
	@echo "============= common libs complete ==============="

modules:
	@echo "============= building common modules ==============="
	@make doit
	@echo "============= completed common modules =============="

clean:
	@echo "============= cleaning common libs ==============="
	@make action=clean doit
	@rm -rf ./obj ./lib ./include
	@echo "========== common lib cleanup complete ==========="

doit:
	@cd common_rl; make ${action}
	@cd point; make ${action}
	@cd timer; make ${action}
	@cd abs_queue; make ${action}
	@cd linear_queue; make ${action}
	@cd sockets; make ${action}
	@cd serializer; make ${action}
	@cd singleton; make ${action}
	@cd logger; make ${action}
	@cd confreader; make ${action}
	@cd ipc_channel; make ${action}

# --- the following are obsolete and may be removed later.
#	@cd threads; make ${action}
#	@cd GPB; make ${action}
#	@cd transceiver; make ${action}
	
