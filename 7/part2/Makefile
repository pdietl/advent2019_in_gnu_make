ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

SHELL := /bin/bash

define NL


endef

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

from_x_downto_y = \
    $(eval priv_X := $1) \
    $(foreach x,$(wordlist 1,$(sub $1 $2 -1),$(FROM_n)), \
        $(priv_X)$(eval priv_X := $(sub $(priv_X) 1)))

NUMBERS := 0 1 2 3 4 5 6 7 8 9 

split_helper = \
    $(if $1, \
        $(call split_helper,$(wordlist 2,$(words $1),$1), \
            $(subst $(firstword $1),$(firstword $1) ,$2)), \
        $2)

# a number (in decimal)
split = $(strip $(call split_helper,$(NUMBERS),$1))

# numbers
permute = $(strip \
    $(if $(eq $(words $1),1), \
        $1, \
        $(foreach i,$(call from_x_upto_y,1,$(words $1)), \
            $(addprefix $(word $(i),$1), \
                $(call permute, \
                    $(wordlist 1,$(sub $(i) 1),$1) $(wordlist $(add 1 $(i)),$(words $1),$1) \
                    )  \
                ) \
            ) \
        ) \
    )

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

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
    $(eval load math.so), \
    $(eval math.so:))

# perms
runner = $(strip \
	$(shell echo $$'$(firstword $1)\n0' > out) \
	$(foreach n,$(call tail,$1), \
		$(eval v := $(shell $(MAKE) -f subMakefile < out | sed '/make.*/d')) \
		$(shell echo $$'$(n)\n$(v)' > out) \
		) \
	$(eval v := $(shell $(MAKE) -f subMakefile < out | sed '/make.*/d')) \
	$(v) \
)

main = $(strip \
	$(foreach n,$(call permute,0 1 2 3 4), \
		$(eval thrusts := $(thrusts) $(call runner,$(call split,$(n)))) \
		) \
	$(call find_largest,$(firstword $(thrusts)),$(call tail,$(thrusts))) \
)

NUMBER_AMPS := 5

# perm inputs
run_a_permutation = \
	mkfifo$(call from_x_upto_y,0,$(sub $(NUMBER_AMPS) 1)); \
		$(foreach n,$(call from_x_upto_y,0,$(sub $(NUMBER_AMPS) 1)), \
			$(MAKE) -f subMakefile < $(n) > $(mod $(add 1 $(n)),$(NUMBER_AMPS)) & \
			pids+=( $$! ); \
			echo $(word $(add 1 $(n)),$1) > $(n); \
			) \
		exec 3<0 && sleep 10000 & \
		keep_open_pid=$$!; \
		echo 0 > 0; \
		wait $${pids[@]}; \
		exec 4>0; \
		exec 3<0; \
		read -u 3; \
		echo $$REPLY >> all_thrusts; \
		rm $(call from_x_upto_y,0,$(sub $(NUMBER_AMPS) 1)); \
		kill $$keep_open_pid;
all:
	rm -f all_thrusts
	$(foreach n,$(call permute,5 6 7 8 9), \
		$(eval l := $(call split,$(n)))$(NL) \
		$(call run_a_permutation,$(l)) \
		)
	$(MAKE) --no-print -f $(ROOT_DIR)Makefile calc_max

calc_max:
	$(eval l := $(shell cat all_thrusts))
	$(info $(call find_largest,$(firstword $(l)),$(call tail,$(l))))

# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so

