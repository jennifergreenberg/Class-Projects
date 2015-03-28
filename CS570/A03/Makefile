# Bryant To, Jessica To
# CS570 - Operating Systems A03

main: producer.o consumer.o main.o
	g++ producer.o consumer.o main.o -o main -lpthread -lrt
producer.o: main.h producer.cpp
	g++ -c producer.cpp
consumer.o: main.h consumer.cpp
	g++ -c consumer.cpp
main.o: main.h main.cpp
	g++ -c main.cpp 