bin_PROGRAMS = dbricks

dbricks_SOURCES = main.cxx \
	Diagram.cxx \
	CustomShape.cxx \
	ModifyContext.cxx \
	CreateContext.cxx \
	DispatchContext.cxx \
	Display.cxx \
	Desktop.cxx \
	gtkutil.cxx \
	RectShape.cxx \
	EllipseShape.cxx \
	BoxShape.cxx \
	Handle.cxx \
	Connector.cxx \
	LineShape.cxx \
	GroupShape.cxx

dbricks_CPPFLAGS = \
	$(PANGOMM_CFLAGS) \
	$(GTKMM_CFLAGS) \
	$(RSVG_CFLAGS) \
	$(CAIROMM_CFLAGS) \
	-I$(top_srcdir)/src

dbricks_LDADD = \
	$(PANGOMM_LIBS) \
	$(GTKMM_LIBS) \
	$(RSVG_LIBS) \
	$(CAIROMM_LIBS) \
	$(top_srcdir)/src/logging/liblogging.a 