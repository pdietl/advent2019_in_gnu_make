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

get_anscestors_helper = $(strip \
	$(if $1, \
		$1 $(call get_anscestors_helper,$($1))))

# an orbit
get_anscestors = $(call get_anscestors_helper,$($1))

# orbit a, orbit b
get_common_anscestor = $(firstword $(filter $(call get_anscestors,$1),$(call get_anscestors,$2)))

# orbit, anscestor
get_distance_to_anscestor = $(strip \
	$(if $(filter $1,$2),-1, \
		$(add 1 $(call get_distance_to_anscestor,$($1),$2))))

COM := 

ORBIT_LIST := COM

main = \
	$(eval new_orbit := $(word $(add 1 $1),$2)) \
	$(eval $(new_orbit) := $(word $1,$2)) \
	$(eval ORBIT_LIST := $(ORBIT_LIST) $(new_orbit)) \
	$(if $(word $(add 2 $1),$2),$(call main,$(add 2 $1),$2))

all:
	$(call main,1,$(PROGRAM))
	$(info orbits: $(ORBIT_LIST))
	$(info anscestors of YOU: $(call get_anscestors,YOU))
	$(info anscestors of SAN: $(call get_anscestors,SAN))
	$(eval common := $(call get_common_anscestor,YOU,SAN))
	$(info common anscestor of YOU & SAN: $(common))
	$(eval dist_from_YOU := $(call get_distance_to_anscestor,YOU,$(common)))
	$(info distance to common from YOU: $(dist_from_YOU))
	$(eval dist_from_SAN := $(call get_distance_to_anscestor,SAN,$(common)))
	$(info distance to common from SAN: $(dist_from_SAN))
	$(info $(add $(dist_from_YOU) $(dist_from_SAN)))

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
