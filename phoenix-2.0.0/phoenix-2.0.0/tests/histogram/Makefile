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

HIST_OBJS = histogram.o
HIST_SEQ_OBJS = histogram-seq.o
HIST_PTHREAD_OBJS = histogram-pthread.o

PROGS = histogram histogram-seq_O0 histogram-pthread_O0 histogram-seq_O1 histogram-pthread_O1 histogram-seq_O2 histogram-pthread_O2 histogram-seq_O3 histogram-pthread_O3

.PHONY: default all clean

default: all

all: $(PROGS)

histogram: $(HIST_OBJS) $(LIB_DEP)
	$(CC) $(CFLAGS) -o $@ $(HIST_OBJS) $(LIBS)

histogram-seq_O0: $(HIST_SEQ_OBJS)
	$(CC) $(CFLAGS) -o $@ $(HIST_SEQ_OBJS) $(LIBS)

histogram-pthread_O0: $(HIST_PTHREAD_OBJS)
	$(CC) $(CFLAGS) -o $@ $(HIST_PTHREAD_OBJS) $(LIBS)
	
histogram-seq_O1: $(HIST_SEQ_OBJS)
	$(CC) -O1 $(CFLAGS) -o $@ $(HIST_SEQ_OBJS) $(LIBS)

histogram-pthread_O1: $(HIST_PTHREAD_OBJS)
	$(CC) -O1 $(CFLAGS) -o $@ $(HIST_PTHREAD_OBJS) $(LIBS)
	
histogram-seq_O2: $(HIST_SEQ_OBJS)
	$(CC) -O2 $(CFLAGS) -o $@ $(HIST_SEQ_OBJS) $(LIBS)

histogram-pthread_O2: $(HIST_PTHREAD_OBJS)
	$(CC) -O2 $(CFLAGS) -o $@ $(HIST_PTHREAD_OBJS) $(LIBS)
	
histogram-seq_O3: $(HIST_SEQ_OBJS)
	$(CC) -O3 $(CFLAGS) -o $@ $(HIST_SEQ_OBJS) $(LIBS)

histogram-pthread_O3: $(HIST_PTHREAD_OBJS)
	$(CC) -O3 $(CFLAGS) -o $@ $(HIST_PTHREAD_OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(HOME)/$(INC_DIR)

clean:
	rm -f $(PROGS) $(HIST_OBJS) $(HIST_SEQ_OBJS) $(HIST_PTHREAD_OBJS)
