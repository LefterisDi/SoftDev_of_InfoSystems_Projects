# OBJECT FILES
MAIN_OBJS    = ./main.o
UTILS_OBJS = ./sortingAlg/quicksort.o ./searchingAlg/binarySearch.o
OBJECTS    = $(MAIN_OBJS) $(UTILS_OBJS)

# SOURCES
MAIN_SRCS    = ./main.cpp
UTILS_SRCS = ./sortingAlg/quicksort.cpp ./searchingAlg/binarySearch.cpp
SOURCES    = $(MAIN_SRCS) $(UTILS_SRCS)

# EXECUTABLES
MAIN_OUT    = main

# FLAGS
CC         = g++ -Wall #-Wno-unused-variable
FLAGS      = -c
DEBUG      = -g3

# ------------------------------------------------- #

all: $(OBJECTS) $(MAIN_OUT)

# Creating Executables
main : $(MAIN_OBJS) $(UTILS_OBJS)
	@echo "\n\033[1;33mCompiling executables\033[0m";
	$(CC) $(DEBUG) -o $(MAIN_OUT) $^
	@echo "";

# ------------------------------------------------- #

# Creating Object Files
./main.o : ./main.cpp
	@echo "\n\033[1;33mCompiling objects\033[0m";
	$(CC) $(DEBUG) -o ./main.o $(FLAGS) ./main.cpp
	
./sortingAlg/quicksort.o: ./sortingAlg/quicksort.cpp
	$(CC) $(DEBUG) -o ./sortingAlg/quicksort.o $(FLAGS) ./sortingAlg/quicksort.cpp
	
./searchingAlg/binarySearch.o: ./searchingAlg/binarySearch.cpp
	$(CC) $(DEBUG) -o ./searchingAlg/binarySearch.o $(FLAGS) ./searchingAlg/binarySearch.cpp

# ------------------------------------------------- #

# Cleaning Files
clean:
	rm -f $(MAIN_OUT) $(OBJECTS)

# ------------------------------------------------- #

# Counting Files
count:
	wc $(SOURCES)
