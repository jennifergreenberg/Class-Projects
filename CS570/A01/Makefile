# Bryant To, Jessica To
# CS570 - Operating Systems A01

myls: UnixLS.o myls.o
	g++ UnixLS.o myls.o -o myls
UnixLS.o: UnixLS.h UnixLS.cpp
	g++ -c UnixLS.cpp
myls.o: myls.cpp
	g++ -c myls.cpp
