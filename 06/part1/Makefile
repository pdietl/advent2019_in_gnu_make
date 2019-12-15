.PHONY: all

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

SHELL := /bin/bash

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

, := ,
PAREN := )
SPACE := 
SPACE := $(SPACE) 

PROGRAM := $(subst $(PAREN),$(SPACE),$(strip $(shell cat input.txt)))

NUMBERS := 0 1 2 3 4 5 6 7 8 9
LETTERS := A B C D E F G H I J K L M N O P Q R S T U V Q X Y Z

split_helper = \
    $(if $1, \
        $(call split_helper,$(wordlist 2,$(words $1),$1), \
            $(subst $(firstword $1),$(firstword $1) ,$2)), \
        $2)

# a number (in decimal)
split = $(strip $(call split_helper,$(NUMBERS) $(LETTERS),$1))

map = $(foreach x,$2,$(call $1,$(x)))

# expects a list
tail = $(wordlist 2,$(words $1),$1)

reverse = $(strip \
	$(if $(eq $(words $1),0), \
		$1, \
		$(call reverse,$(call tail,$1)) $(firstword $1)))

# string, times to add a space
pad_with_spaces_helper = $(if $(filter 0,$2),$1, \
		$(call pad_with_spaces_helper,$1,$(sub $2 1)))

# string, max_len
pad_with_spaces = $(call pad_with_spaces_helper,$1,$(sub $2 $(words $(call split,$1))))

COM := 

ORBIT_LIST := COM

main = \
	$(eval new_orbit := $(word $(add 1 $1),$2)) \
	$(eval $(new_orbit) := $(word $1,$2)) \
	$(eval ORBIT_LIST := $(ORBIT_LIST) $(new_orbit)) \
	$(if $(word $(add 2 $1),$2),$(call main,$(add 2 $1),$2))

get_number_of_direct_orbits = $(if $($1),1,0)

get_number_of_indirect_orbits_helper = $(strip \
	$(if $($1), \
		$(add 1 $(call get_number_of_indirect_orbits_helper,$($1))), \
		0))

get_number_of_indirect_orbits = $(call get_number_of_indirect_orbits_helper,$($1))

# orbit name
show_direct_orbit = \
	$(if $($1), \
		$(info orbit $(call pad_with_spaces,$1,3) directly orbits $($1)), \
		$(info orbit $(call pad_with_spaces,$1,3) directly orbits nothing))

# an orbit
show_indirect_orbits_helper = $(strip \
	$(if $($1), \
		$($1) $(call show_indirect_orbits_helper,$($1))))

# an orbit
show_indirect_orbits = \
	$(info indirect orbits of $1 are: $(call show_indirect_orbits_helper,$1))

# orbit list
show_orbit_numbers = \
	$(if $1, \
		$(eval orbit := $(firstword $1)) \
		$(call show_direct_orbit,$(orbit)) \
		$(call show_indirect_orbits,$(orbit)) \
		$(call show_orbit_numbers,$(call tail,$1)) \
		)

# orbits
calc_orbit_numbers_helper = $(strip \
	$(if $1, \
		$(eval orbit := $(firstword $1)) \
		$(eval COUNT := $(add $(COUNT) \
							  $(call get_number_of_direct_orbits,$(orbit)) \
							  $(call get_number_of_indirect_orbits,$(orbit)))) \
		$(call calc_orbit_numbers_helper,$(call tail,$1)), \
		$(COUNT)))

calc_orbit_numbers = $(eval COUNT := 0)$(call calc_orbit_numbers_helper,$1)

all:
	$(call main,1,$(PROGRAM))
#	$(call show_orbit_numbers,$(ORBIT_LIST))
	$(info $(call calc_orbit_numbers,$(ORBIT_LIST)))

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
