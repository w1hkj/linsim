#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Generic support for FLTK applications


AC_COPYRIGHT([Copyright (C) 2009 Dave Freese, W1HKJ (w1hkj AT w1hkj DOT com)])

AC_PREREQ([2.71])

dnl major and minor must be integers; patch may
dnl contain other characters or be empty
m4_define(linsim_MAJOR,  [2])
m4_define(linsim_MINOR,  [0])
m4_define(linsim_PATCH,  [.5])

AC_INIT([linsim],[linsim_MAJOR.linsim_MINOR[]linsim_PATCH],[w1hkj AT w1hkj DOT com])

AC_SUBST([linsim_VERSION_MAJOR], [linsim_MAJOR])
AC_SUBST([linsim_VERSION_MINOR], [linsim_MINOR])
AC_SUBST([linsim_VERSION_PATCH], [linsim_PATCH])
AC_SUBST([linsim_VERSION], [linsim_MAJOR.linsim_MINOR[]linsim_PATCH])

AC_DEFINE([linsim_VERSION_MAJOR], [linsim_MAJOR], [major version number])
AC_DEFINE([linsim_VERSION_MINOR], [linsim_MINOR], [minor version number])
AC_DEFINE([linsim_VERSION_PATCH], ["linsim_PATCH"], [patch/alpha version string])
AC_DEFINE([linsim_VERSION], ["linsim_MAJOR.linsim_MINOR[]linsim_PATCH"], [version string])

AC_SUBST([AC_CONFIG_ARGS], [$ac_configure_args])
AC_CONFIG_AUX_DIR([build-aux])

# define build, build_cpu, build_vendor, build_os
AC_CANONICAL_BUILD
# define host, host_cpu, host_vendor, host_os
AC_CANONICAL_HOST
# define target, target_cpu, target_vendor, target_os
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([-Wall -Wno-portability foreign std-options subdir-objects 1.9.6])

#change the next config item for the specific application src's
AC_CONFIG_SRCDIR([src/main.cxx])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS

# Checks for header files.
m4_warn([obsolete],
[The preprocessor macro `STDC_HEADERS' is obsolete.
  Except in unusual embedded environments, you can safely include all
  ISO C90 headers unconditionally.])dnl
# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_HEADER_DIRENT
AC_CHECK_HEADERS([arpa/inet.h execinfo.h fcntl.h limits.h memory.h netdb.h netinet/in.h regex.h stdint.h stdlib.h string.h strings.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h sys/utsname.h termios.h unistd.h values.h linux/ppdev.h dev/ppbus/ppi.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
m4_warn([obsolete],
[Update your code to rely only on HAVE_SYS_TIME_H,
then remove this warning and the obsolete code below it.
All current systems provide time.h; it need not be checked for.
Not all systems provide sys/time.h, but those that do, all allow
you to include it and time.h simultaneously.])dnl
AC_CHECK_HEADERS_ONCE([sys/time.h])
# Obsolete code to be removed.
if test $ac_cv_header_sys_time_h = yes; then
  AC_DEFINE([TIME_WITH_SYS_TIME],[1],[Define to 1 if you can safely include both <sys/time.h>
	     and <time.h>.  This macro is obsolete.])
fi
# End of obsolete code.

AC_STRUCT_TM
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_PROG_GCC_TRADITIONAL
dnl AC_FUNC_MALLOC
dnl AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
m4_warn([obsolete],
[your code may safely assume C89 semantics that RETSIGTYPE is void.
Remove this warning and the `AC_CACHE_CHECK' when you adjust the code.])dnl
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[AC_COMPILE_IFELSE(
[AC_LANG_PROGRAM([#include <sys/types.h>
#include <signal.h>
],
		 [return *(signal (0, 0)) (0) == 1;])],
		   [ac_cv_type_signal=int],
		   [ac_cv_type_signal=void])])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[$ac_cv_type_signal],[Define as the return type of signal handlers
		    (`int' or `void').])

AC_FUNC_STRFTIME
AC_FUNC_STRTOD
AC_CHECK_FUNCS([getaddrinfo gethostbyname hstrerror gmtime_r localtime_r memmove memset mkdir select setenv snprintf socket socketpair strcasecmp strcasestr strchr strdup strerror strlcpy strncasecmp strrchr strstr strtol uname unsetenv vsnprintf])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([strstr])

### static flag
# Set ac_cv_static to yes/no
# Substitute RTLIB in Makefile
AC_FL_STATIC

### optimizations
# Set ac_cv_opt to arg
# Substitute OPT_FLAGS in Makefile
AC_FL_OPT

### debug flag
# Set ac_cv_debug to yes/no
# Override CXXFLAGS
# Set ENABLE_DEBUG Makefile conditional
# Substitute RDYNAMIC in Makefile
AC_FL_DEBUG

###### OS support
### OSX
# Set ac_cv_mac_universal to yes/no
# Set DARWIN Makefile conditional
# Substitute MAC_UNIVERSAL_CFLAGS and MAC_UNIVERSAL_LDFLAGS in Makefile
AC_FL_MACOSX
### win32
# Set WIN32 Makefile conditional
# Set HAVE_WINDRES Makefile conditional
# Substitute WINDRES in Makefile
AC_FL_WIN32

### X11
# Set ac_cv_x to yes/no
# Define USE_X in config.h
# Substitute X_CFLAGS and X_LIBS in Makefile
if test "x$target_darwin" = "xno" && test "x$target_win32" = "xno"; then
    AC_linsim_PKG_CHECK([x], [x11], [no], [yes])
fi

### libsamplerate
# Required if $ac_cv_want_fldigi is "yes"
# Set ac_cv_samplerate to yes/no    (not used)
# Define USE_SAMPLERATE in config.h (as above)
# Substitute SAMPLERATE_CFLAGS and SAMPLERATE_LIBS in Makefile
AC_linsim_PKG_CHECK([samplerate], [samplerate >= 0.1.1], [no], [no])

### libsndfile
# Set ac_cv_sndfile to yes/no
# Define USE_SNDFILE in config.h
# Substitute SNDFILE_CFLAGS and SNDFILE_LIBS in Makefile
AC_linsim_PKG_CHECK([sndfile], [sndfile >= 1.0.10], [yes], [yes],
                    [enable reading/writing of audio files via libsndfile @<:@autodetect@:>@] )

### FLTK
# Substitute FLTK_CFLAGS and FLTK_LIBS in Makefile
# Set FLUID variable
# Set HAVE_FLUID Makefile conditional
AC_CHECK_FLTK

### XML-RPC library
# Set ac_cv_xmlrpc to yes/no
# Substitute XMLRPC_CFLAGS and XMLRPC_LIBS in Makefile
# Define USE_XMLRPC in config.h
# Set ENABLE_XMLRPC Makefile conditional
#AC_linsim_XMLRPC

### Non-POSIX compatibility (i.e. mingw32)
# Sets various Makefile conditionals; see m4/np-compat.m4
AC_FL_NP_COMPAT

### build info
# Define various build variables in config.h
AC_linsim_BUILD_INFO

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT

### summary
AC_MSG_RESULT([
Configuration summary:

  Version ..................... $VERSION
  
  Target OS ................... $target_os

  Static linking .............. $ac_cv_static
  CPU optimizations ........... $ac_cv_opt
  Debugging ................... $ac_cv_debug

  sndfile ..................... $ac_cv_sndfile
])
