lib:
	@echo "============= building common libs ==============="
	@make action=lib doit
	@echo "============= common libs complete ==============="

modules:
	@echo "============= building common modules ==============="
	@make doit
	@echo "============= completed common modules =============="

clean:
	@echo "============= cleaning common libs ==============="
	@make action=clean doit
	@rm -fv ./obj/*
	@echo "========== common lib cleanup complete ==========="

doit:
	@cd point; make ${action}
	@cd timer; make ${action}
	@cd work_queue_thread; make ${action}
	@cd singleton; make ${action}
	@cd plugin_loader; make ${action}
