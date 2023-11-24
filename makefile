# Example 38
EXE=archipelago

PATCH = patchtest

# Main target
all: $(EXE)

# Perlin target
# perlin: perlin.cpp
# 	g++ -c $(CFLG) -o perlin.o $< -std=c++17

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW -Wno-comment
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarationsg++  -Wno-comment -DRES=$(RES)
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall -Wno-comment
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Main Functions
archipelago.o: archipelago.cpp archlib.h
patchtest.o: patch_terrain_test.cpp archlib.h


# Dependencies -- Utility Functions
fatal.o: fatal.cpp archlib.h
errcheck.o: errcheck.cpp archlib.h
print.o: print.cpp archlib.h
loadtexbmp.o: loadtexbmp.cpp archlib.h
loadobj.o: loadobj.cpp archlib.h
projection.o: projection.cpp archlib.h
transform.o: transform.cpp archlib.h
perlin.o: perlin.cpp archlib.h

# Dependencies -- Objects
flyer.o: flyer.cpp transform.cpp color.cpp drawpoly.cpp archlib.h

#  Create archive
archlib.a:fatal.o errcheck.o print.o loadtexbmp.o loadobj.o projection.o color.o transform.o drawpoly.o perlin.o flyer.o
	ar -rcs $@ $^

# Compile rules
.cpp.o:
	g++ -c $(CFLG)  $< -std=c++20
.c.o:
	gcc -c $(CFLG)  $< -std=c++20


#  Link
archipelago: archipelago.cpp archlib.a
	g++ $(CFLG) -o $@ $^  $(LIBS) -std=c++20


patchtest: patch_terrain_test.cpp archlib.a
	g++ $(CFLG) -o $@ $^  $(LIBS) -std=c++20

#  Clean
clean:
	$(CLEAN)
