#Makefile for compile the c++ soruce code using opencv  under ubuntu
#
#to avoid using pkg-config opencv --cflags --libs
#
CC = g++
CFLAGS= `pkg-config opencv --cflags --libs`

#segmentG: graph.cpp graph.h MLSmooth.h  image.h filter.h segment-graph.h disjoint-set.h
segmentG: main.cpp graph.h MLSmooth.h  image.h filter.h segment-graph.h disjoint-set.h
	$(CC) *.cpp  $(CFLAGS)   -o segmentG
