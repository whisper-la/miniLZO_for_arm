PROGRAM = lzo_tool
SOURCES = lzo_tool.c minilzo.c


# Make sure that minilzo.h, lzoconf.h and lzodefs.h are in the
# current dircectory. Otherwise you may want to adjust CPPFLAGS.
CPPFLAGS = -I. -I../include/lzo

GCC_CFLAGS = -s -Wall -O2 -fomit-frame-pointer

default:
	arm-rockchip830-linux-uclibcgnueabihf-gcc $(CPPFLAGS) $(GCC_CFLAGS) -o $(PROGRAM) $(SOURCES)


gcc:
	gcc $(CPPFLAGS) $(GCC_CFLAGS) -o $(PROGRAM) $(SOURCES)

#
# other targets
#

clean:
	rm -f $(PROGRAM) $(PROGRAM).exe $(PROGRAM).map $(PROGRAM).tds
	rm -f *.err *.o *.obj

.PHONY: default clean
