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

lib: ../lib/sim_communications.a

../lib/sim_communications.a:
	@echo "============= building communication libs ==============="
#	@make action=lib doit
	@touch ../lib/sim_communications.a
	@echo "============= communication libs complete ==============="

modules:
	@echo "============= building communication modules ==============="
	@make doit
	@echo "============= completed communication modules =============="

clean:
	@echo "============= cleaning communication libs ==============="
#	@make action=clean doit
	@rm -fv ../lib/sim_communications.a
	@echo "========== communication lib cleanup complete ==========="

doit:

