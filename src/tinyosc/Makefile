include ../../config.mak

ifneq ($(targetos), Darwin)
NAME = libtinyosc$(LIBSUF)
TARGET = $(NAME).$(version)
SONAME = $(NAME).$(soversion)
SHFLAGS += -Wl,-soname,$(SONAME)
else
NAME = libtinyosc$(LIBSUF)
TARGET = libtinyosc.$(version)$(LIBSUF)
SONAME = libtinyosc.$(soversion)$(LIBSUF)
SHFLAGS += -install_name $(libdir)/$(SONAME) -current_version $(version) -compatibility_version $(soversion)
endif

OBJS = tinyosc.o

INCS = tinyosc.h

SRCS := $(OBJS:.o=.c)

CFLAGS += -I.. $(RDYNAMIC)

all: $(TARGET)

$(TARGET): $(OBJS)
		$(CC) $(SHFLAGS) -o $@ $(OBJS) $(LDFLAGS)
		ln -sf $(TARGET) $(NAME)
		ln -sf $(TARGET) $(SONAME)

depend:	$(SRCS)
		$(CC) -MM $(CFLAGS) $^ 1>.depend

distclean:	clean
		rm -f .depend

clean:	
		rm -f $(OBJS) $(TARGET) $(NAME) $(SONAME)

install: 	all
	install -m 755 $(TARGET) $(DESTDIR)$(libdir)
	ln -sf $(TARGET) $(DESTDIR)$(libdir)/$(SONAME)
	ln -sf $(TARGET) $(DESTDIR)$(libdir)/$(NAME)
	mkdir -p "$(DESTDIR)$(prefix)/include/mlt/mvcp"
	install -m 644 $(INCS) "$(DESTDIR)$(prefix)/include/mlt/mvcp"

uninstall:
	rm -f "$(DESTDIR)$(libdir)/$(TARGET)"
	rm -f "$(DESTDIR)$(libdir)/$(SONAME)"
	rm -f "$(DESTDIR)$(libdir)/$(NAME)"
	rm -rf "$(DESTDIR)$(prefix)/include/mlt/mvcp"

ifneq ($(wildcard .depend),)
include .depend
endif
