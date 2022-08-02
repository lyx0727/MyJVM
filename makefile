CC = g++
CFFLAGS = -lm -Wall -g -O0
# dependency
CMD_DEP = cmd.o
CLASSPATH_DEP = entry.o classpath.o
CLASSFILE_DEP = attribute_info.o class_reader.o constant_pool.o member_info.o constant_info.o classfile.o
RTDA_DEP = jvm_stack.o thread.o frame.o
INSTRUCTION_DEP = interpreter.o instruction.o
HEAP_DEP = class.o class_loader.o class_constant_pool.o class_member.o sym_ref.o descriptor_parser.o object.o

jvm: utils.o main.o $(CMD_DEP) $(CLASSPATH_DEP) $(CLASSFILE_DEP) $(RTDA_DEP) $(INSTRUCTION_DEP) $(HEAP_DEP)
	$(CC) $(CFFLAGS) $^ -o $@	

utils.o: utils/utils.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

main.o: main.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

# cmd
cmd_test: utils.o cmd_test.o $(CMD_DEP)
	$(CC) $(CFFLAGS) $^ -o $@

cmd_test.o: ./cmd/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

cmd.o: ./cmd/cmd.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

# classpath
classpath_test: utils.o classpath_test.o $(CLASSPATH_DEP)
	$(CC) $(CFFLAGS) $^ -o $@

classpath_test.o: ./classpath/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

classpath.o: ./classpath/classpath.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

entry.o: ./classpath/entry.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

# classfile
classfile_test: utils.o  classfile_test.o $(CLASSFILE_DEP)
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

# runtime data area
rtda_test: rtda_test.o utils.o $(RTDA_DEP)
	$(CC) $(CFFLAGS) $^ -o $@

rtda_test.o: rtda/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

jvm_stack.o: rtda/jvm_stack.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

thread.o: rtda/thread.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

frame.o: rtda/frame.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

# heap
heap_test: heap_test.o utils.o $(HEAP_DEP) $(CLASSFILE_DEP) $(CLASSPATH_DEP) $(RTDA_DEP) $(INSTRUCTION_DEP)
	$(CC) $(CFFLAGS) $^ -o $@
heap_test.o: heap/test.cpp 
	$(CC) $(CFFLAGS) -c $^ -o $@

class.o: heap/class.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

class_member.o: heap/class_member.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

sym_ref.o: heap/sym_ref.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@


class_loader.o: heap/class_loader.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

class_constant_pool.o: heap/constant_pool.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

descriptor_parser.o: heap/descriptor_parser.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

object.o: heap/object.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

# instructions 
instruction_test: instruction_test.o utils.o $(INSTRUCTION_DEP) $(CLASSPATH_DEP) $(CLASSFILE_DEP) $(RTDA_DEP) $(HEAP_DEP)
	$(CC) $(CFFLAGS) $^ -o $@
instruction_test.o: instruction/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

instruction.o: instruction/instruction.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

interpreter.o: instruction/interpreter.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

# test
t: test.o
	$(CC) $(CFFLAGS) $^ -o $@
test.o: test/test.cpp
	$(CC) $(CFFLAGS) -c $^ -o $@

clean: 
	rm *.o
	rm *_test