
CXX = g++
OBJ_FILES_ALOX = ast.o lox.o environment.o  parser.o scanner.o token.o semantic_analysis.o
CXXFLAGS = -g

alox: ${OBJ_FILES_ALOX}
	${CXX} -o alox ${OBJ_FILES_ALOX}

clean:
	rm *.o
