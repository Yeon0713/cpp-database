FLAGS = -Werror -cpp

main.exe: main.o Database_System.cpp
	g++ ${FLAGS} -o main.exe Database_System.o

Database_System.o: Database_System.cpp
	g++ ${FLAGS} Database_System.cpp