SUBDIRS := $(shell find . -type f -name Makefile -not -path "./Makefile" -exec dirname {} \;)

.PHONY: all $(SUBDIRS) clean

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir clean; \
	done

