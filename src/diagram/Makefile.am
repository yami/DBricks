bin_PROGRAMS = dbricks

dbricks_SOURCES = main.cxx \
	Diagram.cxx \
	CustomShape.cxx \
	ModifyContext.cxx \
	DispatchContext.cxx \
	Display.cxx

dbricks_CPPFLAGS = \
	$(GTKMM_CFLAGS) \
	$(RSVG_CFLAGS) \
	$(CAIROMM_CFLAGS) \
	-I$(top_srcdir)/src

dbricks_LDADD = \
	$(GTKMM_LIBS) \
	$(RSVG_LIBS) \
	$(CAIROMM_LIBS) \
	$(top_srcdir)/src/logging/liblogging.a 