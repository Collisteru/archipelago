# Example 38
EXE=archipelago

# Main target
all: $(EXE)

# Perlin target
perlin:perlin.o
	gcc $(CFLG) -o $@ $^  $(LIBS)

perlin.o: perlin.c

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=$(RES)
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies -- Utility Functions
archipelago.o: archipelago.c archlib.h
fatal.o: fatal.c archlib.h
errcheck.o: errcheck.c archlib.h
print.o: print.c archlib.h
loadtexbmp.o: loadtexbmp.c archlib.h
loadobj.o: loadobj.c archlib.h
projection.o: projection.c archlib.h

transform.o: transform.c archlib.h

# Dependencies -- Objects
cube.o: cube.c transform.c color.c drawpoly.c archlib.h
cylinder.o: cylinder.c transform.c color.c drawpoly.c archlib.h
torus.o: torus.c transform.c color.c drawpoly.c archlib.h

#  Create archive
archlib.a:fatal.o errcheck.o print.o loadtexbmp.o loadobj.o projection.o cube.o cylinder.o torus.o color.o transform.o drawpoly.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG)  $<
.cpp.o:
	g++ -c $(CFLG)  $<

#  Link
archipelago:archipelago.o   archlib.a
	gcc $(CFLG) -o $@ $^  $(LIBS)

#  Clean
clean:
	$(CLEAN)
