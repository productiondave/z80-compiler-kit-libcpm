TOP=..
include $(TOP)/Make.default
-include $(TOP)/Make.local

# Examples built for every ARCH
SUBDIRS_COMMON = \
					fileio \
					malloc \
					demo \
					tms \
					args \
					joytest \
					exit \
					ay38910 \
					sprites \
					sptest \
					cpmdcout \
					highres \
					snakemc \
					tmstxt

# Examples only built when ARCH matches.  Add new gated examples here.
SUBDIRS_NABU = tcp
SUBDIRS_RETRO =
SUBDIRS_NOUVEAU =

SUBDIRS = $(SUBDIRS_COMMON) $(SUBDIRS_$(ARCH))

CLEAN_DIRS=$(SUBDIRS:%=clean-%)
ALL_DIRS=$(SUBDIRS:%=all-%)
COPY_DIRS=$(SUBDIRS:%=copy-%)

.PHONY: all clean copy-nabu $(CLEAN_DIRS) $(ALL_DIRS) $(COPY_DIRS)

all:: $(ALL_DIRS)

clean:: $(CLEAN_DIRS)

copy:: $(COPY_DIRS)

world:: clean all

$(ALL_DIRS):
	$(MAKE) -C $(@:all-%=%) all

$(CLEAN_DIRS):
	$(MAKE) -C $(@:clean-%=%) clean

$(COPY_DIRS):
	$(MAKE) -C $(@:copy-%=%) copy


# Copy built NABU examples (uppercased names) to the NABU Internet Adapter
# folder given by NABU_IA_DIR (see Make.default / Make.local).
copy-nabu: all
ifneq ($(ARCH),NABU)
	$(error copy-nabu requires ARCH=NABU)
endif
	@mkdir -p "$(NABU_IA_DIR)"
	@find "$(TOP)/build/arch/NABU/examples" -type f -name '*.com' | while read -r f; do \
		dest="$(NABU_IA_DIR)/$$(basename "$$f" | tr '[:lower:]' '[:upper:]')"; \
		cp "$$f" "$$dest" && echo "Copied: $$f -> $$dest"; \
	done
