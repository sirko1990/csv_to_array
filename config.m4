PHP_ARG_ENABLE(csvtoarray, Enable viewer support)

if test "$PHP_CSVTOARRAY" = "yes"; then
   AC_DEFINE(HAVE_CSVTOARRAY, 1, [You have CSVTOARRAY extension])
   PHP_NEW_EXTENSION(csvtoarray, csvtoarray.c, $ext_shared)
fi