# Archivo: Makefile
# Autor: Cruz, Kouichi Julian Andres
TARGET=main
CC=gcc
CXX=g++
CLANG=clang++
RM=rm -f

SRCS = $(wildcard *.cpp)
SRCS := $(filter-out main.cpp, $(SRCS))
HDRS = $(wildcard *.hpp)
MAIN = main.cpp
TESTS = test_compression.cc
OBJS = $(SRCS:.cpp=.o)
FLAGS= -std=c++11 -lgdal -g
DEBUGFLAGS = -DDEBUG

all:
	$(CXX) $(SRCS) $(MAIN) $(FLAGS) -o $(TARGET) $(OBJECTS)

cla:
	$(CLANG) $(SRCS) $(MAIN) $(FLAGS) -o $(TARGET) $(OBJECTS)
	
test:
	$(CXX) $(SRCS) $(TESTS) $(FLAGS) -L/usr/lib -lcmocka -o tests $(OBJECTS)
	
debug:
	$(CXX) $(SRCS) $(MAIN) $(FLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS)

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)
	