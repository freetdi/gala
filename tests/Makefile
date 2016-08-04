
CPPFLAGS=-I../.. -DTRACE_UNTESTED $(LOCAL_CPPFLAGS)
CXXFLAGS=-std=c++11 -O0 -g

LOGS=copy.log view.log copy2.log

all: $(LOGS)

$(LOGS): %.log: %
	./$< |& tee $@

$(LOGS): .P

.PHONY: .P
