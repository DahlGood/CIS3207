#Compiler
CC = g++

#Compiler flags
#compiling with -std=c++11 because enhanced for loops arent available in the default c++98 compiler
FLAGS = -Wall -Werror -std=c++11

FILES = Shell.cpp BuiltInCommands.cpp CommandParser.cpp CommandProcessor.cpp

OUTPUT = Shell

all: $(OUTPUT)

$(OUTPUT): $(FILES)
	$(CC) $(FLAGS) $(FILES) -o $(OUTPUT)