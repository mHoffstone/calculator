.PHONY: clean All

All:
	@echo "----------Building project:[ calculator - Debug ]----------"
	@"$(MAKE)" -f  "calculator.mk"
clean:
	@echo "----------Cleaning project:[ calculator - Debug ]----------"
	@"$(MAKE)" -f  "calculator.mk" clean
