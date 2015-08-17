BUILD_DIR=BuildDir
DELEGATETEST=$(BUILD_DIR)/delegateTest.exe
CLOSURETEST=$(BUILD_DIR)/closureTest.exe
CC=clang++
CFLAGS=-c -Wall -O0 -g3 -std=c++11
INC=-Iinclude

.cpp.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@

test: $(CLOSURETEST) $(DELEGATETEST)
	./$(CLOSURETEST)
	./$(DELEGATETEST)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(CLOSURETEST): $(BUILD_DIR)/closureTest.o
	$(CC) -o $(CLOSURETEST) $(BUILD_DIR)/closureTest.o

$(BUILD_DIR)/closureTest.o: tests/ClosureTests.cpp include/Delegate.h include/Closure.h $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -o$(BUILD_DIR)/closureTest.o tests/ClosureTests.cpp

$(DELEGATETEST): $(BUILD_DIR)/delegateTest.o
	$(CC) -o $(DELEGATETEST) $(BUILD_DIR)/delegateTest.o

$(BUILD_DIR)/delegateTest.o: tests/DelegateTests.cpp include/Delegate.h include/Delegate.h $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -o$(BUILD_DIR)/delegateTest.o tests/DelegateTests.cpp

clean:
	rm -rf $(BUILD_DIR)

