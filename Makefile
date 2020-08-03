OBJ = main.o
INC = -I "./"

rt: $(OBJ)
	mpiCC $(OBJ) -g -Wall -lm -o trace_rays
	rm -f $(OBJ)

main.o:
	mpiCC -c main.cpp $(INC)

clean:
	rm -f $(OBJ) rt