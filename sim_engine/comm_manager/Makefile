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

lib: ./lib/nrtb_communication.a

./lib/nrtb_communication.a:
	@echo "============= building communication libs ==============="
	@make action=lib doit
	@ar -r ./../lib/nrtb_communication.a ./../obj/*.o
	@echo "============= communication libs complete ==============="

modules:
	@echo "============= building communication modules ==============="
	@make doit
	@echo "============= completed communication modules =============="

clean:
	@echo "============= cleaning communication libs ==============="
	@make action=clean doit
	@rm -fv ./../obj/* ./../lib/* ./../include/*
	@echo "========== communication lib cleanup complete ==========="

doit:
	@cd triplet; make ${action}
	@cd location_data; make ${action}
	@cd velocity_data; make ${action}
	@cd obj_setup_info; make ${action}
	@cd sim_info; make ${action}
	@cd sim_termination; make ${action}
