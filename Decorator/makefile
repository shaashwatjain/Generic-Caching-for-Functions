OBJECT_FILES=client.o component.o concrete_component.o concrete_decorator1.o concrete_decorator.o decorator.o

all: $(OBJECT_FILES)

# Define a pattern rule that compiles every .c file into a .o file
%.o : %.cpp
	g++ -c -std=c++2a -Wall $< -o $@