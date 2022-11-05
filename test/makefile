
CC=gcc
AR=ar
SRC=../src
TEST=.
BUILD=../build

SRC_OBJECTS = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(shell find $(SRC) -name "*.c"))
TEST_OBJECTS = $(patsubst $(TEST)/%.c, $(BUILD)/test/%.o, $(shell find $(TEST) -name "*.c"))

CC_FLAGS = -I$(SRC) -c -DSU_USE_STDLIB=1

dir_guard=@mkdir -p $(@D)

all: mkdir $(BUILD)/test.out

$(BUILD)/test.out: $(SRC_OBJECTS) $(TEST_OBJECTS)
	$(dir_guard)
	$(CC) -o $@ $^

$(BUILD)/test/%.o: $(TEST)/%.c
	$(dir_guard)
	$(CC) -o $@ $^ $(CC_FLAGS)

$(BUILD)%.o: $(SRC)/%.c
	$(dir_guard)
	$(CC) -o $@ $^ $(CC_FLAGS)

.PHONY: update
update:
	@curl https://raw.githubusercontent.com/SigJig/sigunit/main/sigunit.h -o sigunit.h

mkdir:
	@mkdir -p $(BUILD)