AUTOMAKE_OPTIONS = subdir-objects
lib_LTLIBRARIES = \
    libmobajson.la

libmobajson_la_SOURCES = \
    jsonstreamreadersocket.cpp \
    jsondecoder.cpp

otherincludedir = \
    $(includedir)/moba-json

otherinclude_HEADERS = \
    json.h \
    jsonstreamreadersocket.h \
    jsonstreamreader.h \
    jsondecoder.h

ACLOCAL_AMFLAGS = \
    -I m4

CLEANFILES = \
    *~

AM_CPPFLAGS = \
    -g3 -g -ggdb -O3 -Wall
