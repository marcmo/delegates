BUILD_DIR=BuildDir
DELEGATETEST=$(BUILD_DIR)/delegateTest.exe
CLOSURETEST=$(BUILD_DIR)/closureTest.exe
BENCHMARK=$(BUILD_DIR)/benchmark.exe
CC=clang++
CFLAGS=-c -Wall -O0 -g3 -std=c++11
CFLAGS_OPT=-c -O2 -std=c++11
INC=-Iinclude

.cpp.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@

test: $(CLOSURETEST) $(DELEGATETEST)
	./$(CLOSURETEST)
	./$(DELEGATETEST)

bench: $(BENCHMARK)
	./$(BENCHMARK)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(CLOSURETEST): $(BUILD_DIR)/closureTest.o
	$(CC) -o $(CLOSURETEST) $(BUILD_DIR)/closureTest.o

$(BUILD_DIR)/closureTest.o: tests/closureTests.cpp include/delegate.h include/closure.h $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -o$(BUILD_DIR)/closureTest.o tests/closureTests.cpp

$(DELEGATETEST): $(BUILD_DIR)/delegateTest.o
	$(CC) -o $(DELEGATETEST) $(BUILD_DIR)/delegateTest.o

$(BUILD_DIR)/delegateTest.o: tests/delegateTests.cpp include/delegate.h include/delegate.h $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -o$(BUILD_DIR)/delegateTest.o tests/delegateTests.cpp

$(BENCHMARK): $(BUILD_DIR)/benchmark.o $(BUILD_DIR)/benchmark_worker.o
	$(CC) -o $(BENCHMARK) $(BUILD_DIR)/benchmark_worker.o $(BUILD_DIR)/benchmark.o

$(BUILD_DIR)/benchmark_worker.o: src/benchmark_worker.cpp $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -o$(BUILD_DIR)/benchmark_worker.o src/benchmark_worker.cpp

$(BUILD_DIR)/benchmark.o: src/benchmark.cpp src/benchmark_worker.cpp src/benchmark_worker.h include/delegate.h include/closure.h $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -o$(BUILD_DIR)/benchmark.o src/benchmark.cpp

clean:
	rm -rf $(BUILD_DIR)

