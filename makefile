
CC = g++
OBJ_FILES = ast.o callable.o cpplox.o environment.o interpreter.o parser.o scanner.o token.o 
FLAG = -g
TARGET = cpplox

${TARGET} : ${OBJ_FILES}
	g++ -o ${TARGET} ${OBJ_FILES} ${FLAG}


#No need of the below code, its redundant. Make automatically knows when to rebuild the object file
#ast.o: ast.cpp
#	g++ -c ast.cpp


clean:
	rm *.o
	rm ${TARGET}

