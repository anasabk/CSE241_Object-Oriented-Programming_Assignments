target:	SoloS_game

SoloS_lib:	SoloS_lib.hpp SoloS_lib.cpp
			g++ -std=c++11 -c SoloS_lib.cpp


SoloS_game: SoloS_lib
		g++ SoloS_lib.o -o SoloS_game

clean:
		rm SoloS_game