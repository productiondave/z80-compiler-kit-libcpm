SUBDIRS = \
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
					snakemc

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

