#!/bin/sh

mkdir include

ln -s $TCS/include/common/ include/
ln -s $TCS/include/ops/ include/
ln -s $TCS/include/tmlib/ include/

ln -s $TCS/include/varargs.h include/
ln -s $TCS/include/stddef.h include/
ln -s $TCS/include/iso646.h include/
ln -s $TCS/include/stdarg.h include/
ln -s $TCS/include/float.h include/
ln -s ../stdbool.h include/

