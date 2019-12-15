.PHONY: all

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

SHELL := /bin/bash

DEBUG := 

define NL


endef

dprint = $(if $(DEBUG),$(info $1))

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

# list of integers
find_index_of_largest = $(strip \
	$(eval largest := $(firstword $1)) \
	$(eval index_of_largest := 1) \
	$(foreach i,$(call from_x_upto_y,2,$(words $1)), \
		$(if $(gt $(word $(i),$1),$(largest)), \
			$(eval largest := $(word $(i),$1)) \
			$(eval index_of_largest := $(i)) \
			) \
		) \
		$(index_of_largest) $(largest)\
	)

, := ,
SPACE := 
SPACE := $(SPACE) 

NUMBERS     := 0 1 2 3 4 5 6 7 8 9
OTHER_CHARS := . \#

split_helper = \
    $(if $1, \
        $(call split_helper,$(wordlist 2,$(words $1),$1), \
            $(subst $(firstword $1),$(firstword $1) ,$2)), \
        $2)

split = $(strip $(call split_helper,$(if $2,$2,$(NUMBERS)),$1))


PROGRAM := $(subst $(NL),$(SPACE),$(strip $(shell cat input.txt)))
PROGRAM := $(subst $(NL),$(SPACE),$(strip $(shell cat input.txt)))
WIDTH   := $(words $(call split,$(firstword $(PROGRAM)),$(OTHER_CHARS)))
HEIGHT  := $(words $(PROGRAM))
PROGRAM := $(call split,$(PROGRAM),$(OTHER_CHARS))

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

# list name, value
lappend = $(eval $1 := $($1) $2)

#list name, value
lappend_unique = $(if $(filter $2,$($1)),,$(call lappend,$1,$2))

POINTS := 

make_points = \
	$(foreach y,$(call from_x_upto_y,0,$(sub $(HEIGHT) 1)), \
		$(foreach x,$(call from_x_upto_y,0,$(sub $(WIDTH) 1)), \
			$(if $(filter #,$(word $(add $(mul $(y) $(WIDTH)) $(x) 1),$(PROGRAM))), \
				$(call lappend,POINTS,$(x).$(y)) \
				) \
			) \
		)

ATAN_LIST :=

main = $(strip \
	$(eval most_per_point := ) \
	$(foreach x,$(POINTS), \
		$(eval x_list := $(subst .,$(SPACE),$(x))) \
		$(eval ATANS := ) \
		$(foreach y,$(POINTS), \
			$(if $(filter $(x),$(y)),, \
				$(eval p_list := $(subst .,$(SPACE),$(y))) \
				$(eval p := $(atan2 $(sub $(firstword $(p_list)) $(firstword $(x_list))),$(sub $(word 2,$(p_list)) $(word 2,$(x_list))))) \
				$(call lappend_unique,ATANS,$(p)) \
				) \
			) \
			$(call lappend,most_per_point,$(words $(ATANS))) \
		) \
		$(eval temp := $(call find_index_of_largest,$(most_per_point))) \
		$(info asteroids detected := $(lastword $(temp))) \
		$(info point := $(subst .,$(,),$(word $(firstword $(temp)),$(POINTS)))) \
	)

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

all:
#	@: $(info prog is: $(PROGRAM) height=$(HEIGHT) width=$(WIDTH))
	@: $(make_points)
#	@: $(info POINTS := $(POINTS))
	@: $(main)

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
