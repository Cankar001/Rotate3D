OBJ=.obj/
PROT=.proto
PROTO=MakeProto $< >$(PROT)/`basename $< .C`.proto ; cat $(PROT)/*.proto >Proto.h
OPTIM=-O0 -g
CFLAGS=-Wall -include Rotate3D.h
CC=$(PROTO) ; g++ $(CFLAGS) $(OPTIM)
LIBS=-L/usr/X11R6/lib -lXt -lXext -lX11 -lglut -lGL -lGLU -lm

FILES=$(OBJ)Gfx.o $(OBJ)MathFunctions.o $(OBJ)Matrix.o $(OBJ)OpenGLVertexBuffer.o $(OBJ)OpenGLIndexBuffer.o $(OBJ)Main.o 

run: .proto .obj MakeProto Rotate3D 
	./Rotate3D

clean:
	rm -f Rotate3D MakeProto 
	rm -rf $(OBJ)
	rm -rf $(PROT)

# Alle Files hängen vom globalen Header ab:
$(FILES): Rotate3D.h

$(OBJ)Gfx.o: Gfx.C
	$(CC) -c -o $@ $<

$(OBJ)Main.o: Main.C
	$(CC) -c -o $@ $<

$(OBJ)MathFunctions.o: MathFunctions.C
	$(CC) -c -o $@ $<

$(OBJ)Matrix.o: Matrix.C
	$(CC) -c -o $@ $<

$(OBJ)OpenGLVertexBuffer.o: OpenGLVertexBuffer.C
	$(CC) -c -o $@ $<

$(OBJ)OpenGLIndexBuffer.o: OpenGLIndexBuffer.C
	$(CC) -c -o $@ $<

MakeProto: MakeProto.c
	gcc -o MakeProto MakeProto.c
	strip MakeProto

.obj:
	mkdir .obj

.proto:
	mkdir .proto

Rotate3D: $(FILES)
	g++ $(CFLAGS) $(OPTIM) -o $@ $(FILES) $(LIBS)
