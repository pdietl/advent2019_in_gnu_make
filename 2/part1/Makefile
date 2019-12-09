.PHONY: all

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

, := ,
SPACE := 
SPACE := $(SPACE) 

PROGRAM := $(subst $(,),$(SPACE),$(strip $(shell cat input.txt)))

map = $(foreach x,$2,$(call $1,$(x)))

# list, index
main = \
	$(if $(findstring 1,$(word $2,$1)), \
		$(eval result := $(add $(word $(add 1 $(word $(add 1 $2),$1)), $1) \
			$(word $(add 1 $(word $(add 2 $2),$1)), $1))) \
		) \
	$(if $(findstring 2,$(word $2,$1)), \
		$(eval result := $(mul $(word $(add 1 $(word $(add 1 $2),$1)), $1) \
			$(word $(add 1 $(word $(add 2 $2),$1)), $1))) \
		) \
	$(if $(findstring 99,$(word $2,$1)),$(info $1), \
		$(eval new_list := $(wordlist 1,$(word $(add 3 $2),$1),$1) $(result) $(wordlist $(add 2 $(word $(add 3 $2),$1)),$(words $1),$1)) \
		$(call main,$(new_list),$(add 4 $2)) \
		)

# If math.so doesn't exist, then don't load it,
# but rather, change the default target to math.so
$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

all:
	$(call main,$(PROGRAM),1)
# We recall this makefile if we are making math.so for the first time
# when we call ourselves, 'all' will then be the default target
math.so: math.c
	$(CC) -shared -fPIC -g3 -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
