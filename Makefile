PROG = rcbrowser
#MODULE_CFLAGS=  -DMG_ENABLE_THREADS -DMG_ENABLE_HTTP_WEBSOCKET=0
MODULE_CFLAGS=-DMG_DISABLE_DAV_AUTH -DMG_ENABLE_FAKE_DAVLOCK
MONGOOSE_DIR = ../mongoose
SOURCES = rcbrowser.cpp\
	 $(MONGOOSE_DIR)/mongoose.c
CFLAGS = -g -W -Wall -Werror -I$(MONGOOSE_DIR) -Wno-unused-function $(CFLAGS_EXTRA) $(MODULE_CFLAGS)

all: $(PROG)

ifeq ($(OS), Windows_NT)
# TODO(alashkin): enable SSL in Windows
CFLAGS += -lws2_32
CC = gcc
else
CFLAGS += -pthread
endif

ifdef ASAN
CC = clang
CFLAGS += -fsanitize=address
endif

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I../.. /MD /Fe$@

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)


