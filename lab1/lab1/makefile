# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab1-1 lab1-2 lab1-3

mac: lab1-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab1-1 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab1-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL

linux : lab1-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab1-1 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab1-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

lab1-1 : lab1-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab1-1 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab1-1.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL

lab1-2 : lab1-2.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
		gcc -Wall -o lab1-2 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab1-2.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL

lab1-3 : lab1-3.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
				gcc -Wall -o lab1-3 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab1-3.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL

lab1-4 : lab1-4.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
				gcc -Wall -o lab1-4 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab1-4.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL

lab1-5 : lab1-5.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
				gcc -Wall -o lab1-5 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab1-5.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework Cocoa -framework OpenGL

linux 3: lab1-3.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
				gcc -Wall -o lab1-3 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab1-3.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

linux 4: lab1-4.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
				gcc -Wall -o lab1-4 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab1-4.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

linux 5: lab1-5.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
				gcc -Wall -o lab1-5 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab1-5.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm


clean :
	rm lab1-1
	rm lab1-2
	rm lab1-3
	rm lab1-4
	rm lab1-5
