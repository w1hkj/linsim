# Copyright (c) 2013 Dave Freese, W1HKJ (w1hkj AT w1hkj DOT com)

bin_PROGRAMS = linsim

linsim_cxxFLAGS = @linsim_BUILD_cxxFLAGS@
linsim_CXXFLAGS = @linsim_BUILD_CXXFLAGS@
linsim_CFLAGS = $(linsim_CXXFLAGS)
linsim_LDFLAGS = @linsim_BUILD_LDFLAGS@
linsim_LDADD = @linsim_BUILD_LDADD@

MINGW32_SRC = \
	linsim.rc \
	include/linsimrc.h

# We distribute these but do not always compile them
EXTRA_linsim_SOURCES = $(linsim_WIN32_RES_SRC) $(MINGW32_SRC)

linsim_SOURCES =

if MINGW32
if HAVE_WINDRES
  linsim_SOURCES += $(MINGW32_SRC)
endif
endif

########################################################################


linsim_VERSION_MAJOR = @linsim_VERSION_MAJOR@
linsim_VERSION_MINOR = @linsim_VERSION_MINOR@
linsim_VERSION_PATCH = @linsim_VERSION_PATCH@
linsim_VERSION = @linsim_VERSION@

.EXPORT_ALL_VARIABLES: nsisinst appbundle

# Sources that are generated,
BUILT_SOURCES =
# not distributed,
nodist_linsim_SOURCES = $(BUILT_SOURCES)
# and deleted by the clean targets
CLEANFILES = $(BUILT_SOURCES)
CLEAN_LOCAL =

if MINGW32
if HAVE_WINDRES
.rc.o:
	$(WINDRES) -I$(srcdir)/include -I$(srcdir)/../data/win32 $< -O coff $@
endif
endif

install-data-local:
	if test -f $(srcdir)/../data/linsim.xpm; then \
	    $(mkinstalldirs) $(DESTDIR)/$(datadir)/pixmaps; \
	    $(INSTALL_DATA) $(srcdir)/../data/linsim.xpm $(DESTDIR)/$(datadir)/pixmaps; \
	fi
	if test -f $(srcdir)/../data/linsim.desktop; then \
	    $(mkinstalldirs) $(DESTDIR)/$(datadir)/applications; \
	    $(INSTALL_DATA) $(srcdir)/../data/linsim.desktop $(DESTDIR)/$(datadir)/applications; \
	fi

uninstall-local:
	rm -f $(DESTDIR)/$(datadir)/pixmaps/linsim.xpm
	rm -f $(DESTDIR)/$(datadir)/applications/linsim.desktop

FL_SRC = linsim_ui.fl

if HAVE_FLUID
flgen: $(FL_SRC)
	(cd $(srcdir)/include; \
	for f in $(FL_SRC); do \
	    c=$${f%.fl}.cxx; \
		h=$${f%.fl}.h; \
		h=$${h##*/}; \
	    $(FLUID) -c -o ../$$c -h $$h ../$$f; \
	done)
endif


INSTALLER_FILE = linsim-$(linsim_VERSION)_setup.exe
APPBUNDLE=linsim-$(linsim_VERSION)
APPBUNDLE_NOLIBS=$(APPBUNDLE)-nolibs

if HAVE_NSIS
nsisinst: $(bin_PROGRAMS)
	sh $(srcdir)/../scripts/mknsisinst.sh "$(srcdir)/../data" .
    CLEANFILES += $(INSTALLER_FILE)
endif

if DARWIN
appbundle: $(bin_PROGRAMS)
	sh $(srcdir)/../scripts/mkappbundle.sh "$(srcdir)/../data" .
    CLEAN_LOCAL += $(APPBUNDLE_NOLIBS) $(APPBUNDLE) $(APPBUNDLE)*.dmg
endif

clean-local:
	-rm -rf $(CLEAN_LOCAL)

# Sources that we build. It is OK to have headers here.
linsim_SOURCES += \
	main.cxx \
	delay.cxx \
	linsim_ui.cxx \
	noise_gen.cxx \
	path.cxx \
	sim.cxx \
	sound.cxx \
	gauss_fir.cxx \
	csv.cxx \
	include/cmplx.h \
	include/csv.h \
	include/delay.h \
	include/filter_tables.h \
	include/gauss_fir.h \
	include/linsim_ui.h \
	include/main.h \
	include/noise_gen.h \
	include/path.h \
	include/sim.h \
	include/sim_vals.h \
	include/sound.h

# Additional source files that are distributed
EXTRA_DIST = \
	linsim_ui.fl \
	linsim_icon.cxx \
	linsim.rc \
	include/linsimrc.h

# Additional non-source files that are distributed
# Additional source files that support non-Linux cross compilation
EXTRA_DIST += \
	$(srcdir)/../data/linsim.desktop \
	$(srcdir)/../data/linsim.xpm \
	$(srcdir)/../data/win32/fl_app.nsi \
	$(srcdir)/../data/win32/linsim.ico \
	$(srcdir)/../data/linsim.simulations.xls \
	$(srcdir)/../data/linsim.simulations.csv \
	$(srcdir)/../scripts/mknsisinst.sh \
	$(srcdir)/../scripts/build.mxe \
	$(srcdir)/../data/mac/Info.plist.in \
	$(srcdir)/../data/mac/linsim.icns \
	$(srcdir)/../scripts/mkappbundle.sh \
	$(linsim_WIN32_SRC) \
	$(linsim_FL_SRC)
