.PHONY: all

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

SHELL := /bin/bash

FROM_TO_N := x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x 
FROM_TO_N := $(foreach e,$(FROM_TO_N),$(FROM_TO_N)) # 1024
FROM_TO_N := $(foreach e,$(FROM_TO_N),$(FROM_TO_N)) # 1,048,576

#from, to (inclusive), optional step
from_x_upto_y   = \
    $(eval priv_X := $1) \
    $(foreach x,$(wordlist 1,$(sub $2 $1 -1),$(FROM_TO_N)), \
        $(priv_X)$(eval priv_X := $(add $(if $3,$3,1) $(priv_X))))

from_x_downto_y = \
    $(eval priv_X := $1) \
    $(foreach x,$(wordlist 1,$(sub $1 $2 -1),$(FROM_TO_N)), \
        $(priv_X)$(eval priv_X := $(sub $(priv_X) 1))

, := ,
SPACE := 
SPACE := $(SPACE) 

PROGRAM := $(subst $(,),$(SPACE),$(strip $(shell cat input.txt)))

NUMBERS := 0 1 2 3 4 5 6 7 8 9 

split_helper = \
    $(if $1, \
        $(call split_helper,$(wordlist 2,$(words $1),$1), \
            $(subst $(firstword $1),$(firstword $1) ,$2)), \
        $2)

# a number (in decimal)
split = $(strip $(call split_helper,$(NUMBERS),$1))

map = $(foreach x,$2,$(call $1,$(x)))

# expects a list
tail = $(wordlist 2,$(words $1),$1)

# string, times to add a char, pad_char
pad_helper = $(if $(le $2,0),$1,$3$(call pad_helper,$1,$(sub $2 1),$3))

# string, pad_char, max_len
pad = $(call pad_helper,$1,$(sub $3 $(words $(call split,$1))),$2)

# fill value, list, total_length
fill_front = $(strip \
	$(if $(ge $(words $2),$3), \
		$2, \
		$(call fill_front,$1,$1 $2,$3)))

# fill value, list, total_length
fill_back = $(strip \
	$(if $(ge $(words $2),$3), \
		$2, \
		$(call fill_back,$1,$2 $1,$3)))

reverse = $(strip \
	$(if $(eq $(words $1),0), \
		$1, \
		$(call reverse,$(call tail,$1)) $(firstword $1)))

# index
get_op_code = $(strip \
	$(eval priv_op = $(call split,$(call get_mem_loc,$1))) \
	$(if $(filter $(words $(priv_op)),1), \
		0$(priv_op), \
		$(word $(sub $(words $(priv_op)) 1),$(priv_op))$(word $(words $(priv_op)),$(priv_op))))

# index
get_param_modes = $(strip \
	$(eval priv_param = $(call split,$(call get_mem_loc,$1))) \
		$(if $(lt $(words $(priv_param)),3), \
		0, \
		$(call reverse,$(wordlist 1,$(sub $(words $(priv_param)) 2),$(priv_param)))))

# opcode index, param_modes, argument number
get_argument = $(strip \
	$(if $(filter 0,$(word $3,$2)), \
		$(eval priv_res := $(call get_mem_loc,$(call get_mem_loc,$(add $1 $3)))) \
		$(priv_res) \
		, \
		$(if $(filter 1,$(word $3,$2)), \
			$(eval priv_res := $(call get_mem_loc,$(add $1 $3))) \
			$(priv_res) \
			, \
			$(eval priv_res := $(call get_mem_loc,$(add $(call get_mem_loc,$(add $1 $3)) $(REL_OFFSET)))) \
			$(priv_res) \
			)))

op_to_string = $(strip \
	$(if $(filter 01,$1),add) \
	$(if $(filter 02,$1),mul) \
	$(if $(filter 03,$1),in) \
	$(if $(filter 04,$1),out) \
	$(if $(filter 05,$1),jt) \
	$(if $(filter 06,$1),jf) \
	$(if $(filter 07,$1),lt) \
	$(if $(filter 08,$1),gt) \
	$(if $(filter 09,$1),ar) \
	$(if $(filter 99,$1),hlt))

get_input = \
	$(eval input := $(shell read; echo "$$REPLY")) \
	$(if $(findstring make,$(input)),$(call get_input),$(input))

REL_OFFSET := 0

# index
main = \
	$(info main's index: $1) \
	$(eval op_code := $(call get_op_code,$1)) \
	$(eval param_modes := $(call get_param_modes,$1)) \
	$(eval param_modes := $(call fill_back,0,$(param_modes),4)) \
	$(info $(SPACE) opcode=$(op_code) param_modes=$(param_modes) ($(call op_to_string,$(op_code)))) \
	$(if $(filter 01 02 07 08 99,$(op_code)), \
		$(if $(filter 01,$(op_code)), \
			$(eval result := $(add $(call get_argument,$1,$(param_modes),1) \
				$(call get_argument,$1,$(param_modes),2))) \
			) \
		$(if $(filter 02,$(op_code)), \
			$(eval result := $(mul $(call get_argument,$1,$(param_modes),1) \
				$(call get_argument,$1,$(param_modes),2))) \
			) \
		$(if $(filter 07,$(op_code)), \
			$(eval result := $(lt $(call get_argument,$1,$(param_modes),1), \
				$(call get_argument,$1,$(param_modes),2))) \
			$(if $(result),,$(eval result := 0)) \
			) \
		$(if $(filter 08,$(op_code)), \
			$(eval result := $(eq $(call get_argument,$1,$(param_modes),1), \
				$(call get_argument,$1,$(param_modes),2))) \
			$(if $(result),,$(eval result := 0)) \
			) \
		$(if $(filter 99,$(op_code)),$1, \
			$(call set_mem_loc,$(call get_mem_loc,$(add $1 3)),$(result)) \
			$(call main,$(add 4 $1))) \
		, \
		$(if $(filter 03 04,$(op_code)), \
			$(if $(filter 03,$(op_code)), \
				$(eval input := $(call get_input)) \
				$(call set_mem_loc,$(call get_argument,$1,$(param_modes),1)) \
				) \
			$(if $(filter 04,$(op_code)), \
				$(eval out := $(call get_argument,$1,$(param_modes),1)) \
				$(info out is: '$(out)')) \
			$(call main,$(add 2 $1)) \
			,  \
			$(if $(filter 05 06,$(op_code)), \
				$(eval true_or_false := $(call get_argument,$1,$(param_modes),1)) \
				$(eval jump_location := $(call get_argument,$1,$(param_modes),2)) \
				$(if $(filter 05,$(op_code)), \
					$(if $(ne $(true_or_false),0), \
						$(call main,$(jump_location)), \
						$(call main,$(add 3 $1))),  \
				$(if $(filter 06,$(op_code)), \
					$(if $(eq $(true_or_false),0), \
						$(call main,$(jump_location)), \
						$(call main,$(add 3 $1))))) \
				, \
				$(if $(filter 09,$(op_code)), \
					$(eval rel_change := $(call get_argument,$1,$(param_modes),1)) \
					$(info rel_change := $(rel_change)) \
					$(eval REL_OFFSET := $(add $(REL_OFFSET) $(rel_change))) \
					$(info new rel offset := $(REL_OFFSET)) \
					$(call main,$(add 2 $1)) \
					))))

# list name, value
lappend = $(eval $1 := $($1) $2)

SET_MEM_LOCS := 

# ZERO-BASED!
# location, value
set_mem_loc = \
	$(eval loc := $(call pad,$1,0,16)) \
	$(eval __ML_$(loc) := $2) \
	$(if $(filter __ML_$(loc),$(SET_MEM_LOCS)),,$(call lappend,SET_MEM_LOCS,__ML_$(loc)))

# ZERO-BASED mem location
get_mem_loc = $(if $(filter __ML_$(call pad,$1,0,16),$(SET_MEM_LOCS)),$(__ML_$(call pad,$1,0,16)),0)

# list of initial state
initialize_memory = \
	$(foreach n,$(call from_x_upto_y,0,$(sub $(words $1) 1)), \
		$(call set_mem_loc,$(n),$(word $(add 1 $(n)),$1)) \
		)

view_memory = \
	$(eval mem_locs := $(sort $(SET_MEM_LOCS))) \
	$(foreach l,$(mem_locs), \
		$(info [$l] = $($l)))

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

all:
	@: $(call initialize_memory,$(PROGRAM))
#	@: $(call view_memory)
	@: $(call main,0)
#	@: $(call view_memory)

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
