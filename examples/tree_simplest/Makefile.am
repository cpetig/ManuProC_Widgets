# generated 2001/6/15 9:52:50 CEST by christof@puck.(none)
# using glademm V0.6.2_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new
EXTRA_PROGRAMS = tree

tree_SOURCES = \
	tree.cc\
	with_class_glade.cc \
	with_class.cc \
	simplest_glade.cc \
	simplest.cc

noinst_HEADERS = \
	with_class_glade.hh \
	with_class.hh \
	simplest.hh \
	simplest_glade.hh

AM_CXXFLAGS =-I$(top_srcdir)/src @COMMONGTK2_INCLUDES@ \
		@MPC_BASE_INCLUDES@

tree_LDADD = @LIBS@ $(top_srcdir)/src/libManuProC_Widgets.a \
        @COMMONGTK2_LIBS@ @MPC_BASE_LIBS@

AM_LDFLAGS= @MPC_BASE_LDFLAGS@ @COMMONGTK2_LDFLAGS@
