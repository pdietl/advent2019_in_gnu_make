.PHONY: all

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

SHELL := /bin/bash

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

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

# list, index
get_op_code = $(strip \
	$(eval priv_op = $(call split,$(word $2,$1))) \
	$(if $(filter $(words $(priv_op)),1), \
		0$(priv_op), \
		$(word $(sub $(words $(priv_op)) 1),$(priv_op))$(word $(words $(priv_op)),$(priv_op))))

#list, index
get_param_modes = $(strip \
	$(eval priv_param = $(call split,$(word $2,$1))) \
		$(if $(lt $(words $(priv_param)),3), \
		0, \
		$(call reverse,$(wordlist 1,$(sub $(words $(priv_param)) 2),$(priv_param)))))

#opcode index, instruction list, param_modes, argument number
get_argument = $(strip \
	$(if $(filter 0,$(word $4,$3)), \
		$(eval priv_res := $(word $(add 1 $(word $(add $1 $4),$2)),$2)) \
		$(priv_res) \
		, \
		$(eval priv_res := $(word $(add $1 $4),$2)) \
		$(priv_res) \
		))

# list, index
main = \
	$(eval op_code := $(call get_op_code,$1,$2)) \
	$(eval param_modes := $(call get_param_modes,$1,$2)) \
	$(if $(filter 01 02 99,$(op_code)), \
		$(eval param_modes := $(call fill_back,0,$(param_modes),3)) \
		$(if $(filter 01,$(op_code)), \
			$(eval result := $(add $(call get_argument,$2,$1,$(param_modes),1) \
				$(call get_argument,$2,$1,$(param_modes),2))) \
			) \
		$(if $(filter 02,$(op_code)), \
			$(eval result := $(mul $(call get_argument,$2,$1,$(param_modes),1) \
				$(call get_argument,$2,$1,$(param_modes),2))) \
			) \
		$(if $(filter 99,$(op_code)),$1, \
			$(eval new_list := $(wordlist 1,$(word $(add 3 $2),$1),$1) $(result) $(wordlist $(add 2 $(word $(add 3 $2),$1)),$(words $1),$1)) \
			$(call main,$(new_list),$(add 4 $2))) \
		) \
	$(if $(filter 03 04,$(op_code)), \
		$(eval param_modes := $(call fill_back,0,$(param_modes),2)) \
		$(if $(filter 03,$(op_code)), \
			$(eval input := $(shell read -e; echo "$$REPLY")) \
			$(eval new_list := $(wordlist 1,$(word $(add 1 $2),$1),$1) $(input) $(wordlist $(add 2 $(word $(add 1 $2),$1)),$(words $1),$1)) \
			) \
		$(if $(filter 04,$(op_code)), \
			$(info $(word $(add 1 $(word $(add 1 $2),$1)),$1))) \
		$(call main,$(new_list),$(add 2 $2)) \
		)

helper = $(if $(filter 99,$1),99,$1 $(call helper,$(add $1 1)))

0_to_99 := $(call helper,0)

all:
	@: $(call main,$(PROGRAM),1)

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
