BUILD_DIR=BuildDir
DELEGATETEST=$(BUILD_DIR)/delegateTests.exe
CLOSURETEST=$(BUILD_DIR)/closureTests.exe
BENCHMARK=$(BUILD_DIR)/benchmark.exe
BENCHMARKG=$(BUILD_DIR)/benchmarkg.exe
CC=clang++
# CC=g++-5
CFLAGS=-c -Wall -O0 -g3 -std=c++11
CFLAGS_OPT=-c -O2 -std=c++11
INC=-Iinclude

.cpp.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@

test: $(CLOSURETEST) $(DELEGATETEST)
	./$(CLOSURETEST)
	./$(DELEGATETEST)

testOld: CFLAGS += -D=DELEGATES_TEST_PRE_CPP11
testOld: clean test

bench: CFLAGS = $(CFLAGS_OPT)
bench: $(BENCHMARK)
	./$(BENCHMARK) -s 300
benchg: CFLAGS = $(CFLAGS_OPT)
benchg: $(BENCHMARKG)
	./$(BENCHMARKG)

dir_guard=@mkdir -p $(@D)
includes = $(wildcard include/*.h)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(CLOSURETEST): $(BUILD_DIR)/closureTests.o
	$(dir_guard)
	$(CC) $? -o $@
$(DELEGATETEST): $(BUILD_DIR)/delegateTests.o
	$(dir_guard)
	$(CC) $? -o $@
$(BUILD_DIR)/%.o: tests/%.cpp ${includes}
	$(dir_guard)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

bench_includes = $(wildcard benchmarks/include/*.h)
$(BENCHMARK): $(BUILD_DIR)/bench.o $(BUILD_DIR)/benchmark_worker.o $(BUILD_DIR)/benchmark_virtualworker.o
	$(CC) $? -o $@
$(BUILD_DIR)/%.o: benchmarks/src/%.cpp ${bench_includes} $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -Ibenchmarks/include -Ibenchmarks/include/nonius -c -o $@ $<

bench_includes = $(wildcard benchmark/include/*.h)
$(BENCHMARKG): $(BUILD_DIR)/benchmarkg.o $(BUILD_DIR)/benchmark_worker.o $(BUILD_DIR)/benchmark_virtualworker.o
	$(CC) $? -Lbenchmark/src -lbenchmark -o $@
$(BUILD_DIR)/%.o: benchmarks/src/google/%.cpp ${bench_includes} $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -Ibenchmarks/include -Ibenchmark/include -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

