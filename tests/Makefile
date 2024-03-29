CPPFLAGS = -I. -DTRACE_UNTESTED $(LOCAL_CPPFLAGS)
CXXFLAGS = -std=c++11 -O0 -g -Wall -Wextra $(LOCAL_CXXFLAGS)

all: check

TESTS = \
	bidir \
	bmd \
	cbs \
	cbsg.0 \
	cbsg.2 \
	cbsg_cp \
	concepts0 \
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
	flatset \
	idmap \
	iter \
	move \
	move_1 \
	pmap \
	rewire \
	sfinae \
	symm \
	symm2 \
	view \
	vvg

LOGS = ${TESTS:%=%.log}

CLEANFILES = ${TESTS} ${LOGS} gala ${TESTS:%=%.o}

check: $(LOGS)

dep: dep.mk
dep.mk: | $(TESTS:%=%.cc) gala
	$(CXX) -c -MM $(CXXFLAGS) ${CPPFLAGS} $(TESTS:%=%.cc) > $@

include dep.mk

$(TESTS:%=%.o): %.o: %.cc
	$(CXX) $(CXXFLAGS) ${CPPFLAGS} -c $< -o $@

$(TESTS): %: %.o
	$(CXX) $(LDFLAGS) ${LIBS} $< -o $@

$(TESTS): | gala

SHELL = /bin/bash
$(LOGS): %.log: %
	./$< 2>&1 | tee $@; exit $${PIPESTATUS[0]}

gala:
	[ -L ./gala ] || ln -sf ../ gala

$(LOGS): .P

clean:
	rm -f $(CLEANFILES)

.PHONY: .P clean
