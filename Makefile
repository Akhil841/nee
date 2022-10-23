all:
	g++ -o nee.o src/lexanalyze.cpp src/parser.cpp src/evaluate.cpp src/main.cpp
clean:
	rm *.o