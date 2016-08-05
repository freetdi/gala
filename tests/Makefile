
CPPFLAGS=-I. -DTRACE_UNTESTED $(LOCAL_CPPFLAGS)
CXXFLAGS=-std=c++11 -O0 -g $(LOCAL_CXXFLAGS)

TESTS = \
	bmd \
	copy \
	copy2 \
	count \
	count_adj \
	dir \
	iter \
	view

LOGS = ${TESTS:%=%.log}

all: $(LOGS)

TESTS: gala


$(LOGS): %.log: %
	./$< | tee $@

gala:
	-ln -sf ../ gala

$(LOGS): .P

.PHONY: .P
