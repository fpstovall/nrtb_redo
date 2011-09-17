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

lib: ../lib/nrtb_physics.a

../lib/nrtb_physics.a:
	@cd ../../common; make 
	@echo "============= building physics ==============="
	@make action=lib doit
	@ar -r ../lib/nrtb_physics.a ./obj/*.o
	@cp -v ./include/* ../include
	@echo "============= physics complete ==============="

modules:
	@echo "============= building physics modules ==============="
	@make doit
	@echo "============= completed physics modules =============="

clean:
	@echo "============= cleaning physics ==============="
	@rm -fv ../lib/nrtb_physics.a
	@cd include; for file in *; do rm -fv ../include/$$file; done
	@make action=clean doit
	@rm -fv ./obj/* ./lib/* ./include/*
	@echo "========== physics cleanup complete ==========="

doit:
	@cd abstract_effector; make ${action}
	@cd point_mass; make ${action}
