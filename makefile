BUILD_DIR=BuildDir
DELEGATETEST=$(BUILD_DIR)/delegateTests.exe
CLOSURETEST=$(BUILD_DIR)/closureTests.exe
BENCHMARK=$(BUILD_DIR)/benchmark.exe
CC=clang++
CFLAGS=-c -Wall -O0 -g3 -std=c++11
CFLAGS_OPT=-c -O3 -std=c++11
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
	./$(BENCHMARK) -s 1000

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

clean:
	rm -rf $(BUILD_DIR)

