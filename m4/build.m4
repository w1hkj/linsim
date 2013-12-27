AC_DEFUN([AC_linsim_SH_DQ], [
  ac_sh_dq="\"`$1 | sed 's/"/\\\\"/g'`\""
])

AC_DEFUN([AC_linsim_BUILD_INFO], [
# Define build flags and substitute in Makefile.in
# CPPFLAGS
  linsim_BUILD_CPPFLAGS="-I\$(srcdir) -I\$(srcdir)/include"
  if test "x$target_win32" = "xyes"; then
      linsim_BUILD_CPPFLAGS="$linsim_BUILD_CPPFLAGS -D_WINDOWS"
  fi
# CXXFLAGS
  linsim_BUILD_CXXFLAGS="$FLTK_CFLAGS $linsim_BUILD_CPPFLAGS $SNDFILE_CFLAGS -pipe -Wall -fexceptions $OPT_CFLAGS $DEBUG_CFLAGS $PTW32_CFLAGS"
  if test "x$target_mingw32" = "xyes"; then
      linsim_BUILD_CXXFLAGS="-mthreads $linsim_BUILD_CXXFLAGS -D_WINDOWS"
  fi
# LDFLAGS
  linsim_BUILD_LDFLAGS=
# LDADD
  linsim_BUILD_LDADD="$FLTK_LIBS $X_LIBS $SNDFILE_LIBS $EXTRA_LIBS $PTW32_LIBS"

  if test "x$ac_cv_debug" = "xyes"; then
      linsim_BUILD_CXXFLAGS="$linsim_BUILD_CXXFLAGS -UNDEBUG"
      linsim_BUILD_LDFLAGS="$linsim_BUILD_LDFLAGS $RDYNAMIC"
  else
      linsim_BUILD_CXXFLAGS="$linsim_BUILD_CXXFLAGS -DNDEBUG"
  fi
  if test "x$target_mingw32" = "xyes"; then
      linsim_BUILD_LDFLAGS="-mthreads $linsim_BUILD_LDFLAGS"
  fi

  AC_SUBST([linsim_BUILD_CPPFLAGS])
  AC_SUBST([linsim_BUILD_CXXFLAGS])
  AC_SUBST([linsim_BUILD_LDFLAGS])
  AC_SUBST([linsim_BUILD_LDADD])

#define build variables for config.h
  AC_DEFINE_UNQUOTED([BUILD_BUILD_PLATFORM], ["$build"], [Build platform])
  AC_DEFINE_UNQUOTED([BUILD_HOST_PLATFORM], ["$host"], [Host platform])
  AC_DEFINE_UNQUOTED([BUILD_TARGET_PLATFORM], ["$target"], [Target platform])

  test "x$LC_ALL" != "x" && LC_ALL_saved="$LC_ALL"
  LC_ALL=C
  export LC_ALL

  AC_linsim_SH_DQ([echo $ac_configure_args])
  AC_DEFINE_UNQUOTED([BUILD_CONFIGURE_ARGS], [$ac_sh_dq], [Configure arguments])

  AC_linsim_SH_DQ([date])
  AC_DEFINE_UNQUOTED([BUILD_DATE], [$ac_sh_dq], [Build date])

  AC_linsim_SH_DQ([whoami])
  AC_DEFINE_UNQUOTED([BUILD_USER], [$ac_sh_dq], [Build user])

  AC_linsim_SH_DQ([hostname])
  AC_DEFINE_UNQUOTED([BUILD_HOST], [$ac_sh_dq], [Build host])

  AC_linsim_SH_DQ([$CXX -v 2>&1 | tail -1])
  AC_DEFINE_UNQUOTED([BUILD_COMPILER], [$ac_sh_dq], [Compiler])

  AC_linsim_SH_DQ([echo $linsim_BUILD_CPPFLAGS $linsim_BUILD_CXXFLAGS])
  AC_DEFINE_UNQUOTED([linsim_BUILD_CXXFLAGS], [$ac_sh_dq], [linsim compiler flags])
  AC_linsim_SH_DQ([echo $linsim_BUILD_LDFLAGS $linsim_BUILD_LDADD])
  AC_DEFINE_UNQUOTED([linsim_BUILD_LDFLAGS], [$ac_sh_dq], [linsim linker flags])

  if test "x$LC_ALL_saved" != "x"; then
      LC_ALL="$LC_ALL_saved"
      export LC_ALL
  fi
])
