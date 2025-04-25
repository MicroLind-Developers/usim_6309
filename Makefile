DEBUG		= -O3
CXX			= g++ --std=c++17 -Wall -Wextra -Werror -flto
CC			= gcc --std=c9x -Wall -Werror
CCFLAGS		= $(DEBUG)
CPPFLAGS	= -D_POSIX_SOURCE -I. -o $(@) -I$(BUILD_DIR)
LDFLAGS		= -flto

LIB_SRCS	= usim.cpp mc6809.cpp mc6809in.cpp mc6850.cpp memory.cpp

OBJS		= $(LIB_SRCS:.cpp=.o)
BIN			= usim

BUILD_DIR	= build

LIB			= libusim.a

all: $(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB): $(OBJS) # $(LIB)($(OBJS))
	ar crs $(BUILD_DIR)/$(@) $^
	ranlib $(BUILD_DIR)/$(@)

$(BIN):	$(LIB) main.o term.o $(BUILD_DIR)
	$(CXX) $(CCFLAGS) $(LDFLAGS) main.o term.o -L. -lusim -o $(BUILD_DIR)/$(@)

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CCFLAGS) -c $< -o $(BUILD_DIR)/$(@)

$(OBJS): machdep.h

machdep: $(BUILD_DIR)/machdep.o $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(@) $(CCFLAGS) $(LDFLAGS) $(BUILD_DIR)/machdep.o

machdep.h: machdep
	./$(BUILD_DIR)/machdep $(BUILD_DIR)/$(@)

clean:
	$(RM) machdep.h machdep.o machdep $(BIN) $(OBJS) main.o term.o $(LIB) $(BUILD_DIR)

depend:	machdep.h
	makedepend 	$(LIB_SRCS) main.cpp term.cpp

# Manually defined dependencies

usim.o: usim.h device.h typedefs.h memory.h wiring.h
usim.o: bits.h
mc6809.o: mc6809.h wiring.h usim.h device.h typedefs.h
mc6809.o: memory.h bits.h machdep.h
mc6809in.o: mc6809.h wiring.h usim.h device.h typedefs.h
mc6809in.o: memory.h bits.h machdep.h
mc6850.o: mc6850.h device.h typedefs.h wiring.h bits.h
memory.o: memory.h device.h typedefs.h
main.o: mc6809.h wiring.h usim.h device.h
main.o: typedefs.h memory.h bits.h machdep.h mc6850.h
main.o: term.h
term.o: term.h mc6850.h device.h typedefs.h wiring.h

# DO NOT DELETE THIS LINE -- make depend depends on it.

