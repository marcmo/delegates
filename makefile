BUILD_DIR=BuildDir
DELEGATETEST=$(BUILD_DIR)/delegateTest.exe
SAMPLE=$(BUILD_DIR)/sample01.exe
CLOSURETEST=$(BUILD_DIR)/closureTest.exe
CC=g++
CFLAGS=-c -Wall -O0 -g3 -std=c++11
INC=-Iinclude

run: $(SAMPLE)
	./$(SAMPLE)

.cpp.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@

test: $(CLOSURETEST) $(DELEGATETEST)
	./$(CLOSURETEST)
	./$(DELEGATETEST)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SAMPLE): $(BUILD_DIR)/sample01.o
	$(CC) -o $(SAMPLE) $(BUILD_DIR)/sample01.o

$(BUILD_DIR)/sample01.o: tests/sample01.cpp include/Delegate01.h $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -o$(BUILD_DIR)/sample01.o tests/sample01.cpp

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

