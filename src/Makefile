CC = g++
FLAGS = -std=c++17
CPP_FILES = main.cpp extract.cpp language.cpp text_decoder.cpp level.cpp textures.cpp

all: $(CPP_FILES)
	@$(CC) $(FLAGS) $(CPP_FILES) -static -o extract

clean:
	@rm extract
