DOCKER_IMAGE_VER := 1
DOCKER_IMAGE_TAG := pdietl/alpine_make:$(DOCKER_IMAGE_VER)

MD5 := $(shell which md5sum || which md5 || exit 1)

vol_mnt    = -v $(1):$(1)
vol_mnt_ro = $(call vol_mnt,$(1)):ro
map        = $(foreach f,$(2),$(call $(1),$(f)))
uniq       = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

DOCKER_ARGS = -t --rm -w $(CURDIR) $(call vol_mnt,$(CURDIR))
DOCKER_ARGS += $(call map,vol_mnt_ro,/etc/passwd /etc/group)
ifeq ($(ROOT),)
    DOCKER_ARGS += -u $(shell id -u):$(shell id -g)
    DOCKER_ARGS += $(call vol_mnt_ro,$(HOME)/.ssh)
else
    DOCKER_ARGS += -v $(HOME)/.ssh:/root/.ssh
endif
DOCKER_ARGS += $(DOCKER_IMAGE_TAG)

define NL


endef

.PHONY: $(addprefix docker-,build publish shell) test

all:
	$(foreach d,$(sort $(shell find . -name 'Makefile' | tail -n+2 | xargs -L 1 dirname)), \
		time $(MAKE) -C $(d)$(NL))

PARTS := 1 2

day-%:
	$(foreach p,$(PARTS),time $(MAKE) -C $*/part$(p)/$(NL))

docker-shell:
	docker run -ti $(DOCKER_ARGS) /bin/bash

docker-build:
	docker build . --tag $(DOCKER_IMAGE_TAG)

docker-publish:
	docker push $(DOCKER_IMAGE_TAG)

# Don't change `make` to `$(MAKE)` -- this will break MacOS support
docker-%: Dockerfile
	docker run -ti $(DOCKER_ARGS) $(notdir $(MAKE)) $* $(MAKEFLAGS)
