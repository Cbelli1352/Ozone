# Program name
NAME = http
# Compiler command
CC = g++

# The root directory of headers
IDIR = include

# The root directory of the includes
ODIR = obj

# Include Params
IPRMS = $(INC:%=-I%)

# Compile Flags
CFLAGS=-I $(IDIR) -std=c++11 -lpthread -Wall

# Linker Flags
LFLAGS = -pthread

DEPS = $(wildcard $(ODIR)/*.cpp)
OBJECTS = $(DEPS:.cpp=.o)

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $(NAME)

%.o: %.cpp $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

# Cleans up object files
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

