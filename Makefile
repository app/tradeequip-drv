NAME   =	tradeequip-drv
NAMEV  =	$(NAME)-1.0.0
MAKE = make
QMAKE = qmake
MAKEFILE = Makefile
LIBDIR = $(INSTALL_ROOT)/usr/lib
BINDIR = $(INSTALL_ROOT)/usr/bin
DOCDIR = $(INSTALL_ROOT)/usr/share/doc/$(NAMEV)
INCLUDEDIR = $(INSTALL_ROOT)/usr/include/tradeequip
# project subdirectories
PROJECTDIRS = cashregisters readers scales displays

#BINDIR =	/usr/bin
#MANDIR =	/usr/man/man8
#VARDIR =	/usr/share/$(NAME)
TAR    =  	$(HOME)/tmp/$(NAMEV)
TARGET =	i586


first: all 

all: tr
	cd cashregisters && $(QMAKE) && $(MAKE)
	cd readers && $(QMAKE) && $(MAKE)
	cd scales && $(QMAKE) && $(MAKE) 
	cd displays && $(QMAKE) && $(MAKE)
clean:
	cd cashregisters && $(QMAKE) && $(MAKE) clean
	cd readers && $(QMAKE) && $(MAKE) clean
	cd scales && $(QMAKE) && $(MAKE) clean
	cd displays && $(QMAKE) && $(MAKE) clean
	rm -fr lib
	rm -fr rpm
	rm -fr bin
	rm -f translations/*.qm

trupdate:
#	lupdate src/lib/*.pro
#	lupdate `find . -name *.pro`
	lupdate -verbose `cat list.ts`

tr:
	lrelease translations/*.ts
doc: dummy
	cd doc && make
	
install: all
	find src doc -name Makefile -delete
	$(QMAKE) src/src.pro -o src/$(MAKEFILE)
	cd src && $(MAKE) install LIBDIR=$(LIBDIR) BINDIR=$(BINDIR) INCLUDEDIR=$(INCLUDEDIR) && $(MAKE) install DOCDIR=$(DOCDIR)
	
uninstall:
	cd src && $(MAKE) uninstall

tgz:	dummy
	mkdir -p $(TAR)
	cp -r -f * $(TAR)
	cd $(TAR) 
	cd $(HOME)/tmp && tar cf - "$(NAMEV)/" | bzip2 -f9 > "$(HOME)/RPM/SOURCES/$(NAMEV).tar.bz2"	
	rm -f -r $(TAR)

srpm:	tgz
	mkdir -p rpm
	rpmbuild --target=$(TARGET) -bs $(NAME).spec
	mv -f $(HOME)/RPM/SRPMS/$(NAME)* rpm

rpm:	srpm
	rpmbuild --target=$(TARGET) -bb $(NAME).spec
	mv -f $(HOME)/RPM/RPMS/$(TARGET)/$(NAME)* rpm

dummy:
