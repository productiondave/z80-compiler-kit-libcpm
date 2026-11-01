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

.PHONY: all clean $(CLEAN_DIRS) $(ALL_DIRS) $(COPY_DIRS)

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

