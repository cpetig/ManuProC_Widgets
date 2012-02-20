SUBDIRS=newtree tree_simplest dwhouse MVC

EXTRA_PROGRAMS = Event TreeViewUtil TreeSelection test_sts test_logwin \
	test_clist datewintest EvenMoreSimpleUsage BasicExcel xlsview STExcel

AM_CXXFLAGS=@MPC_WIDGETS_INCLUDES@ 
#-Wl,--enable-runtime-pseudo-reloc

Event_SOURCES=Event.cc
Event_LDADD=@MPC_WIDGETS_LIBS@
TreeViewUtil_SOURCES=TreeViewUtil.cc
TreeViewUtil_LDADD=@MPC_WIDGETS_LIBS@
TreeSelection_SOURCES=TreeSelection.cc
TreeSelection_LDADD=@MPC_WIDGETS_LIBS@
test_sts_SOURCES=test_sts.cc
test_sts_LDADD=@MPC_WIDGETS_LIBS@
test_logwin_SOURCES=test_logwin.cc
test_logwin_LDADD=@MPC_WIDGETS_LIBS@
test_clist_SOURCES=test_clist.cc
test_clist_LDADD=@MPC_WIDGETS_LIBS@
datewintest_SOURCES=datewintest.cc
datewintest_LDADD = @MPC_WIDGETS_LIBS@
EvenMoreSimpleUsage_SOURCES=EvenMoreSimpleUsage.cc
EvenMoreSimpleUsage_LDADD= @MPC_WIDGETS_LIBS@
BasicExcel_SOURCES=BasicExcel.cc
BasicExcel_LDADD=@MPC_WIDGETS_LIBS@
xlsview_SOURCES=xlsview.cc
xlsview_LDADD=@MPC_WIDGETS_LIBS@
STExcel_SOURCES=STExcel.cc
STExcel_LDADD=@MPC_WIDGETS_LIBS@

AM_LDFLAGS=@MPC_WIDGETS_LDFLAGS@
