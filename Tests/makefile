all: compile link cleanObjAndrun

compile:
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude ./*.cpp

link:
	g++ -g -Wall -Weffc++ -std=c++11 -o tests *.o

cleanObjAndrun:
	rm -f *.o
	./tests