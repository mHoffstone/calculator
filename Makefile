.PHONY: clean All

All:
	@echo "----------Building project:[ nogui - Debug ]----------"
	@"$(MAKE)" -f  "nogui.mk"
clean:
	@echo "----------Cleaning project:[ nogui - Debug ]----------"
	@"$(MAKE)" -f  "nogui.mk" clean
