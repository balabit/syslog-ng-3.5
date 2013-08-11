modules_affile_tests_TESTS			=	\
	modules/affile/tests/test_filemonitor

check_PROGRAMS					+=	\
	${modules_affile_tests_TESTS}

modules_affile_tests_test_filemonitor_CFLAGS	=	\
	$(TEST_CFLAGS)					\
	-I$(top_srcdir)/modules/affile
modules_affile_tests_test_filemonitor_LDADD	=	\
	$(TEST_LDADD)					\
	$(top_builddir)/modules/affile/filemonitor.lo
modules_affile_tests_test_filemonitor_LDFLAGS	=	\
	$(PREOPEN_CORE)
