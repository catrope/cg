############################################################################
# Makefile for the raytracer framwork created for the Computer Science 
# course "Introduction Computer Graphics" taught at the University of 
# Groningen by Tobias Isenberg.
############################################################################

### MACROS

# GNU (everywhere)
CPP = g++ -g -Wall

# GNU (faster)
#CPP = g++ -O5 -Wall -fomit-frame-pointer -ffast-math 

LIBS = -lm

EXECUTABLE = ray

OBJS = main.o raytracer.o sphere.o light.o material.o \
	image.o triple.o lodepng.o scene.o

YAMLOBJS = $(subst .cpp,.o,$(wildcard yaml/*.cpp))

IMAGES = $(subst .yaml,.png,$(wildcard *.yaml))


### TARGETS

$(EXECUTABLE): $(OBJS) $(YAMLOBJS)
	$(CPP) $(OBJS) $(YAMLOBJS) $(LIBS) -o $@

run: $(IMAGES)

%.png: %.yaml $(EXECUTABLE)
	./$(EXECUTABLE) $<

depend: make.dep

clean:
	- /bin/rm -f  *.bak *~ $(OBJS) $(YAMLOBJS) $(EXECUTABLE) $(EXECUTABLE).exe

make.dep:
	gcc -MM $(OBJS:.o=.cpp) > make.dep

### RULES

.SUFFIXES: .cpp .o .yaml .png

.cpp.o:
	$(CPP) -c -o $@ $<

### DEPENDENCIES

include make.dep
