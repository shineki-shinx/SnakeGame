LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
OPENGL=-lGL -lGLU -lglut -lm 
all: output

main.o: main.cpp 
	g++ -c  main.cpp  
output: main.o
	g++ -o output main.o include/*.cpp $(LIBS) $(OPENGL)
clean:
	-rm -f 	*.o 
