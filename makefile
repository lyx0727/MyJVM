CC = g++
CFFLAGS = -lm -Wall -g
jvm: utils.o main.o cmd.o classpath.o entry.o attribute_info.o class_reader.o constant_pool.o member_info.o constant_info.o classfile.o
	$(CC) $(CFFLAGS) $^ -o $@	

utils.o: utils/utils.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

main.o: main.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

# cmd
cmd_test: utils.o cmd.o cmd_test.o
	$(CC) $(CFFLAGS) $^ -o $@

cmd_test.o: ./cmd/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

cmd.o: ./cmd/cmd.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

# classpath
classpath_test: utils.o classpath.o classpath_test.o entry.o
	$(CC) $(CFFLAGS) $^ -o $@

classpath_test.o: ./classpath/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

classpath.o: ./classpath/classpath.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

entry.o: ./classpath/entry.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

# classfile
classfile_test: utils.o attribute_info.o class_reader.o constant_pool.o member_info.o constant_info.o classfile.o classfile_test.o
	$(CC) $(CFFLAGS) $^ -o $@

classfile_test.o: ./classfile/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@
classfile.o: ./classfile/classfile.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

class_reader.o: ./classfile/class_reader.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

constant_pool.o: ./classfile/constant_pool.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

member_info.o: ./classfile/member_info.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

attribute_info.o: ./classfile/attribute_info.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

constant_info.o: ./classfile/constant_info.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

clean: 
	rm *.o
	rm *_test