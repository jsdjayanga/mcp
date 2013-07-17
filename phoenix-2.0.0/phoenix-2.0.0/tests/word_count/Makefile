#------------------------------------------------------------------------------
# Copyright (c) 2007-2009, Stanford University
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Stanford University nor the names of its 
#       contributors may be used to endorse or promote products derived from 
#       this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#------------------------------------------------------------------------------ 

# This Makefile requires GNU make.

HOME = ../..

include $(HOME)/Defines.mk

LIBS += -L$(HOME)/$(LIB_DIR) -l$(PHOENIX)

WC_OBJS := word_count.o sort.o
WC_OBJS_PTHREAD := word_count-pthread.o sort-pthread.o
WC_OBJS_SEQ := word_count-seq.o

PROGS := word_count word_count-pthread_O0 word_count-seq_O0 word_count-pthread_O1 word_count-seq_O1 word_count-pthread_O2 word_count-seq_O2 word_count-pthread_O3 word_count-seq_O3

.PHONY: default all clean

default: all

all: $(PROGS)

word_count: $(WC_OBJS) $(LIB_DEP)
	$(CC) $(CFLAGS) -o $@ $(WC_OBJS) $(LIBS)

word_count-pthread_O0: $(WC_OBJS_PTHREAD)
	$(CC) -O0 $(CFLAGS) -o $@ $(WC_OBJS_PTHREAD) $(LIBS)
	
word_count-seq_O0: $(WC_OBJS_SEQ)
	$(CC) -O0 $(CFLAGS) -o $@ $(WC_OBJS_SEQ) $(LIBS)
	
word_count-pthread_O1: $(WC_OBJS_PTHREAD)
	$(CC) -O1 $(CFLAGS) -o $@ $(WC_OBJS_PTHREAD) $(LIBS)
	
word_count-seq_O1: $(WC_OBJS_SEQ)
	$(CC) -O1 $(CFLAGS) -o $@ $(WC_OBJS_SEQ) $(LIBS)
	
word_count-pthread_O2: $(WC_OBJS_PTHREAD)
	$(CC) -O2 $(CFLAGS) -o $@ $(WC_OBJS_PTHREAD) $(LIBS)
	
word_count-seq_O2: $(WC_OBJS_SEQ)
	$(CC) -O2 $(CFLAGS) -o $@ $(WC_OBJS_SEQ) $(LIBS)
	
word_count-pthread_O3: $(WC_OBJS_PTHREAD)
	$(CC) -O3 $(CFLAGS) -o $@ $(WC_OBJS_PTHREAD) $(LIBS)
	
word_count-seq_O3: $(WC_OBJS_SEQ)
	$(CC) -O3 $(CFLAGS) -o $@ $(WC_OBJS_SEQ) $(LIBS)

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@ -I$(HOME)/$(INC_DIR)

clean:
	rm -f $(PROGS) $(WC_OBJS) $(WC_OBJS_PTHREAD) $(WC_OBJS_SEQ)
