.PHONY: all

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

, := ,
SPACE := 
SPACE := $(SPACE) 

n := x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x
n := $(foreach e,$(n),$(n)) # 1024
n := $(foreach e,$(n),$(n)) # 1,048,576

PROGRAM1 := $(subst $(,),$(SPACE),$(strip $(shell sed -n '1,1p;42q' input.txt)))
PROGRAM2 := $(subst $(,),$(SPACE),$(strip $(shell sed -n '2,2p;42q' input.txt)))

map = $(foreach x,$2,$(call $1,$(x)))

letters := U D L R

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

split_helper = \
	$(if $1, \
		$(call split_helper,$(wordlist 2,$(words $1),$1), \
			$(subst $(firstword $1),$(firstword $1) ,$2)), \
		$2)

split = $(strip $(call split_helper,$(letters),$1))

#from #to (inclusive)
from_x_upto_y   = \
	$(eval priv_X := $1) \
	$(foreach x,$(wordlist 1,$(sub $2 $1 -1),$(n)), \
		$(priv_X)$(eval priv_X := $(add 1 $(priv_X))))

from_x_downto_y = \
	$(eval priv_X := $1) \
	$(foreach x,$(wordlist 1,$(sub $1 $2 -1),$(n)), \
		$(priv_X)$(eval priv_X := $(sub $(priv_X) 1)))

list = $(eval X := 0)$(eval Y := 0)$(eval PAIRS := $(X):$(Y)) \
	$(foreach x,$1, \
		$(eval direction := $(firstword $(call split,$(x)))) \
		$(eval scalar := $(lastword $(call split,$(x)))) \
		$(if $(filter R,$(direction)), \
			$(foreach i,$(call from_x_upto_y,$(add 1 $(X)),$(add $(X) $(scalar))), \
				$(eval PAIRS := $(PAIRS) $(i):$(Y))) \
				$(eval X := $(add $(X) $(scalar)))) \
		$(if $(filter U,$(direction)), \
			$(foreach i,$(call from_x_upto_y,$(add 1 $(Y)),$(add $(Y) $(scalar))), \
				$(eval PAIRS := $(PAIRS) $(X):$(i))) \
				$(eval Y := $(add $(Y) $(scalar)))) \
		$(if $(filter D,$(direction)), \
			$(foreach i,$(call from_x_downto_y,$(sub $(Y) 1),$(sub $(Y) $(scalar))), \
				$(eval PAIRS := $(PAIRS) $(X):$(i))) \
				$(eval Y := $(sub $(Y) $(scalar)))) \
		$(if $(filter L,$(direction)), \
			$(foreach i,$(call from_x_downto_y,$(sub $(X) 1),$(sub $(X) $(scalar))), \
				$(eval PAIRS := $(PAIRS) $(i):$(Y))) \
				$(eval X := $(sub $(X) $(scalar)))))
 
find_smallest = $(strip \
					$(if $2, \
						$(if $(lt $(firstword $2),$1), \
							$(call find_smallest,$(firstword $2),$(wordlist 2,$(words $2),$2)), \
							$(call find_smallest,$1,$(wordlist 2,$(words $2),$2))) \
						,$1) \
				)

#list $point
find_length_to_point = $(strip \
	$(eval DIST := 0) \
	$(foreach i,$(call from_x_upto_y,1,$(words $1)), \
		$(if $(filter $(word $(i),$1),$2), \
			$(DIST), \
			$(eval DIST := $(add 1 $(DIST))))))


compute = $(strip \
	$(foreach i,$1, \
		$(info [$(shell date +%T)] computing for point $(i).) \
		$(add $(call find_length_to_point,$(PAIRS1),$(i)) \
			  $(call find_length_to_point,$(PAIRS2),$(i))) \
		))

all:
	$(info WARNING: This might take up to 10 hours to complete lol)
	$(if $(lt $(shell ulimit -s),16192),$(info WARNING: You should probably increase your stack size to 16KiB))
	$(info [$(shell date +%T)] Start creating points.)
	$(call list,$(PROGRAM1))
	$(eval PAIRS1 := $(PAIRS))
	$(call list,$(PROGRAM2))
	$(eval PAIRS2 := $(PAIRS))
	$(info [$(shell date +%T)] Done.)
	$(info [$(shell date +%T)] Filter.)
	$(eval intersections := $(filter-out 0:0,$(filter $(PAIRS1),$(PAIRS2))))
	$(info intersections := $(intersections))
	$(info number of intersections: $(words $(intersections)))
	$(info [$(shell date +%T)] Done.)
	$(info [$(shell date +%T)] Start computing distances.)
	$(eval distances := $(call compute,$(intersections)))
	$(info [$(shell date +%T)] Done.)
	$(info distances := $(distances))
	$(info $(call find_smallest,$(firstword $(distances)),$(wordlist 2,$(words $(distances)),$(distances))))


# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
