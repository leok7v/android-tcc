#TODO:
#SVNDEV := -D'SVN_REV="$(shell svnversion -n ../../repo)"'
echo "#define ZSPACE_REVISION 1" > jni/inc/revision.h