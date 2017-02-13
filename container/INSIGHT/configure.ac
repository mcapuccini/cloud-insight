dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.63)
AC_INIT([INSIGHTv3], [1.0], [jan.mikelson@bsse.ethz.ch])
AC_CONFIG_MACRO_DIR([m4])
AC_ARG_ENABLE([mpi], AS_HELP_STRING([--enable-mpi], [enables mpi compilation]))

AX_PROG_CXX_MPI([test "x$enable_mpi" = "xyes"], [HAVE_MPI=yes], [HAVE_MPI=no]) 
AM_CONDITIONAL([USE_MPI], [test "x$HAVE_MPI" = "xyes"])

dnl Initialize automake
AM_INIT_AUTOMAKE([foreign])

AC_LANG([C++])

AC_PROG_LIBTOOL
AC_CONFIG_FILES([Makefile])

AC_ARG_VAR(Eigen3_CFLAGS, [The location of the Eigen3.2 library])
AS_IF([test "x$Eigen3_CFLAGS" = "x"], [AC_SUBST(Eigen3_CFLAGS, [-I./eigen3.2])])

AS_IF([test "x$enable_mpi" = "xyes"], [AC_CHECK_LIB([mpi], [MPI_Init])] )
AC_CHECK_LIB([boost_program_options], [main])
AC_CHECK_LIB([boost_serialization], [main])
AC_OUTPUT
