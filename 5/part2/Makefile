.PHONY: all

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

SHELL := /bin/bash

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

op_to_string = $(strip \
	$(if $(filter 01,$1),add) \
	$(if $(filter 02,$1),mul) \
	$(if $(filter 03,$1),in) \
	$(if $(filter 04,$1),out) \
	$(if $(filter 05,$1),jt) \
	$(if $(filter 06,$1),jf) \
	$(if $(filter 07,$1),lt) \
	$(if $(filter 08,$1),gt) \
	$(if $(filter 99,$1),halt))

# list, index
main = \
	$(info called main) \
	$(eval op_code := $(call get_op_code,$1,$2)) \
	$(eval param_modes := $(call get_param_modes,$1,$2)) \
	$(eval param_modes := $(call fill_back,0,$(param_modes),4)) \
	$(info index=$2 op_code=$(op_code) ($(call op_to_string,$(op_code)))) \
	$(if $(filter 01 02 07 08 99,$(op_code)), \
		$(if $(filter 01,$(op_code)), \
			$(eval result := $(add $(call get_argument,$2,$1,$(param_modes),1) \
				$(call get_argument,$2,$1,$(param_modes),2))) \
			) \
		$(if $(filter 02,$(op_code)), \
			$(eval result := $(mul $(call get_argument,$2,$1,$(param_modes),1) \
				$(call get_argument,$2,$1,$(param_modes),2))) \
			) \
		$(if $(filter 07,$(op_code)), \
			$(eval result := $(lt $(call get_argument,$2,$1,$(param_modes),1), \
				$(call get_argument,$2,$1,$(param_modes),2))) \
			$(if $(result),,$(eval result := 0)) \
			) \
		$(if $(filter 08,$(op_code)), \
			$(eval result := $(eq $(call get_argument,$2,$1,$(param_modes),1), \
				$(call get_argument,$2,$1,$(param_modes),2))) \
			$(if $(result),,$(eval result := 0)) \
			) \
		$(if $(filter 99,$(op_code)),$(error *** halting)$1, \
			$(info else of halt) \
			$(info storing value '$(result)' into index $(add 1 $(word $(add 3 $2),$1))) \
			$(eval new_list := $(wordlist 1,$(word $(add 3 $2),$1),$1) $(result) $(wordlist $(add 2 $(word $(add 3 $2),$1)),$(words $1),$1)) \
			$(call main,$(new_list),$(add 4 $2))) \
		) \
	$(info after halt) \
	$(if $(filter 03 04,$(op_code)), \
		$(info >>>>>>>>>>>>>>> in filter 03 04) \
		$(if $(filter 03,$(op_code)), \
			$(eval input := $(shell read -p 'input: ' -e; echo "$$REPLY")) \
			$(info storing value '$(input)' into index $(add 1 $(word $(add 3 $2),$1))) \
			$(eval new_list := $(wordlist 1,$(word $(add 1 $2),$1),$1) $(input) $(wordlist $(add 2 $(word $(add 1 $2),$1)),$(words $1),$1)) \
			) \
		$(if $(filter 04,$(op_code)), \
			$(info $(word $(add 1 $(word $(add 1 $2),$1)),$1))) \
		$(info >>> helo-[pppppp) \
		$(call main,$(new_list),$(add 2 $2)) \
		) \
	$(info >>>>>>>>>>>>>>> AFTER filter 03 04) \
	$(if $(filter 05 06,$(op_code)), \
		$(info >>>>>>>>>>>>>>> in filter 05 06) \
		$(eval true_or_false := $(call get_argument,$2,$1,$(param_modes),1)) \
		$(eval jump_location := $(call get_argument,$2,$1,$(param_modes),2)) \
		$(info true_or_false condition number := $(true_or_false)) \
		$(info jump_location:=$(jump_location)) \
		$(if $(filter 05,$(op_code)), \
			$(if $(ne $(true_or_false),0), \
				$(call main,$1,$(add 1 $(jump_location))))) \
		$(if $(filter 06,$(op_code)), \
			$(if $(eq $(true_or_false),0), \
				$(call main,$1,$(add 1 $(jump_location))))) \
		$(call main,$1,$(add 3 $2))) \
	$(info >>>>>>>>>>>>>>> AFTER filter 05 06) 

helper = $(if $(filter 99,$1),99,$1 $(call helper,$(add $1 1)))

0_to_99 = $(call helper,0)

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

all:
	@: $(call main,$(PROGRAM),1)

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
