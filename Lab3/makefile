#Compiler
CC = g++

#Compiler flags
#compiling with -std=c++11 because enhanced for loops arent available in the default c++98 compiler <--- Used in Lab 2, just keeping this.
FLAGS = -std=c++11 -lpthread -Wall -Werror

FILES = Server.h Server.cpp SpellChecker.cpp circular_buffer.cpp

OUTPUT = Server

all: $(OUTPUT)

$(OUTPUT): $(FILES)
	$(CC) $(FILES) -o $(OUTPUT) $(FLAGS)