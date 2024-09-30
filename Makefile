CXX = g++
CXXFLAGS = -std=c++17 #-Wall -Wextra

SRCS = sorting.cpp splay.cpp dynamic_connectivity.cpp link_cut_tree.cpp redblue.cpp white_components.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = sorting
OUTPUT = output.txt

.PHONY: all clean distclean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean:
	rm -vf *.o 

distclean: clean
	rm -vf $(EXEC)
