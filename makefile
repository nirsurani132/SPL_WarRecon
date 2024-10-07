all: clean compile link cleanObjAndrun

compile:
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/*.cpp

link:
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/simulation *.o

clean:
	rm -f bin/*

cleanObjAndrun:
	rm -f *.o
	valgrind --leak-check=full --show-reachable=yes ./bin/simulation config_file.txt