# set this variable to the director in which you saved the common files
commondir = ../common/

all : main 

main: main.c skybox-loader.c terrain.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o main -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES main.c skybox-loader.c terrain.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL

clean :
	rm main

