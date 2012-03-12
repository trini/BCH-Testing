CC = gcc
OPTFLAGS = -O3
DBGFLAGS = -O0 -g
CFLAGS = -Wall $(OPTFLAGS) -Iinclude -DCONFIG_BCH_CONST_PARAMS \
	 -DCONFIG_BCH_CONST_M=13 -DCONFIG_BCH_CONST_T=8
COMMON_OBJS = bch.o tester.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

all: bch_sw

bch_sw: $(COMMON_OBJS)
	$(CC) -o $@ $(COMMON_OBJS)

clean:
	rm -f bch_sw *.o
