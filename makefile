# Example 38
EXE=archipelago

# Main target
all: $(EXE)

# Perlin target
perlin: perlin.cpp
	g++ $(CFLG) -o perlin $^  $(LIBS) -std=c++17

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarationsg++  -DRES=$(RES)
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Main Functions
archipelago.o: archipelago.cpp archlib.h
perlin.o: perlin.cpp archlib.h

# Dependencies -- Utility Functions
fatal.o: fatal.cpp archlib.h
errcheck.o: errcheck.cpp archlib.h
print.o: print.cpp archlib.h
loadtexbmp.o: loadtexbmp.cpp archlib.h
loadobj.o: loadobj.cpp archlib.h
projection.o: projection.cpp archlib.h
transform.o: transform.cpp archlib.h

# Dependencies -- Objects
cube.o: cube.cpp transform.cpp color.cpp drawpoly.cpp archlib.h
cylinder.o: cylinder.cpp transform.cpp color.cpp drawpoly.cpp archlib.h
torus.o: torus.cpp transform.cpp color.cpp drawpoly.cpp archlib.h

#  Create archive
archlib.a:fatal.o errcheck.o print.o loadtexbmp.o loadobj.o projection.o cube.o cylinder.o torus.o color.o transform.o drawpoly.o
	ar -rcs $@ $^

# Compile rules
.cpp.o:
	g++ -c $(CFLG)  $< 
.c.o:
	gcc -c $(CFLG)  $< 


#  Link
archipelago:archipelago.o   archlib.a
	gcc $(CFLG) -o $@ $^  $(LIBS)

#  Clean
clean:
	$(CLEAN)
