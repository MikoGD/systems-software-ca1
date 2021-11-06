CC := gcc

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/main
SRC := $(filter-out src/send_message.c, $(wildcard $(SRC_DIR)/*.c))
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude -MMD -MP
CFLAGS := -Wall -g

.PHONY := clean all

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

send: $(OBJ_DIR)/send_message.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $(BIN_DIR)/$@

$(OBJ_DIR)send_message.o: $(SRC_DIR)/send_message.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)