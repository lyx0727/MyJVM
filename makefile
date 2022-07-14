CC = g++
CFFLAGS = -lm -Wall -g
jvm: main.cpp ./cmd/*.cpp ./classpath/*.cpp ./test/*.cpp
	$(CC) $(CFFLAGS) main.cpp ./cmd/*.cpp ./classpath/*.cpp ./test/*.cpp -o jvm	

clean: 
	rm *.o