CC = gcc
CFLAGS = -Wall -I./src/logging -I./src/requests -I./src/openai
LDFLAGS = -lcurl
BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = obj

# Source files
LOGGING_SRC = $(SRC_DIR)/logging/logging.c
REQUESTS_SRC = $(SRC_DIR)/requests/requests.c
OPENAI_SRC = $(SRC_DIR)/openai/openai.c
TESTS_SRC = $(SRC_DIR)/tests/test.c

# Object files
OBJS = $(LOGGING_SRC) $(REQUESTS_SRC) $(TESTS_SRC) $(OPENAI_SRC)

# Output binary
TEST_BIN = $(BIN_DIR)/test

.PHONY: all clean run

all: $(TEST_BIN);

$(TEST_BIN): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BIN_DIR)
