
CXX = g++
OBJ_FILES_CPPLOX = ast.o callable.o cpplox.o environment.o interpreter.o parser.o scanner.o token.o semantic_analysis.o
OBJ_FILES_LOX = ast.o callable.o lox.o environment.o interpreter.o parser.o scanner.o token.o semantic_analysis.o
CXXFLAGS = -g


cpplox: ${OBJ_FILES}
	${CXX} -o cpplox ${OBJ_FILES}


lox: ${OBJ_FILES_LOX}
	${CXX} -o lox ${OBJ_FILES_LOX}



clean:
	rm *.o
	rm ${TARGET}

