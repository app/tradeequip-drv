# Portable trade equipment project
# Spec file for building rpm and rpms under ALT
# $Id: tradeequip-drv.spec,v 1.20 2006/04/07 08:13:43 leader Exp $
# $Name:  $
#

#%define name1 tradeequip-qt
%define tedir tradeequip
%define ShortName tradeequip-drv
%define _qtdir %(echo $QTDIR)

Name: tradeequip-drv
Version: 1.0.0
Release: alt0

Summary: Portable trade equipment Applications drivers

License: GPL
Group: Office
Url: http://www.leaderit.ru/
Vendor:	Project Portable Trade Equipment Applications Drivers
Source:		%name-%version.tar.bz2

Requires: qt3 >= 3.2
Requires: tradeequip


BuildRequires: XFree86-devel XFree86-libs fontconfig freetype2 
BuildRequires: gcc-c++ libstdc++-devel
BuildRequires: libqt3-devel doxygen tetex-latex tetex-dvips
BuildRequires: tradeequip tradeequip-devel

%description
Portable trade equipment Applications drivers

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================
%package -n tradeequip-drv-utils
Summary: Portable trade equipment Applications drivers utilites

Group: Office
Requires: tradeequip-drv

%description -n tradeequip-drv-utils
Portable trade equipment Applications drivers utilites

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================


#==============================================================
%package -n tradeequip-drv-devel
Summary: Portable trade equipment Applications

Group: Office
Requires: tradeequip-devel

%description -n tradeequip-drv-devel
Portable trade equipment Applications

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================
%package -n tradeequip-drv-doc
Summary: Portable trade equipment Applications drivers documentation

Group: Office
#Requires:	qt3 >= 3.2

%description -n tradeequip-drv-doc
Portable trade equipment Applications drivers documentation

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>


#==============================================================
# DRIVERS
#==============================================================
#%package -n tecashregister-shtrih-fr
#Summary: Portable trade equipment shtrih-fr driver
#Group: Office
#Requires: tradeequip
#%description -n tecashregister-shtrih-fr
#Portable trade equipment Applications
#%files -n tecashregister-shtrih-fr
#%_libdir/%tedir/*shtrih-fr*

#==============================================================

%prep
%setup

%build
#export QTDIR=/usr/lib/qt3
export PATH=${PATH}:$QTDIR/bin
%make
#%make tr

%install
#export QTDIR=/usr/lib/qt3
export PATH=${PATH}:$QTDIR/bin
#make doc && make install INSTALL_ROOT="$RPM_BUILD_ROOT" LIBDIR=%_libdir BINDIR=%_bindir DOCDIR=%_docdir/%name-%version INCLUDEDIR=%_includedir/%name
%__mkdir_p %buildroot/%_bindir
%__mkdir_p %buildroot/%_libdir/%tedir
%__mkdir_p %buildroot/%_sysconfdir/%name
%__mkdir_p %buildroot/%_datadir/%tedir/drv/translations
%__mkdir_p %buildroot/%_includedir/%tedir/drv
%__mkdir_p %buildroot/%_docdir/%name-%version/html
%__mkdir_p %buildroot/%_docdir/%name-%version/html-ru

cp -df %_builddir/%name-%version/bin/* %buildroot%_bindir
cp -df %_builddir/%name-%version/lib/lib* %buildroot%_libdir/%tedir
cp -f %_builddir/%name-%version/*/*/lib/*.h %buildroot%_includedir/%tedir/drv
cp -f %_builddir/%name-%version/translations/*.qm %buildroot%_datadir/%tedir/drv/translations
#cp -f %_builddir/%name-%version/src/libshell/*.h %buildroot%_includedir/%name
#cp -f %_builddir/%name-%version/doc/html/* %buildroot%_docdir/%name-%version/html
#cp -f %_builddir/%name-%version/doc/html-ru/* %buildroot%_docdir/%name-%version/html-ru
#cp -f %_builddir/%name-%version/doc/tradeequip* %buildroot%_docdir/%name-%version
cd %buildroot%_libdir/ && ln -s %tedir/* ./
%post

%postun
rm -f %_datadir/%tedir/drv/translations/*.qm
rm -f %_datadir/%name/templates/*.*

rmdir --ignore-fail-on-non-empty %_datadir/%tedir/drv/translations
#rmdir --ignore-fail-on-non-empty %_datadir/%tedir/drv/templates
rmdir --ignore-fail-on-non-empty %_sysconfdir/%name
rmdir --ignore-fail-on-non-empty %_datadir/%tedir/drv

%clean

%files
%_libdir/*
%_datadir/%tedir/drv/*

%files -n tradeequip-drv-utils
%_bindir/*

%files -n tradeequip-drv-doc
%_docdir/%name-%version/*

%files -n tradeequip-drv-devel
%_includedir/%tedir/drv/*.h

%changelog
* Sat Mar 19 2005 Valery Grazdankin <leader@altlinux.ru> 1.0.0-alt1
  + init

