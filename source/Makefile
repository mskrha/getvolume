BUILD	= getvolume
VERSION	?= 0.0
ARCH	?= amd64
PKG	?= $(BUILD)_$(VERSION)_$(ARCH)
OUTPUT	= $(BUILD)
LIBS	= -lasound
SRC	= main.c
CC	= gcc
OPTS	= -pedantic
OPTS	+= -Wall
OPTS	+= -ansi
OPTS	+= -pedantic-errors

all: clean build

clean:
	rm -f $(OUTPUT)

build:  clean
	$(CC) $(OPTS) -o $(OUTPUT) $(SRC) $(LIBS)

temp:
	$(eval TMPDIR = $(shell mktemp -d))
	$(eval OUTPUT = $(TMPDIR)/usr/bin/$(BUILD))
	mkdir -p $(TMPDIR)/usr/bin

files: temp
	cp -a ../DEBIAN $(TMPDIR)/

deb: files build
	$(eval SIZE = $(shell du -k $(OUTPUT) | sed 's/^\([0-9]\+\).*/\1/'))
	$(shell sed -i "s/INSTALL_SIZE/$(SIZE)/" $(TMPDIR)/DEBIAN/control)
	$(shell sed -i "s/INSTALL_ARCH/$(ARCH)/" $(TMPDIR)/DEBIAN/control)
	$(shell sed -i "s/INSTALL_VERSION/$(VERSION)/" $(TMPDIR)/DEBIAN/control)
	dpkg-deb --build $(TMPDIR) $(PKG).deb
	rm -rf $(TMPDIR)
