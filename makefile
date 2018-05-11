crypter: crypter.o
	g++ -o crypter crypter.o -std=c++11
crypter.o: crypter.cpp
	g++ -c crypter.cpp -Wall -std=c++11
