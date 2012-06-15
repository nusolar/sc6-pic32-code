SUBDIRS = 	car_bms.X car_can_msg_relay.X car_driver_controls.X \
			t_error_reporting.X
.PHONY: subdirs ${SUBDIRS}

subdirs: ${SUBDIRS}

# recursion lock
ifneq (${make_lock}, 1)

${SUBDIRS}:
	-${MAKE} -j -C $@ make_lock=1

# override rule for the directory we were called from
# so we don't build that directory twice
$(notdir ${lastdir}):
	

# $(addprefix tests/,$(notdir ${lastdir})):
	

else
${SUBDIRS}:
	

endif