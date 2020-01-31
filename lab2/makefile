# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab2-1 

linux : lab2-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab2-1 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab2-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

lab1-1 : lab2-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab2-1 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab2-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL

clean :
	rm lab2-1