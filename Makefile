include Makefile.settings
include MakeList

package = gala
includedir = ${prefix}/include
pkgincludedir = ${includedir}/${package}

INSTALL = install

all:

check:
	$(MAKE) -C tests

clean:
	rm -f *~
	$(MAKE) -C tests clean

install:
	-$(INSTALL) -d ${DESTDIR}/${includedir}
	-$(INSTALL) -d ${DESTDIR}/${pkgincludedir}
	$(INSTALL) $(HEADERS) ${DESTDIR}/${pkgincludedir}
	$(MAKE) -C examples pkgincludedir="${pkgincludedir}" INSTALL="${INSTALL}" install

uninstall:
	for i in $(HEADERS); do rm ${DESTDIR}/${pkgincludedir}/$$i; done
	$(MAKE) -C examples pkgincludedir="${pkgincludedir}" INSTALL="${INSTALL}" uninstall
	rmdir ${DESTDIR}/${pkgincludedir}

Makefile.settings:
	@echo run configure first && false
