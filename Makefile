CC=g++-12
FLAG=--std=c++11 -g

OBJ = Main.o token.o 
Main: $(OBJ)
	$(CC) $(FLAG) $(OBJ) -o Main

.cpp.o: 
	$(CC) $(FLAG) -c $<

Main.cpp: lex.h

token.cpp: token.h

test: 1.txt 2.txt Main
	./Main 1.txt && ./Main 2.txt

clean:
	rm -f *.o Main
