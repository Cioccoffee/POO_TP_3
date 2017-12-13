#makefile pour compilation et reliure

#variables
RM = rm
COMP = g++
EDL = g++

INT = ListeTrajets.h Trajet.h TrajetSimple.h TrajetCompose.h Test.h

REAL=$(INT:.h=.cpp)
OBJ=$(REAL:.cpp=.o)

EXE = executable

RMFLAGS = -f
COMPFLAGS = -ansi -pedantic -Wall -std=c++11
EDLFLAGS = 
CLEAN = clean

#useless
#$(REAL) : $(INT)
#$(OBJ) : $(REAL)

#realisation de l'executable
$(EXE) : $(OBJ)
	$(EDL) -o $(EXE) $(OBJ)

#pattern pour la reliure
%.o:%.cpp
	$(COMP) -g $(COMPFLAGS) -c $<


$(CLEAN):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core


#regle explicite pour la reliure de l'executable



	

