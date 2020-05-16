CC = g++
FLAGS = -lboost_filesystem-mgw92-mt-x32-1_68 -lboost_system-mgw92-mt-x32-1_68

BOOST_PATH = "C:\boost\include\boost-1_68"
BOOST_LINK = "C:\boost\lib"

all: main.cpp extract.cpp language.cpp text_decoder.cpp level.cpp textures.cpp
	@$(CC) main.cpp extract.cpp language.cpp text_decoder.cpp level.cpp textures.cpp -I$(BOOST_PATH) -L$(BOOST_LINK) $(FLAGS) -static -o extract

clean:
	@rm extract