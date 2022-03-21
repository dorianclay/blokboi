CXX		= g++ -std=c++11
CXXFLAGS	= -g -Wall
OBJS		= game.o scene.o block.o player.o
PROG		= blokboi

all:		$(PROG)

$(PROG):	$(OBJS)
		$(CXX) -o $(PROG) $(OBJS)

clean:;		$(RM) $(PROG) core *.o
