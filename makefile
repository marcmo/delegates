BUILD_DIR=BuildDir
TEST1=$(BUILD_DIR)/sample.exe
TEST2=$(BUILD_DIR)/sample01.exe

run: $(TEST1) $(TEST2)
	./$(TEST1)
	./$(TEST2)

$(TEST1): $(BUILD_DIR)/sample.o
	g++ -o $(TEST1) $(BUILD_DIR)/sample.o

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/sample.o: tests/sample.cpp include/Delegate.h $(BUILD_DIR)
	g++ -c -Wall -O0 -g3 -Wall -std=c++11 -Iinclude -DUNIT_TEST -o$(BUILD_DIR)/sample.o tests/sample.cpp

$(TEST2): $(BUILD_DIR)/sample01.o
	g++ -o $(TEST2) $(BUILD_DIR)/sample01.o

$(BUILD_DIR)/sample01.o: tests/sample01.cpp include/Delegate01.h
	g++ -c -Wall -O0 -g3 -Wall -std=c++11 -Iinclude -DUNIT_TEST -o$(BUILD_DIR)/sample01.o tests/sample01.cpp

clean:
	rm -r $(BUILD_DIR)

