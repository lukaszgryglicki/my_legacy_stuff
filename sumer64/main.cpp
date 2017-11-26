#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Range
{
	unsigned int from, to, sum, num;
} Range;

int n_ranges;
Range* ranges;

void calc_sum(int le, char* fn, Range* r, char* ofn)
{
 FILE *f, *of;
 unsigned int lb;
 unsigned char buf[4];
 unsigned char buf2[4];
 unsigned int l_sum = 0;
 unsigned long len;
 unsigned int times, i;

 f = fopen(fn, "rb");
 if (!f) 
 {
	 printf("Nie moge odczytac pliku: %s\n", fn);
	 return;
 }

 fseek(f, 0, SEEK_END);
 len = ftell(f);
 if (r->from >= len || r->to >= len)
 {
	 printf("Zakres [%08x %08x] wykracza poza wielkosc pliku: %08x\n", r->from, r->to, len);
	 fclose(f);
	 return;
 }

 fseek(f, r->from, SEEK_SET);
 times = (r->to - r->from) / 4;

 for (i=0;i<times;i++)
 {
	 if (fread((void*)(buf), 4, 1, f) != 1)
	 {
		 printf("Blad odczytu bajtow z pliku %d/%d\n", i+1, times);
		 fclose(f);
	 }

	 if (!le) memcpy((void*)(&lb), (const void*)buf, 4);
	 else 
	 {
		 buf2[0] = buf[3];
		 buf2[1] = buf[2];
		 buf2[2] = buf[1];
		 buf2[3] = buf[0];
		 memcpy((void*)(&lb), (const void*)buf2, 4);
	 }
	 l_sum += lb;
	 //printf("read = %d [%lf]\n", lb, d_sum);
 }
 //printf("Sum (64bit) = %.0lf\n", d_sum);
 //printf("Sum (32bit) = %u(0x%08x)\n", l_sum, l_sum);
 fclose(f);

 of = fopen(ofn, "a+");
 if (!of) 
 {
	 printf("Nie moge zapisac pliku: %s\n", ofn);
	 return;
 }
 
 r->sum = l_sum;
 fprintf(of, "%d) [%08x %08x] %08x\n", r->num+1, r->from, r->to, r->sum);

 fclose(of);
}

void read_ranges(char* fn)
{
	FILE* f;
	int i, diff;

	f = fopen(fn, "r");
	if (!f)
	{
		printf("Nie moge otworzyc pliku zakresow: %s\n", fn);
		exit(1);
	}
	n_ranges = -1;
	fscanf(f, "%d\n", &n_ranges);
	if (n_ranges <= 0)
	{
		printf("Bledna ilosc zakresow: %d\n", n_ranges);
		fclose(f);
		exit(1);
	}
	ranges = (Range*)malloc(n_ranges * sizeof(Range));
	for (i=0;i<n_ranges;i++)
	{
		fscanf(f, "%x %x\n", &ranges[i].from, &ranges[i].to);
		diff = ranges[i].to - ranges[i].from;
		if (ranges[i].from < 0 || ranges[i].to < 0 || diff < 4 || (diff % 4))
		{
			printf("Bledny zakres: %08x %08x\n", ranges[i].from, ranges[i].to);
			fclose(f);
			exit(1);
		}
		printf("zakres: [%08x %08x]\n", ranges[i].from, ranges[i].to);
		ranges[i].num = i;
	}

	fclose(f);
}

int main(int lb, char** par)
{
	int i;
	printf("Zastosowanie: %s [le/be] plik_danych plik_zakresow plik_sum\n", par[0]);
	if (lb < 5) return 0;
	int le;
	if (par[1][0] == 'l' || par[1][0] == 'L') 
	{
		printf("Tryb LittleEndian\n");
		le = 1;
	}
	else 
	{
		printf("Tryb BigEndian\n");
		le = 0;
	}

	read_ranges(par[3]);
	unlink( par[4]);
	for (i=0;i<n_ranges;i++)
	{
		calc_sum(le, par[2], &ranges[i], par[4]);
	}
	return 0;
}