PROG = rcbrowser
#MODULE_CFLAGS=  -DMG_ENABLE_THREADS -DMG_ENABLE_HTTP_WEBSOCKET=0
MODULE_CFLAGS=-DMG_DISABLE_DAV_AUTH -DMG_ENABLE_FAKE_DAVLOCK
MONGOOSE_DIR = ../mongoose
SOURCES = rcbrowser.cpp\
	 $(MONGOOSE_DIR)/mongoose.c
CFLAGS = -g -W -Wall -I$(MONGOOSE_DIR) -Wno-unused-function $(CFLAGS_EXTRA) $(MODULE_CFLAGS)  -std=c++11
CFLAGS += -pthread
CFLAGS += -I../rapidjson/include/

all: $(PROG)


$(PROG): $(SOURCES)
	$(CXX) $(SOURCES) -o $@ $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I../.. /MD /Fe$@

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)


