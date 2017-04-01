CPPFLAGS=-I. -DTRACE_UNTESTED $(LOCAL_CPPFLAGS)
CXXFLAGS=-std=c++11 -O0 -g -Wall -Wextra $(LOCAL_CXXFLAGS)

all: check

TESTS = \
	bmd \
	cbs \
	cbsg \
	concepts \
	copy \
	copy2 \
	copy3 \
	copy4 \
	count \
	count_adj \
	dir \
	edg \
	erase1 \
	fbsg \
	idmap \
	iter \
	move \
	move_1 \
	pmap \
	rewire \
	sfinae \
	view \
	vvg

LOGS = ${TESTS:%=%.log}

CLEANFILES = ${TESTS} ${LOGS} gala

check: $(LOGS)

$(TESTS): | gala

SHELL = /bin/bash
$(LOGS): %.log: %
	./$< 2>&1 | tee $@; exit $${PIPESTATUS[0]}

gala:
	-ln -sf ../ gala

$(LOGS): .P

clean:
	rm -f $(CLEANFILES)

.PHONY: .P clean
