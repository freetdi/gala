
CPPFLAGS=-I. -DTRACE_UNTESTED $(LOCAL_CPPFLAGS)
CXXFLAGS=-std=c++11 -O0 -g $(LOCAL_CXXFLAGS)

TESTS=copy view copy2 bmd dir
LOGS=${TESTS:%=%.log}

all: $(LOGS)

TESTS: gala


$(LOGS): %.log: %
	./$< |& tee $@

gala:
	-ln -sf ../ gala

$(LOGS): .P

.PHONY: .P
