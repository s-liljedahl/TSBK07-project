# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab4-1 lab4-2

lab4-1 : lab4-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4-1 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab4-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

lab4-2 : lab4-2.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4-2 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab4-2.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

lab4-3 : lab4-3.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4-3 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab4-3.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

lab4-4 : lab4-4.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4-4 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab4-4.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

lab4-5 : lab4-5.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4-5 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab4-5.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

lab4-6 : lab4-6.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4-6 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab4-6.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm



mac 1 : lab4-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab4-1 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab4-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL -lm

mac 2 : lab4-2.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab4-2 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab4-2.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL -lm

mac 3 : lab4-3.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab4-3 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab4-3.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL -lm

mac 4 : lab4-4.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab4-4 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab4-4.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL -lm

mac 5 : lab4-5.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
		gcc -Wall -o lab4-5 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab4-5.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL -lm

mac 6 : lab4-6.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
		gcc -Wall -o lab4-6 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab4-6.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL -lm



clean :
	rm lab4-1
	rm lab4-2
	rm lab4-3
	rm lab4-4
	rm lab4-5
