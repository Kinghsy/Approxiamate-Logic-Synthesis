#----------------------------------------------------------------------
# Makefile for C++ program that uses cudd for class ECE571
#
# Last modified on 1/23/02
#----------------------------------------------------------------------

# compiler
#CPP	= g++
CPP     = gcc

RANLIB	= ranlib
CFLAGS  = -g -O6
DDDEBUG = 

# include files and library
PDIR    = /home/wuyi/usr/CUDD/cudd-2.5.0
INCLUDE = $(PDIR)/include

# for c++
#LIBS	= $(PDIR)/obj/libobj.a $(PDIR)/cudd/libcudd.a $(PDIR)/mtr/libmtr.a \
	  $(PDIR)/st/libst.a $(PDIR)/util/libutil.a $(PDIR)/epd/libepd.a
#for c
LIBS	= $(PDIR)/cudd/libcudd.a $(PDIR)/mtr/libmtr.a $(PDIR)/dddmp/libdddmp.a\
	  $(PDIR)/st/libst.a $(PDIR)/util/libutil.a $(PDIR)/epd/libepd.a	   	  
	  
# source files
# for c++
#SRC     = tt.cc
#OBJ	= $(SRC:.cc=.o)
#TARGET  = $(SRC:.cc=) 
#for c
SRC     = exer1.c
OBJ	= $(SRC:.c=.o)
TARGET  = $(SRC:.c=) 


# link files for building BDDs from BLIF files
BDIR    = /home/wuyi/usr/CUDD/cudd-2.5.0/blif
BLIF    = $(BDIR)/bnet.o $(BDIR)/ntr.o $(BDIR)/ntrHeap.o $(BDIR)/ntrBddTest.o \
          $(BDIR)/ntrMflow.o $(BDIR)/ntrZddTest.o \
	  $(BDIR)/ntrShort.o $(BDIR)/chkMterm.o $(BDIR)/ucbqsort.o $(BDIR)/blif.o
# There is no need to link the above programs if you do not build 
# BDD from blif. Thus, use the following configuration for BLIF
#BLIF =	 


# make executable

#for c++
#.cc.o: $(SRC)
#	$(CPP) -c $< -I$(INCLUDE) $(CFLAGS) $(DDDEBUG)
	
#for c
.c.o: $(SRC)
	$(CPP) -c $< -I$(INCLUDE) $(CFLAGS) $(DDDEBUG)
	
$(TARGET): $(SRC) $(OBJ)
	$(CPP) $(CFLAGS) -o $@ $(OBJ) $(BLIF) $(LIBS) -lm

clean:
	rm -f $(OBJ) $(TARGET)
