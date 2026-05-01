CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -I./include
CXXFLAGS = -Wall -Wextra -std=c++11 -I./include
TARGET = inventory_app

SRC_DIR = src
OBJ_DIR = obj

# Source files
C_SRCS = $(SRC_DIR)/inventory.c
CXX_SRCS = $(SRC_DIR)/InventoryManager.cpp $(SRC_DIR)/main.cpp

# Object files
C_OBJS = $(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CXX_OBJS = $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(C_OBJS) $(CXX_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) inventory.dat

.PHONY: all clean
