# OBJECT FILES
MAIN_OBJ   = main.o
PARSER_OBJ = ./opts/getopts.o
SEARCH_OBJ = ./searchingAlg/binarySearch.o
SORT_OBJS  = ./sortingAlg/quicksort.o ./sortingAlg/tablesort.o
TMPL_OBJ   = ./templates/list.o
UTILS_OBJ  = ./utils/utils.o ./utils/predicates.o
OBJECTS    = $(MAIN_OBJ) $(PARSER_OBJ) $(SEARCH_OBJ) $(SORT_OBJS) $(TMPL_OBJ) $(UTILS_OBJ)

# SOURCES
MAIN_SRC   = main.cpp
PARSER_SRC = ./opts/getopts.cpp
SEARCH_SRC = ./searchingAlg/binarySearch.cpp
SORT_SRCS  = ./sortingAlg/quicksort.cpp ./sortingAlg/tablesort.cpp
TMPL_SRC   = ./templates/list.cpp
UTILS_SRC  = ./sortingAlg/quicksort.cpp ./searchingAlg/binarySearch.cpp ./utils/predicates.cpp
SOURCES    = $(MAIN_SRCS) $(PARSER_SRC) $(SEARCH_SRC) $(SORT_SRCS) $(TMPL_SRC) $(UTILS_SRC)

# HEADERS
RELS_HDR   = relationStructs.hpp
PARSER_HDR = ./opts/getopts.hpp
SEARCH_HDR = ./searchingAlg/binarySearch.hpp
SORT_HDRS  = ./sortingAlg/quicksort.hpp ./sortingAlg/tablesort.hpp
TMPL_HDR   = ./templates/list.hpp
UTILS_HDR  = ./sortingAlg/quicksort.hpp ./searchingAlg/binarySearch.hpp ./utils/predicates.hpp
HEADERS    = $(PARSER_HDR) $(SEARCH_HDR) $(SORT_HDRS) $(TMPL_HDR) $(UTILS_HDR)

# EXECUTABLES
MAIN_OUT   = main

# FLAGS
CC         = g++
FLAGS      = -c
DEBUG      = -g3

# ------------------------------------------------- #

all: $(OBJECTS) $(MAIN_OUT)

# Creating Executables
main : $(OBJECTS)
	@echo "\n\033[1;33mCompiling executables\033[0m";
	$(CC) $(DEBUG) -o $(MAIN_OUT) $(OBJECTS)

# ------------------------------------------------- #

# Creating Object Files
main.o : main.cpp
	@echo "\n\033[1;33mCompiling objects\033[0m";
	$(CC) $(DEBUG) -o main.o                        $(FLAGS) main.cpp

./opts/getopts.o : ./opts/getopts.cpp
	$(CC) $(DEBUG) -o ./opts/getopts.o              $(FLAGS) ./opts/getopts.cpp

./templates/list.o : ./templates/list.cpp
	$(CC) $(DEBUG) -o ./templates/list.o            $(FLAGS) ./templates/list.cpp

./utils/utils.o : ./utils/utils.cpp
	$(CC) $(DEBUG) -o ./utils/utils.o               $(FLAGS) ./utils/utils.cpp

./utils/predicates.o : ./utils/predicates.cpp
	$(CC) $(DEBUG) -o ./utils/predicates.o          $(FLAGS) ./utils/predicates.cpp

./sortingAlg/quicksort.o : ./sortingAlg/quicksort.cpp
	$(CC) $(DEBUG) -o ./sortingAlg/quicksort.o      $(FLAGS) ./sortingAlg/quicksort.cpp

./sortingAlg/tablesort.o : ./sortingAlg/tablesort.cpp
	$(CC) $(DEBUG) -o ./sortingAlg/tablesort.o      $(FLAGS) ./sortingAlg/tablesort.cpp

./searchingAlg/binarySearch.o : ./searchingAlg/binarySearch.cpp
	$(CC) $(DEBUG) -o ./searchingAlg/binarySearch.o $(FLAGS) ./searchingAlg/binarySearch.cpp

# ------------------------------------------------- #

# Cleaning Files
clean:
	rm -f $(MAIN_OUT) $(OBJECTS)

# ------------------------------------------------- #

# Counting Files
count:
	wc $(SOURCES) $(HEADERS)
