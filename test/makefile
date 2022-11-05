
CC=gcc
AR=ar
SRC=../src
TEST=.
BUILD=../build

SRC_OBJECTS = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(shell find $(SRC) -name "*.c"))
TEST_OBJECTS = $(patsubst $(TEST)/%.c, $(BUILD)/test/%.o, $(shell find $(TEST) -name "*.c"))

CC_FLAGS = -I$(SRC) -c

dir_guard=@mkdir -p $(@D)

all: mkdir $(BUILD)/test.out

$(BUILD)/test.out: $(SRC_OBJECTS) $(TEST_OBJECTS)
	$(dir_guard)
	$(CC) -o $@ $^

$(BUILD)/test/%.o: $(TEST)/%.c
	$(dir_guard)
	$(CC) -o $@ $^ $(CC_FLAGS)

$(BUILD)/lib.a: $(SRC_OBJECTS)
	$(dir_guard)
	$(AR) rcs $@ $^

$(BUILD)%.o: $(SRC)/%.c
	$(dir_guard)
	$(CC) -o $@ $^ $(CC_FLAGS)

mkdir:
	@mkdir -p $(BUILD)