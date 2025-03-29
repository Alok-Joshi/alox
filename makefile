INCLUDE_DIR = include
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build/
SRC = $(wildcard $(SRC_DIR)/*.cpp)
EXE = ${BIN_DIR}/alox
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

CXX = g++
CXXFLAGS = -g

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	${CXX} $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(EXE): ${OBJ} | $(BIN_DIR)
	${CXX} $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $^

$(BIN_DIR) $(BUILD_DIR):
	mkdir -p $@

clean: 
	rm -r $(BIN_DIR) $(BUILD_DIR)
