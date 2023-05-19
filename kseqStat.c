#include <zlib.h>
#include <stdio.h>
#include "kseq.h"
KSEQ_INIT(gzFile, gzread);
void stats(gzFile fq, long int* total_reads, long int* total_bases, long int* Q20Base, long int* Q30Base);
const char q0 = '!';
const char q20 = '5';
const char q30 = '?';

int main(int argc, char *argv[])
{
	long int total_reads = 0, Q20Base = 0, Q30Base = 0;
	long int total_bases = 0;
	gzFile fq1, fq2;
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <in.R1.fq.gz> <in.R2.fq.gz>\n", argv[0]);
		return 1;
	}
	fq1 = gzopen(argv[1], "r");
	fq2 = gzopen(argv[2], "r");
	stats(fq1, &total_reads, &total_bases, &Q20Base, &Q30Base);
	stats(fq2, &total_reads, &total_bases, &Q20Base, &Q30Base);
	printf("Reads\tBases\tQ20\tQ30\n");
	printf("%ld\t%ld\t%.2f%%\t%.2f%%\n",total_reads,total_bases,(Q20Base*100.0)/total_bases, (Q30Base*100.0)/total_bases);
	return 0;
}

void stats(gzFile fq, long int* total_reads, long int* total_bases, long int* Q20Base, long int* Q30Base){
	kseq_t *seq;
	int l;
	seq = kseq_init(fq);
	while ((l = kseq_read(seq)) > 0){
		*total_reads += 1;
		for(int i=0; i < l; i++){
			*total_bases += 1;
			if (seq->qual.s[i] >= q30) {
				*Q20Base += 1;
				*Q30Base += 1;
				/*printf("Q30:%d ", seq->qual.s[i]);*/
			}else if(seq->qual.s[i] >= q20){
				// printf("Q20 value:%d",seq->qual.s[i]);
				*Q20Base += 1;
			}
		}
	}
	kseq_destroy(seq);
	gzclose(fq);
}
