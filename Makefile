all:kseq.h kseqStat.c
		$(CC) -g -O2 kseqStat.c -o kseqStat -lz

clean:
		rm -f *.o kseqStat
