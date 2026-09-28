CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
TARGET := hello
SRC := hello.cpp

.PHONY: all build run clean

all: build

build:
	@echo "Compilando $(TARGET)..."
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)
	@echo "Compilado com sucesso!"

run: build
	@echo "Executando $(TARGET)..."
	@./$(TARGET)

clean:
	@echo "Limpando artefatos..."
	@rm -f $(TARGET)
	@echo "Limpeza concluida!"
