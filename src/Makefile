CC = g++
FLAGS = -std=c++17


all: main.cpp extract.cpp language.cpp text_decoder.cpp level.cpp textures.cpp
	@$(CC) main.cpp extract.cpp language.cpp text_decoder.cpp level.cpp textures.cpp $(FLAGS) -static -o extract

clean:
	@rm extract
