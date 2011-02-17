# GNU (everywhere)
CC = gcc -g -Wall

# GNU (faster)
#CPP = gcc -O5 -Wall -fomit-frame-pointer -ffast-math 

UNAME=$(shell uname)

EXECUTABLE = openglframework
OBJS = openglframework.o

ifeq ($(UNAME),Darwin)
	LIBS = -lm -framework OpenGL -framework GLUT
else
	ifeq ($(findstring MINGW,$(UNAME)),MINGW)
		CC += -Iincludes -DNEED_GLEW -DGLEW_STATIC # You might need glew on MingW
		OBJS += glew.o
		LIBS = -lm -lopengl32 -lglu32 -Llibs -lfreeglut
	else
		LIBS = -lm -lGL -lglut -lGLU
	endif
endif

### TARGETS

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

depend: make.dep

clean:
	- /bin/rm -f  *.bak *~ $(OBJS) $(EXECUTABLE) $(EXECUTABLE).exe

make.dep:
	$(CC) -MM $(OBJS:.o=.c) > make.dep

### RULES

.SUFFIXES: .c .cpp .o .in .ppm .png .jpg .gif

.cpp.o:
	$(CC) -c $<

.c.o:
	$(CC) -c $<

### DEPENDENCIES

include make.dep
