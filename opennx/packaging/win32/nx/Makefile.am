NXESD             = nxesd
NXCOMP            = nxcomp
AUDIOFILE         = audiofile
AUDIOFILE_VERSION = 0.2.6
TMPDIST           = $(abs_builddir)/dist

CFG_GENERIC = \
	--prefix=/ --libdir=/lib --bindir=/bin --mandir=/share/man \
	--includedir=/include --datadir=/share --exec-prefix=/libexec \
	--libexecdir=/libexec --sysconfdir=/etc
CFG_audiofile = $(CFG_GENERIC)
CFG_nxesd = $(CFG_GENERIC) --disable-audiofiletest --with-audiofile=yes --program-transform-name='s/i686-pc-mingw32-//'
CFG_nxcomp = $(CFG_GENERIC) \
			 --x-includes=$(abs_top_builddir)/packaging/win32/xming/xc/exports/include \
			 --x-libraries=$(abs_top_builddir)/packaging/win32/xming/dist/usr/X11R6/lib
CFG_nxssh = $(CFG_GENERIC)

ENV_GENERIC = CPPFLAGS="-I$(TMPDIST)/include" LDFLAGS="-L$(TMPDIST)/lib"
ENV_audiofile =
ENV_nxesd = env $(ENV_GENERIC) LIBS=-laudiofile

EXTRA_DIST = urls.txt patches $(AUDIOFILE).md5sum $(NXESD).md5sum \
	$(NXCOMP).md5sum

CLEANFILES = -r $(TMPDIST) \
	$(AUDIOFILE)-$(AUDIOFILE_VERSION) $(AUDIOFILE)-build-$(NX_BTYPE) \
	$(AUDIOFILE)-srcdir $(AUDIOFILE)-checkarchive \
	$(NXESD) $(NXESD)-build-$(NX_BTYPE) $(NXESD)-srcdir $(NXESD)-checkarchive \
	$(NXCOMP) $(NXCOMP)-build-$(NX_BTYPE) $(NXCOMP)-srcdir $(NXCOMP)-checkarchive

DISTCLEANFILES = -r $(CLEANFILES) \
	$(AUDIOFILE)-$(AUDIOFILE_VERSION).tar.gz \
	$(NXESD)*.tar.gz \
	$(NXCOMP)*.tar.gz

MAINTAINERCLEANFILES = Makefile.in

tmpinstall:
	$(MAKE) subtarget=$(AUDIOFILE) subsrc=$(AUDIOFILE)-$(AUDIOFILE_VERSION) build-$(NX_BTYPE)
	$(MAKE) subtarget=$(NXESD) subsrc=$(NXESD) build-$(NX_BTYPE)
	$(MAKE) subtarget=$(NXCOMP) subsrc=$(NXCOMP) build-$(NX_BTYPE)

build-cross: $(subtarget)-build-cross

build-native: $(subtarget)-build-native

$(subtarget)-build-cross: $(subtarget)-srcdir
	@echo Building $(subtarget)
	@cd $(subsrc) && $(ENV_$(subtarget)) mingw32-configure $(CFG_$(subtarget))
	@$(MAKE) -C $(subsrc) DESTDIR=$(TMPDIST) install
	@touch $@

$(subtarget)-build-native: $(subtarget)-srcdir
	@echo Building $(subtarget)
	@cd $(subsrc) && $(ENV_$(subtarget)) ./configure $(CFG_$(subtarget))
	@$(MAKE) -C $(subsrc) DESTDIR=$(TMPDIST) install
	@touch $@

$(subtarget)-srcdir: $(subtarget)-checkarchive
	@$(RM) -rf $(subsrc)
	@echo Unpacking $(subsrc).tar.gz
	@tar xzf $(subsrc).tar.gz
	@shopt -s nullglob; for p in $(srcdir)/patches/$(subtarget)-*.patch ; do \
		echo Applying `basename $$p`; \
		patch -p0 < $$p ; \
	done
	@touch $@

$(subtarget)-checkarchive: $(subsrc).tar.gz
	@echo Checking tarball $(subsrc).tar.gz
	@md5sum --status -c $(srcdir)/$(subtarget).md5sum
	@touch $@

$(subsrc).tar.gz: $(srcdir)/urls.txt
	@echo Downloading $@
	@rm -f $@
	@if grep -q $(subsrc) $(srcdir)/urls.txt ; then \
		wget -O $@ $(shell grep $(subsrc) $(srcdir)/urls.txt) ; \
	else \
		$(top_srcdir)/getnxsrcpkg $(subsrc) && $(LN_S) $(subsrc)-*.tar.gz $(subsrc).tar.gz ; \
	fi
	@touch $@