CC=g++-12
FLAG=--std=c++11 -g

OBJ = Main.o token.o 
HEAD = syntax.h lex.h tree.h intermediate.h
Main: $(OBJ) $(HEAD)
	$(CC) $(FLAG) $(OBJ) -o Main

.cpp.o: 
	$(CC) $(FLAG) -c $<

token.cpp: token.h

Main.cpp: $(HEAD)

test: Main test.sh
	bash test.sh

clean:
	rm -f *.o Main
