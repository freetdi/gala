
CPPFLAGS=-I. -DTRACE_UNTESTED $(LOCAL_CPPFLAGS)
CXXFLAGS=-std=c++11 -O0 -g -Wall -Wextra $(LOCAL_CXXFLAGS)

TESTS = \
	bmd \
	copy \
	copy2 \
	copy3 \
	count \
	count_adj \
	dir \
	iter \
	move \
	rewire \
	view

LOGS = ${TESTS:%=%.log}

CLEANFILES = ${TESTS} ${LOGS}

all: $(LOGS)

TESTS: gala


$(LOGS): %.log: %
	./$< | tee $@

gala:
	-ln -sf ../ gala

$(LOGS): .P

clean:
	rm -f $(CLEANFILES)

.PHONY: .P clean
