CC = g++
CFFLAGS = -lm -Wall -g
jvm: main.cpp ./cmd/cmd.cpp
	$(CC) $(CFFLAGS) main.cpp ./cmd/cmd.cpp -o jvm	

clean: 
	rm *.o