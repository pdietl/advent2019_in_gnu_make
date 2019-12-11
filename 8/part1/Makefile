ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

SHELL := /bin/bash

define NL


endef

PROGRAM := $(shell cat input.txt)

, := ,
SPACE := 
SPACE := $(SPACE) 

FROM_n := x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x 
FROM_n := $(foreach e,$(FROM_n),$(FROM_n)) # 1024
FROM_n := $(foreach e,$(FROM_n),$(FROM_n)) # 1,048,576

#from #to (inclusive)
from_x_upto_y   = \
    $(eval priv_X := $1) \
    $(foreach x,$(wordlist 1,$(sub $2 $1 -1),$(FROM_n)), \
        $(priv_X)$(eval priv_X := $(add 1 $(priv_X))))

NUMBERS := 0 1 2 3 4 5 6 7 8 9 

split_helper = \
    $(if $1, \
        $(call split_helper,$(wordlist 2,$(words $1),$1), \
            $(subst $(firstword $1),$(firstword $1) ,$2)), \
        $2)

# a number (in decimal)
split = $(strip $(call split_helper,$(NUMBERS),$1))

combine_helper = $(strip \
    $(if $1, \
        $(call combine_helper,$(wordlist 2,$(words $1),$1),$(subst $(firstword $1) ,$(firstword $1),$2)), \
        $2) \
)

combine = $(strip $(call combine_helper,$(NUMBERS),$1))

# expects a list
tail = $(wordlist 2,$(words $1),$1)

find_largest = $(strip \
	$(if $2, \
		$(if $(gt $(firstword $2),$1), \
			$(call find_largest,$(firstword $2),$(wordlist 2,$(words $2),$2)), \
			$(call find_largest,$1,$(wordlist 2,$(words $2),$2))) \
		, \
		$1) \
)

find_smallest = $(strip \
	$(if $2, \
		$(if $(lt $(firstword $2),$1), \
			$(call find_smallest,$(firstword $2),$(wordlist 2,$(words $2),$2)), \
			$(call find_smallest,$1,$(wordlist 2,$(words $2),$2))) \
		, \
		$1) \
)

# number list, size of layer
make_layers = $(strip \
	$(if $1, \
		$(call combine,$(wordlist 1,$2,$1)) \
		$(call make_layers,$(wordlist $(add 1 $2),$(words $1),$1),$2) \
		) \
)

# numbers, width, height
main = $(strip \
	$(eval layers_list := $(call make_layers,$(call split,$1),$(mul $2 $3))) \
	$(info layers_list := '$(layers_list)') \
	$(eval num_zeros_list := $(strip $(foreach l,$(layers_list), \
		$(words $(filter 0,$(call split,$(l)))) \
		))) \
	$(info num zeros per layer := $(num_zeros_list)) \
	$(eval smallest_num_zeros := $(call find_smallest,$(firstword $(num_zeros_list)),$(call tail,$(num_zeros_list)))) \
	$(info smallest number of zeros in a single layer: $(smallest_num_zeros)) \
	$(eval COUNT := 1) \
	$(eval layer_with_fewest_zeros := $(strip $(foreach n,$(num_zeros_list), \
		$(if $(filter $(n),$(smallest_num_zeros)),$(COUNT),$(eval COUNT := $(add 1 $(COUNT)))) \
		))) \
	$(info layer with fewest zeros := $(layer_with_fewest_zeros)) \
	$(eval num_ones := $(words $(filter 1,$(call split,$(word $(layer_with_fewest_zeros),$(layers_list)))))) \
	$(eval num_twos := $(words $(filter 2,$(call split,$(word $(layer_with_fewest_zeros),$(layers_list)))))) \
	$(info num_ones := $(num_ones)) \
	$(info num_twos := $(num_twos)) \
	$(eval product := $(mul $(num_ones) $(num_twos))) \
	$(info product := $(product)) \
)


# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
    $(eval load math.so), \
    $(eval math.so:))

all:
	@: $(call main,$(PROGRAM),25,6)

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
