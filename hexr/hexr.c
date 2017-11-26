#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXHEXLEN	16385

typedef struct _hexrplc
{
	char *s1, *s2, *h1, *h2;
	int l1, l2, u, n;
} hexrplc;

hexrplc* hx = NULL;
int hn = 0;
int ml = 0;
int dbg = 0;
int n_pass = -1;
int curr_pass = 0;
int pass_done = 0;

int process_reg(int idx, hexrplc* p, char* h1, char* h2)
{
	char tmp[3];
	int i, tmpi;
	if (!p || !h1 || !h2) return 1;

	p->l1 = (int)strlen(h1);
	p->l2 = (int)strlen(h2);
	if (p->l1 % 2 || p->l2 % 2)
	{
		printf("Bledne dlugosci ciagow, napisy w hex musza miec parzysta dlugosc: %s %s\n", h1, h2);
		return 1;
	}

	tmp[2] = 0;
	p->h1 = (char*)malloc(p->l1 + 1);
	p->h2 = (char*)malloc(p->l2 + 1);
	p->l1 /= 2;
	p->l2 /= 2;

	p->s1 = (char*)malloc(p->l1 + 1);
	p->s2 = (char*)malloc(p->l2 + 1);

	for (i=0;i<p->l1;i++)
	{
		tmp[0] = h1[i*2];
		tmp[1] = h1[i*2+1];
		sscanf(tmp, "%x", &tmpi);
		p->s1[i] = (char)tmpi;
	}
	p->s1[p->l1] = 0;

	for (i=0;i<p->l2;i++)
	{
		tmp[0] = h2[i*2];
		tmp[1] = h2[i*2+1];
		sscanf(tmp, "%x", &tmpi);
		p->s2[i] = (char)tmpi;
	}
	p->s2[p->l2] = 0;

	if (p->l1 > ml) ml = p->l1;
	if (p->l2 > ml) ml = p->l2;
	strcpy(p->h1, h1);
	strcpy(p->h2, h2);

	p->u = 1;
	p->n = 0;
	if (dbg) printf("Odczytalem %d regule: %d(%s/%s)\t%d(%s/%s)\n", idx, p->l1, p->s1, h1, p->l2, p->s2, h2);
	//else printf("Odczytalem %d regule: %d(%s)\t%d(%s)\n", idx, p->l1, h1, p->l2, h2);

	return 0;
} 

int check_unique()
{
	int i, j, n, u, nu;

	printf("Poszukuje duplikatow regul wejsciowych\n");

	n = 0;
	for (i=0;i<hn;i++)
		for (j=i+1;j<hn;j++)
		{
			if (hx[i].l1 != hx[j].l1) continue;
			if (!memcmp((const void*)hx[i].s1, (const void*)hx[j].s1, (size_t)hx[i].l1))
			{
				if (dbg) printf("Identyczne reguly wejsciowe %d i %d: %d(%s)\n", i+1, j+1, hx[i].l1, hx[i].h1);
				hx[j].u = 0;
				n ++;
			}
		}

	if (n > 0) 
	{
		printf("Znaleziono %d duplikatow, wylaczanie tych regul\n", n);
		u = nu = 0;
		for (i=0;i<hn;i++)
		{
			if (hx[i].u) u ++;
			else nu ++;
		}
		printf("Regul aktywnych: %d, niekatywnych: %d\n", u, nu);
	}

	return n;
}

void sort_regulas()
{
    int i, j, k;
    hexrplc htmp;

    printf("Sortuje reguly od najkrotszych do najdluzszych\n");

    k = 0;

    for (i=0;i<hn;i++)
    {
	if (!hx[i].u) continue;
	for (j=i+1;j<hn;j++)
	{
	    if (!hx[j].u) continue;
	    if (hx[j].l1 < hx[i].l1)
	    {
		k ++;
		memcpy((void*)(&htmp), (void*)(&hx[i]), sizeof(hexrplc));
		memcpy((void*)(&hx[i]), (void*)(&hx[j]), sizeof(hexrplc));
		memcpy((void*)(&hx[j]), (void*)(&htmp), sizeof(hexrplc));
	    }
	}
    }

    printf("Dokonano %d podmian\n", k);
    //for (i=0;i<hn;i++) printf("%d ", hx[i].l1);
}

int process_fz( char* fn )
{
	FILE* f;
	int n, i;
	char *h1, *h2;

	printf("Przetwarzam plik zmian: %s\n", fn);

	f = fopen( fn, "r");
	if (!f)
	{
		printf("Nie moge otworzyc pliku: %s\n", fn);
		return 1;
	}

	hx = NULL;
	hn = 0;
	ml = 0;
	n = 0;
	h1 = (char*)malloc(MAXHEXLEN);
	h2 = (char*)malloc(MAXHEXLEN);

	while (fscanf(f, "%s\t%s\n", h1, h2) == 2) n ++;

	printf("Odczytalem %d regul\n", n);
	if (n <= 0)
	{
		fclose( f );
		printf("Blad: brak regul\n");
		return 2;
	}
	
	fseek(f, 0, SEEK_SET);
	i = 0;
	hx = (hexrplc*)malloc(n * sizeof(hexrplc));
	hn = n;

	while (fscanf(f, "%s\t%s\n", h1, h2) == 2)
	{
		if (process_reg(i+1, &hx[i], h1, h2) != 0)
		{
			fclose( f );
			printf("Blad przetwarzania %d reguly: %s %s\n", i+1, h1, h2);
			return 3;
		}
		i ++;
	}
	printf("Przetworzylem %d regul, najdluzsza ma %d znakow\n", hn, ml);

	if (check_unique() != 0)
	{
		//fclose( f );
		printf("Znaleziono duplikaty regul wejsciowych\n");
		//return 4;
	}

	sort_regulas();

	fclose( f );
	return 0;
}

int rhex_match(int zn, int n, int* li)
{
	int i;

	if (dbg) printf("\nMT(%02x,%x, %x) ", zn, n, *li);

	for (i=0;i<hn;i++)
	{
		if (!hx[i].u || hx[i].l1 <= n) continue;
		if (hx[i].s1[n] == (char)zn)
		{
			if (dbg) printf("MS(%x,%x) ", hx[i].l1, n);
			if (hx[i].l1 == n + 1) 
			{
				if (dbg) printf("MSF(%x) ", n);
				*li = -1;
				return i;
			}
			else 
			{
				if (dbg) printf("MSP(%x,%x,%x) ", n, i, *li);
				if (*li == i || *li == -1) 
				{
					*li = i;
					return -1;
				}
			}
		}
	}
	if (dbg) printf("MN(%x) ", n);
	*li = -1;
	return -2;
}

int rhex_push(FILE* f, char* s, int n, int m)
{
	int i;
	for (i=0;i<n;i++) fputc(s[i], f);
	if (dbg && n > 0)
	{
		if (m == 0) printf("\nPUSH_MN(%x) HEX: ", n);
		else if (m == 1) printf("\nPUSH_MF(%x) HEX: ", n);
		else if (m == 2) printf("\nPUSH_BF(%x) HEX: ", n);
		for (i=0;i<n;i++) printf("%02x ", s[i]);
		//printf("\n");
	}
	return 0;
}

int file_map(FILE* f, char** buf)
{
    int fs, i;

    fseek(f, 0, SEEK_END);
    fs = (int)ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fs <= 0)
    {
	*buf = NULL;
	printf("Pusty plik do alokacji\n");
	return 0;
    }

    *buf = (char*)malloc(fs + 1);
    if (!(*buf))
    {
	printf("Nie moge zaalokowac %d bajtow pamieci\n", fs);
	return -1;
    }
    for (i=0;i<fs;i++) (*buf)[i] = (char)fgetc(f);
    return fs;
}

int process_fps(FILE* fi, FILE* fo)
{
	int nd, i, j, k, fnd, fs;
	char *tab;
	char *fm;

	//n = 0;
	tab = (char*)malloc(ml + 2);
	nd = 0;
	//li = -1;

        fs = file_map(fi, &fm);
        if (fs <= 0)
        {
		printf("Nie moge zmapowac w pamieci\n");
		fclose(fo);
		return 2;
        }
        printf("Ilosc bajtow w pliku wejsciowym: %d\n", fs);
	/*
	while ((zn = fgetc(fi)) != EOF)
	{
		m = rhex_match(zn, n, &li);

		if (m == -2)			// Brak dopasowania
		{
			//tab[n] = zn;
			//tab[n+1] = 0;
			//n ++;
			//rhex_push(fo, tab, n, 0);
			//n = 0;

			tab[n] = zn;
			tab[n+1] = 0;
			if (n > 0) fseek(fi, -n, SEEK_CUR);
			rhex_push(fo, tab, 1, 0);
			n = 0;
		}
		else if (m == -1)		// W trakcie dopasowania
		{
			tab[n] = zn;
			tab[n+1] = 0;
			n ++;
		}
		else if (m < hn)		// Dopasowano m-ty wzorzec
		{
			rhex_push(fo, hx[m].s2, hx[m].l2, 1);
			nd ++;
			n = 0;
			hx[m].n ++;
		}
		else
		{
			printf("Blad dopasowywania wzorca\n");
			free((void*)tab);
			return 1;
		}
	}
	// Jak pozosta³o coœ w buforze
	if (dbg) printf("\n");
	printf("Dopasowano %d wzorcow, pozostalo znakow w buforze %d\n", nd, n);
	if (n > 0) rhex_push(fo, tab, n, 2);
	if (dbg) printf("\n");
	*/
        for (i=0;i<fs;i++)
        {
	  fnd = 0;
	  if (dbg) 
	  {
	      printf("byte[%x/%x]: %x(%c): ", i, fs, fm[i], fm[i]);
	      j = ml;
	      if (i + j > fs) j = fs - i;
	      strncpy(tab, (char*)(&fm[i]), j);
	      tab[j] = 0;
	      printf("[%s]\n", tab);
	  }
	  else
	  {
	      if (i && (i % 4096) == 0)
	      {
		  for (k=0;k<20;k++) printf("\r");
		  printf("%03.4f%% [%8d]", ((double)i * 100.) / (double)fs, nd);
		  fflush(stdout);
	      }
	  }
	  for (j=0;j<hn;j++)
	  {
	    if (!memcmp((const void*)(&fm[i]), 
			(const void*)(hx[j].s1), 
			(size_t)(hx[j].l1)))
		    {
		    	for (k=0;k<hx[j].l2;k++) fputc((int)hx[j].s2[k], fo);
			i += (hx[j].l1 - 1);
			hx[j].n ++;
			if (dbg) 
			{
			    printf("applied: [%x,%x] %d(%s/%s) --> %d(%s/%s)\n", 
				    j, hx[j].n, hx[j].l1, hx[j].s1, hx[j].h1, hx[j].l2, hx[j].s2, hx[j].h2);

			}
			j = hn;
			fnd = 1;
			nd ++;
		    }

	  }
	  if (!fnd) fputc((int)fm[i], fo);
        }
	printf("\n");


	if (nd == 0) pass_done = 1;
	curr_pass ++;
	if (curr_pass == n_pass && n_pass > 0) pass_done = 1;

	if (dbg || pass_done)
	{
		for (i=0;i<hn;i++)
		{
			if (hx[i].n > 0) printf("Regula %d: %d(%s)\t%d(%s) uzyta %d razy\n", i+1, hx[i].l1, hx[i].h1, hx[i].l2, hx[i].h2, hx[i].n);
		}
	}

	free((void*)tab);
        free((void*)fm);
	return 0;
}

int process_files(char* fni, char* fno)
{
	FILE *fi, *fo;

	fi = fopen(fni, "rb");
	if (!fi)
	{
		printf("Nie moge odczytac pliku wejsciowego: %s\n", fni);
		return 1;
	}

	fo = fopen(fno, "wb");
	if (!fo)
	{
		fclose(fi);
		printf("Nie moge zapisac pliku wyjsciowego: %s\n", fno);
		return 1;
	}

	if (process_fps( fi, fo ) != 0)
	{
		fclose(fi);
		fclose(fo);
		printf("B³ad wewnêtrzny przetwarzania pliku wejsciowego\n");
		return 1;
	}

	fclose(fi);
	fclose(fo);
	return 0;
}

void make_names(char* fni, char** pfni, char** pfno)
{
//int n_pass = -1;
//int curr_pass = 0;
//int pass_done = 0;

	if (curr_pass == 0) strcpy(*pfni, fni);
	else sprintf(*pfni, "pass%d", curr_pass);

	sprintf(*pfno, "pass%d", curr_pass + 1);

	printf("Current PASS: %s --> %s [%d/%d]\n", *pfni, *pfno, curr_pass + 1, (n_pass > 0) ? n_pass : 999999999);
}

int hexr(char* fnz, char* fni, char* fno)
{
	char *pfni, *pfno;

	if (process_fz( fnz ) != 0)
	{
		printf("B³ad odczytu pliku zmian\n");
		return 1;
	}

	if (n_pass < 0)
	{
		if (process_files( fni, fno ) != 0)
		{
			printf("B³ad przetwarzania pliku wejsciowego\n");
			return 1;
		}
	}
	else
	{
		pass_done = 0;
		pfni = (char*)malloc(1025);
		pfno = (char*)malloc(1025);

		while (!pass_done)
		{
			make_names(fni, &pfni, &pfno);

			if (process_files( pfni, pfno ) != 0)
			{
				printf("B³ad przetwarzania pliku wejsciowego\n");
				return 1;
			}
		}

		printf("Zmiana nazwy pliku: %s --> %s\n", pfno, fno);
		rename(pfno, fno);

		free((void*)pfni);
		free((void*)pfno);
	}

	return 0;
}

int main(int lb, char** par)
{
	if (lb < 4)
	{
		printf("Uzycie: %s plik_zmian plik_wejsciowy plik_wyjsciowy [d] [n]\n", par[0]);
		printf("Format pliku zmian: dowolna ilosc linii typu: ciag_w_hex TABULATOR drugi_ciag_w_hex NOWA_LINIA\n");
		printf("If n given then do <= n passes (until no more changes), if n=0 then loop until no more changes\n");
		return 1;
	}

	if (lb >= 5 && par[4][0] == 'd') dbg = 1;

	if (lb >= 6) sscanf(par[5], "%d", &n_pass);
	else n_pass = -1;

	if (hexr(par[1], par[2], par[3]) != 0)
	{
		printf("Blad wykonania programu %s\n", par[0]);
		return 1;
	}

	return 0;
}
