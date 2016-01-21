PHP_ARG_ENABLE(viewer, Enable viewer support)

if test "$PHP_VIEWER" = "yes"; then
   AC_DEFINE(HAVE_VIEWER, 1, [You have viewer extension])
   PHP_NEW_EXTENSION(viewer, viewer.c, $ext_shared)
fi