TARGET ?= platinum
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++17

$(TARGET): $(OBJS)
	g++ $(LDFLAGS) $(OBJS) -o build/$@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	-rm -f $(shell find $(SRC_DIRS) -name *.o -or -name *.d)