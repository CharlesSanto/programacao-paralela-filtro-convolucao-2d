CXX = g++
MPICXX = mpicxx
CXXFLAGS = -O3 -march=native -Wall

SRC_DIR = src
BIN_DIR = bin

TARGETS = $(BIN_DIR)/baseline $(BIN_DIR)/openmp_convolucao $(BIN_DIR)/mpi_convolucao

all: create_dir $(TARGETS)

create_dir:
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/baseline: $(SRC_DIR)/baseline.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/openmp_convolucao: $(SRC_DIR)/openmp_convolucao.cpp
	$(CXX) $(CXXFLAGS) -fopenmp $< -o $@

$(BIN_DIR)/mpi_convolucao: $(SRC_DIR)/mpi_convolucao.cpp
	$(MPICXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR) gmon.out