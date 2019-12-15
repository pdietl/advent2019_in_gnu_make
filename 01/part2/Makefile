.PHONY: all

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

MASSES := $(shell cat input.txt)

calc_fuel = $(sub $(div $1 3) 2)

map = $(foreach x,$2,$(call $1,$(x)))

cal_then_helper = $(call helper, $(call calc_fuel, $1))

helper = $(add $1 $(call recur, $1))

recur = $(if $(le $(call calc_fuel, $1), 0), \
				0, \
				$(add $(call calc_fuel, $1) $(call recur, $(call calc_fuel, $1))))

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

all:
	$(info $(add $(call map,cal_then_helper,$(MASSES))))

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
