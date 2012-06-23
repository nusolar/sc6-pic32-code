SUBDIRS = 	car_bms.X car_can_msg_relay.X car_driver_controls.X \
			t_error_reporting.X
.PHONY: subdirs ${SUBDIRS}

subdirs: ${SUBDIRS}

# recursion lock
ifneq (${make_lock}, 1)

${SUBDIRS}:
	@echo -e "\n"
	@echo -e "========================================================"
	@echo -e "|\tEntering $@"
	@echo -e "========================================================"
	@echo -e "\n"
	@echo -e "\t~~~~ CLOBBERING... ~~~~"
	@echo -e "\n"
	-${MAKE} -C $@ make_lock=1 clobber
	@echo -e "\n"
	@echo -e "\t~~~~ BUILDING... ~~~~"
	@echo -e "\n"
	-${MAKE} -C $@ make_lock=1 all
	@echo -e "\n"
	@echo -e "========================================================"
	@echo -e "|\tExiting $@"
	@echo -e "========================================================"
	@echo -e "\n"

# override rule for the directory we were called from
# so we don't build that directory twice
$(notdir ${lastdir}):
	

# $(addprefix tests/,$(notdir ${lastdir})):
	

else
${SUBDIRS}:
	

endif