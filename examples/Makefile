
examplesdir=${pkgincludedir}/examples

install:
	-$(INSTALL) -d ${DESTDIR}/${examplesdir}
	$(INSTALL) $(EXAMPLES) ${DESTDIR}/${examplesdir}

uninstall:
	for i in $(EXAMPLES); do rm ${DESTDIR}/${examplesdir}/$$i; done
	rmdir ${DESTDIR}/${examplesdir}

EXAMPLES=ssg32i.h ssg16i.h ssg16ia.h svbs.h svbs_random.h alloc.hpp
