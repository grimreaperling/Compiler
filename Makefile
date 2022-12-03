CC=g++-12
FLAG=--std=c++11 -g

OBJ = Main.o token.o 
Main: $(OBJ)
	$(CC) $(FLAG) $(OBJ) -o Main

.cpp.o: 
	$(CC) $(FLAG) -c $<

Main.cpp: lex.h

token.cpp: token.h

test: Main test.sh
	bash test.sh

clean:
	rm -f *.o Main
