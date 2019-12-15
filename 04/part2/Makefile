.PHONY: all

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

SPACE := 
SPACE := $(SPACE) 

n := x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x
n := $(foreach e,$(n),$(n)) # 1024
n := $(foreach e,$(n),$(n)) # 1,048,576

PROGRAM := $(subst -,$(SPACE),$(shell cat input.txt))

map = $(foreach x,$2,$(call $1,$(x)))

NUMBERS := 0 1 2 3 4 5 6 7 8 9

split_helper = \
    $(if $1, \
        $(call split_helper,$(wordlist 2,$(words $1),$1), \
            $(subst $(firstword $1),$(firstword $1) ,$2)), \
        $2)

split = $(strip $(call split_helper,$(NUMBERS),$1))

# expects a list
tail = $(wordlist 2,$(words $1),$1)

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

#from #to (inclusive)
from_x_upto_y   = \
	$(eval priv_X := $1) \
	$(foreach x,$(wordlist 1,$(sub $2 $1 -1),$(n)), \
		$(priv_X)$(eval priv_X := $(add 1 $(priv_X))))

from_x_downto_y = \
	$(eval priv_X := $1) \
	$(foreach x,$(wordlist 1,$(sub $1 $2 -1),$(n)), \
		$(priv_X)$(eval priv_X := $(sub $(priv_X) 1)))

# current digit, rest of list
non_decreasing_digits_helper = $(strip \
	$(if $2, \
		$(if $(ge $(firstword $2),$1), \
			$(call non_decreasing_digits_helper,$(firstword $2),$(call tail,$2)),), \
		1) \
	)

# expects a list of numbers
non_decreasing_digits? = $(call non_decreasing_digits_helper,$(firstword $1),$(call tail,$1))

num_consec_digits_helper = $(strip \
	$(if $2, \
		$(if $(eq $1,$(firstword $2)), \
			$(eval num := $(add 1 $(num))) \
			$(call num_consec_digits_helper,$1,$(call tail,$2)), \
			$(num)),\
		$(num)) \
	)

# digit, list
num_consec_digits = $(eval num := 0)$(call num_consec_digits_helper,$1,$2)

# digit 1, digits 2, rest of list
htants_helper = $(strip \
	$(and $1,$2, \
		$(if $(eq $1,$2), \
			$(if $3, \
				$(if $(eq $2,$(firstword $3)), \
					$(eval new_list := $(wordlist $(add 1 $(call num_consec_digits,$2,$3)),$(words $3),$3)) \
					$(call htants_helper,$(firstword $(new_list)),$(word 2,$(new_list)),$(wordlist 3,$(words $(new_list)),$(new_list))),1), \
					1), \
				$(call htants_helper,$2,$(firstword $3),$(call tail,$3)))))

# expects a list of numbers
has_two_adjacent_numbers_the_same? = $(call htants_helper,$(firstword $1),$(word 2,$1),$(wordlist 3,$(words $1),$1))

compute = $(strip $(eval COUNT := 0) \
	$(foreach i,$(call from_x_upto_y,$(1),$(2)), \
		$(eval split_number := $(call split,$(i)))	\
		$(and $(call has_two_adjacent_numbers_the_same?,$(split_number)), \
			  $(call non_decreasing_digits?,$(split_number)), \
			  $(eval COUNT := $(add 1 $(COUNT))))) \
	$(COUNT))

all:
#	$(info >> '$(call has_two_adjacent_numbers_the_same?,1 4 4 4 5 6 7 7 7)')
	$(info $(call compute,$(firstword $(PROGRAM)),$(lastword $(PROGRAM))))
#	$(info $(call compute,222200,222222))

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
