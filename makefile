CC = g++
CFFLAGS = -lm -Wall -g
jvm: utils.o main.o cmd.o classpath.o entry.o
	$(CC) $(CFFLAGS) $^ -o $@	

cmd_test: utils.o cmd.o cmd_test.o
	$(CC) $(CFFLAGS) $^ -o $@

cmd_test.o: ./cmd/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

classpath_test: utils.o classpath.o entry.o classpath_test.o
	$(CC) $(CFFLAGS) $^ -o $@

classpath_test.o: ./classpath/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

utils.o: utils/utils.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

main.o: main.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

cmd.o: ./cmd/cmd.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

classpath.o: ./classpath/classpath.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

entry.o: ./classpath/entry.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

clean: 
	rm *.o