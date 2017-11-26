#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <map>

using namespace std;
typedef unsigned char byte;
typedef char sbyte;

class MeteoDate
{
    public:
    	MeteoDate();
    	~MeteoDate();

	void Clear();
	bool Normalize();
	bool isNormal();

	short y;
	sbyte m, d, h, mi;
};

MeteoDate :: MeteoDate()
{
    Clear();
}

MeteoDate :: ~MeteoDate()
{
    Clear();
}

void MeteoDate :: Clear()
{
    y = 1900;
    m = d = 1;
    h = mi = 0;
}

bool MeteoDate :: isNormal()
{
    bool ok = true;

    if (y < 1900) ok = false;
    if (y > 2100) ok = false;

    if (m < 1)    ok = false;
    if (m > 12)   ok = false;

    if (d < 1)    ok = false;

    if ((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) && d > 31) ok = false;
    else if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) ok = false;
    else if (m == 2 && (y % 4) && d > 28) ok = false;
    else if (m == 2 && !(y % 4) && d > 29) ok = false;

    if (h < 0) ok = false;
    if (h > 23) ok = false;

    if (mi < 0) ok = false;
    if (mi > 59) ok = false;

    return ok;
}

bool MeteoDate :: Normalize()
{
    bool ok = true;

    if (y < 1900) { y = 1900; ok = false; }
    if (y > 2100) { y = 2100; ok = false; }

    if (m < 1)    { m = 1;  ok = false; }
    if (m > 12)   { m = 12; ok = false; }

    if (d < 1)    { d = 1; ok = false; }

    if ((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) && d > 31) { d = 31; ok = false; }
    else if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) { d = 30; ok = false; }
    else if (m == 2 && (y % 4) && d > 28) { d = 28; ok = false; }
    else if (m == 2 && !(y % 4) && d > 29) { d = 29; ok = false; }

    if (h < 0) { h = 1; ok = false; }
    if (h > 23) { h = 23; ok = false; }

    if (mi < 0) { mi = 1; ok = false; }
    if (mi > 59) { mi = 59; ok = false; }

    return ok;
}
bool operator<(const MeteoDate& _left, const MeteoDate& _right)
{
	if (_left.y == _right.y)
	{
		if (_left.m == _right.m)
		{
			if (_left.d == _right.d)
			{
				return (bool)(_left.h  < _right.h );
			}
			else return (bool)(_left.d < _right.d);
		}
		else return (bool)(_left.m < _right.m);
	}
	else return (bool)(_left.y < _right.y);
}

class MeteoValue
{
    public:
    	MeteoValue();
    	~MeteoValue();

	void Clear();

	MeteoDate dt;		// data obserwacji
	byte h; 		// wysokosc podstawy chmur;
	byte VV;		// widzialnosc FIXME jakos to przerabiac
	byte N;			// ogolne zachmurzenie 0-8
	byte dd;		// kierunek wiatru 00-36
	byte ff;		// predkosc wiatru w pliku ff/2 ^|^
	float TTT;		// temperatura X-TTT -=0 dodatnia, -=1 ujemna TTT/10.0
	byte WILG;		// wilgotnosc powietrza, z LOG-u wilgotnosc wzgledna chwilowa z T+1 min
	float TTTD;		// temperatura X-TTT punktu rosy
	float CNPS;		// cisnienie na poziomie stacji, z logu cisnienie chwilowe
	float CNPM;		// cisnienie nad poziomem morza, jak CNPM < 100 to +1000 hPa
	byte A;			// charakter tendencji 0-8 0-3 dodatnie PPP, 4 = zero, 5-8 ujemne PPP
	float PPP;		// +/- zalezy od A, PPP/10
	float OPAD;		// 6OPAT, T=1=6h, T=2=12h, jak brak to 999.9 dla h=6,12,8,0 i 999.8 dla pozostalych h
	byte RODZAJ;		//!Puste i dla nas nieznane!
	byte WW, W1;		// 7WW12 1=W1, 2=W2
	byte NH, CL, CM, CH;	// 8 NH CL CM CH {CL, Cm, CH} = {0..9 + / }, wartosci specjalne 98 dla /


        //2009 06 13 18    01003 31484 32908 10032 20000 40263 52005 70200 81542 333 10068 91117 555 0/414 20032 
        //2009 06 15 00    01003 11330 80727 10037 21002 40140 57034 69921 76322 885// 333 91152 555 0/127
        //2009 06 15 03    01003 41330 80827 10032 20002 40117 57022 76366 885// 555 0/231
	//2009 06 14 18    01003 31380 80721 10036 21009 40192 57022 70211 82518 333 10066 91139 555 0/121 20028 
	//2009 06 14 21    01003 41375 80725 10032 21002 40174 57018 70222 885// 555 0/025
	//2009 06 14 21    01003 41375 80725 10032 21002 40174 57018 70222 885// 555 0/025
	//2009 06 15 00    01003 11330 80727 10037 21002 40140 57034 69921 76322 885// 333 91152 555 0/127
	//YYYY MM DD HH    01003 XXhVV Nddff 1-TTT 2-TTD 4CNPM 5APPP[6OPAT]7WW12 8HLMH ...

};

struct LOGData
{
    LOGData() { WILG = 0; CNPS = 0.; }
    int WILG;
    float CNPS;
};


map<MeteoDate, LOGData> logVals;

void MeteoValue :: Clear()
{
    h = VV = 1;
    N = 8;
    dd = 9;
    ff = 1;
    TTT = -1.0;
    WILG = 255;  //95;		// z LOG-u
    TTTD = -2.3;
    CNPS = 666.6; //1013.25;	// z LOG-u
    CNPM = 1013.25;
    A = 4;
    PPP = 0.0;
    OPAD = 1.0;
    RODZAJ = -1;	// ma byc puste!
    WW = 51;
    W1 = 6;
    NH = 8;
    CL = 6;
    CM = CH = 98;
    dt.Clear();
}

MeteoValue :: MeteoValue()
{
    Clear();
}

MeteoValue :: ~MeteoValue()
{
    Clear();
}

vector<MeteoValue> meteos;

int load_meteo_txt(char* fn)
{
    FILE* f;
    MeteoDate dt;
    MeteoValue mv;
    bool done = false;
    int lp = 0;
    int err = 0;
    int rec = 0;
    int iy, im, id, ih, dummy, vis, wind, g1, g2, g4, g5, g6, g7;
    char g8[256], svis[256];

    f = fopen( fn, "rb+" );
    if (!f)
    {
	printf("Nie moge otworzyc pliku: %s\n", fn);
	return 1;
    }

    // File format
    //2009 06 13 18    01003 31484 32908 10032 20000 40263 52005 70200 81542 333 10068 91117 555 0/414 20032 
    //2009 06 15 00    01003 11330 80727 10037 21002 40140 57034 69921 76322 885// 333 91152 555 0/127
    //2009 06 15 03    01003 41330 80827 10032 20002 40117 57022 76366 885// 555 0/231
    //YYYY MM DD HH    01003 XXhVV Nddff 1-TTT 2-TTD 4CNPM 5APPP[6OPAT]7WW12 8HLMH ...

    while(!done)
    {
	lp ++;
	mv.Clear();

    	if (fscanf(f, "%d %d %d %d ", &iy, &im, &id, &ih) != 4)
    	{
	    if (fgetc(f) != EOF)
	    {
	    	printf("Blad(1) odczytu linii numer %d w pliku %s\n", lp, fn);
	    }
	    done = true;
	    continue;
    	}

	dt.y = (short)iy;
	dt.m = (sbyte)im;
	dt.d = (sbyte)id;
	dt.h = (sbyte)ih;

	if (!dt.isNormal())
	{
	    printf("Bledna data: %04d-%02d-%02d %02d:00:00\n", dt.y, dt.m, dt.d, dt.h);
	    err = 2;
	    done = true;
	    continue;
	}

	// Domyslny konstruktor kopiujacy, mozna spokojnie bo nie ma w Meteodate zadnych pointerow, tylko inty
	mv.dt = dt;

	if (fscanf(f, "%d %255s %d %d %d %d %d ", &dummy, svis, &wind, &g1, &g2, &g4, &g5) != 7)
    	{
	    printf("Blad(2) odczytu linii numer %d w pliku %s\n", lp, fn);
	    err = 1;
	    done = true;
	    continue;
    	}

	if (svis[2] == '/') 
	{ 
	    svis[2] = '0'; 
	    im = 1; 
	}
	else im = 0;

	sscanf(svis, "%d", &vis);

        //YYYY MM DD HH    01003 XXhVV Nddff 1-TTT 2-TTD 4CNPM 5APPP[6OPAT]7WW12 8HLMH ...
	mv.h = (vis % 1000) / 100;
	mv.VV = vis % 100;
	if (im) mv.h = 255;		// FIXME potem w excelu ma to byc /

	im = mv.VV;

	if (im <= 1) mv.VV = 1;
	else if (im <= 4) mv.VV = 2;
	else if (im <= 9) mv.VV = 3;
	else if (im <= 19) mv.VV = 4;
	else if (im <= 39) mv.VV = 5;
	else if (im <= 59) mv.VV = 6;
	else if (im <= 69) mv.VV = 7;
	else if (im <= 83) mv.VV = 8;
	else mv.VV = 9;

	if (im  >= 51 && im <= 55) printf("UWAGA: wartosc VV w pliku wejsciowym z przedzialu [51,55]: %d\n", im);

	mv.N = wind / 10000;
	mv.dd = (wind % 10000) / 100;
	mv.ff = wind % 100;

	if (mv.ff % 2) mv.ff = (mv.ff / 2) + 1;	// zaokraglanie zawsze w gore
	else mv.ff /= 2;

	im = g1 / 10000;
	if (im != 1)
	{
	    printf("Blad(3) odczytu linii numer %d w pliku %s, oczekiwano 1sTTT, wczytano: %d", lp, fn ,g1);
	    err = 1;
	    done = true;
	    continue;
	}

	im = (g1 % 10000) / 1000;
	id = g1 % 1000;

	mv.TTT = (float)id / 10.;
	if (im) mv.TTT *= -1.;

	// FIXME: LOD, TTW, PPAR puste a nast WILG do wyszukania w LOG-u

	im = g2 / 10000;
	if (im != 2)
	{
	    printf("Blad(4) odczytu linii numer %d w pliku %s, oczekiwano 2sTTD, wczytano: %d", lp, fn ,g2);
	    err = 1;
	    done = true;
	    continue;
	}

	im = (g2 % 10000) / 1000;
	id = g2 % 1000;

	mv.TTTD = (float)id / 10.;
	if (im) mv.TTTD *= -1.;

        //YYYY MM DD HH    01003 XXhVV Nddff 1-TTT 2-TTD 4CNPM 5APPP[6OPAT]7WW12 8HLMH ...
	//FIXME: CNPS - znalezc w LOG-u
	im = g4 / 10000;
	if (im != 4)
	{
	    printf("Blad(5) odczytu linii numer %d w pliku %s, oczekiwano 4CNPM, wczytano: %d", lp, fn ,g4);
	    err = 1;
	    done = true;
	    continue;
	}
	
	im = g4 % 10000;
	mv.CNPM = (float)im / 10.;
	if (mv.CNPM < 200.) mv.CNPM += 1000.;

	im = g5 / 10000;
	if (im != 5)
	{
	    printf("Blad(6) odczytu linii numer %d w pliku %s, oczekiwano 5APPP, wczytano: %d", lp, fn ,g5);
	    err = 1;
	    done = true;
	    continue;
	}

	mv.A = (g5 % 10000) / 1000;
	im = g5 % 1000;
	mv.PPP = (float)im / 10.;
	if (mv.A >= 5) mv.PPP *= -1;

	if (fscanf(f, "%d ", &g6) != 1)
    	{
	    printf("Blad(7) odczytu linii numer %d w pliku %s\n", lp, fn);
	    err = 1;
	    done = true;
	    continue;
    	}

	im = g6 / 10000;
	if (im != 6 && im != 7)
	{
	    printf("Blad(8) odczytu linii numer %d w pliku %s, oczekiwano 6OPAH lub 7WW12 wczytano: %d", lp, fn ,g6);
	    err = 1;
	    done = true;
	    continue;
	}

	if (im == 6)	// mamy opad
	{
	    iy = (g6 % 10000) / 10;
	    if (iy >= 990) mv.OPAD = ((float)iy - 990.) / 10.;
	    else mv.OPAD = (float)iy;

/*	    printf("OPAD = %f (%d)\n", mv.OPAD, g6);*/

	    if (fscanf(f, "%d ", &g7) != 1)
    	    {
	        printf("Blad(8) odczytu linii numer %d w pliku %s\n", lp, fn);
	        err = 1;
	        done = true;
	        continue;
    	    }

	    im = g7 / 10000;
	    if (im != 7)
	    {
	         printf("Blad(9) odczytu linii numer %d w pliku %s, oczekiwano 7WW12 wczytano: %d", lp, fn ,g7);
	         err = 1;
	         done = true;
	         continue;
	    }
	    //printf("OPAD=%.1f mm\n", mv.OPAD);
	}
	else		// nie mamy opadu
	{
	    if (mv.dt.h % 6) mv.OPAD = 999.8;	// wartosc brak dla godzin nie rownych 0,6,12,18
	    else mv.OPAD = 999.9;		// wartosc brak dla godzin podstawowych czyli 0,6,12,18
		
	    //printf("Brak OPAD=%.1f mm\n", mv.OPAD);

	    g7 = g6;
	}

	//FIXME: rodzaj puste
	mv.WW = (g7 % 10000) / 100;
	mv.W1 = (g7 % 100) / 10;

	if (mv.h == 255)	// przypadek braku widzialnosci
	{
	    // wtedy jak nie znajdzie grupy 8xxxx
	    // to takie domyslne wartosci sa
	    mv.CL = mv.CM = mv.CH = 98;
	    mv.NH = 9;
	}			// wpp dobre sa wartosci domyslne
	
	if (fscanf(f, "%s ", g8) == 1)
    	{
		g8[5] = 0;
		im = g8[0] - '0';

		if (im == 8)
		{
			mv.NH = g8[1] - '0';

			if (g8[2] == '/') mv.CL = 98;
			else mv.CL = g8[2] - '0';

			if (g8[3] == '/') mv.CM = 98;
			else mv.CM = g8[3] - '0';

			if (g8[4] == '/') mv.CH = 98;
			else mv.CH = g8[4] - '0';
			//printf("%s --> NH=%d, CL=%d, CM=%d, CH=%d\n", g8, mv.NH, mv.CL, mv.CM, mv.CH);
		}
	}

	do
	{
	    im = fgetc(f);
	}
	while (im != '\n' && im != EOF);

	if (im == EOF) 
	{
	    done = true;
	}


	//printf("%d --> h=%d km, VV=%d\n", vis, mv.h, mv.VV);
	//printf("%d --> N=%d km, dd=%d deg*10, ff=%d m/s\n", wind, mv.N, mv.dd, mv.ff);
	//printf("%d --> TTT=%.1f C\n", g1, mv.TTT);
	//printf("%d --> TTTD=%.1f C\n", g2, mv.TTTD);
	//printf("%d --> CNPM=%.1f hPa\n", g4, mv.CNPM);
	//printf("%d --> A=%d, PPP=%.1f hPa\n", g5, mv.A, mv.PPP);
	//printf("%d --> WW=%d, W1=%d\n", g7, mv.WW, mv.W1);

	//printf("Odczytano %d linie meteo txt: %04d-%02d-%02d %02d:00:00\n", lp, mv.dt.y, mv.dt.m, mv.dt.d, mv.dt.h);
	meteos.push_back( mv );
	rec ++;
    }
    fclose( f );

    printf("Ilosc rekordow w pliku TXT: %d\n", rec);

    return err;
}


bool fixate_parse(FILE* f, int& lp)
{
    int pos, zn, m, i, l;

    pos = ftell( f );
    m = i = l = 0;

    while (1)
    {
	i ++;
	zn = fgetc(f);

	if (i > 8192)
	{
	    m = 0;
	    break;
	}

	if (zn == EOF)
	{
	    printf("Po ostatnim bledzie skladni, plik konczy sie po mniej niz 8 Kb, zakladam ze wiekszosc odczytano poprawnie\n");
	    return true;
	}
	else if (zn == '\n') l ++;

	if (m == 0 && zn == 'Q') 
	{
	    m = 1;
	    continue;
	}

	if (m == 1)
	{
	    if (zn == 'L') 
	    {
		m = 2;
		break;
	    }
	    else m = 0;
	}
    }

    if (m < 2)
    {
	printf("Nie udalo sie znalezc QL w 8Kb po wystapieniu bledu skladni, oznacza to krytyczne uszkodzenie pliku logu\n");
	fseek(f, pos, SEEK_SET);
	return false;
    }
    else
    {
	fseek(f, -2, SEEK_CUR);
	lp += l;
	printf("Znaleziono kolejny rekord po pominieciu %d bajtow po wystapieniu bledu skladni w pliku logu\n", i);
	return true;
    }

}

int load_meteo_log(char* fn)
{
//QLC50 TOTAL 2011-10-02 12:09                     STACJA: HORNSUND
//
//                                  CHWIL       SR        MAX       MIN
//TEMPERATURA POWIETRZA    C :       3.7 :     3.5 :     3.7 :     3.2 :
//TEMP. GRUNTU na 5 cm     C :       1.9 :     1.8 :     1.9 :     1.8 :
//TEMP. GRUNTU na 10 cm    C :       2.0 :     1.9 :     2.0 :     1.9 :
//TEMP. GRUNTU na 20 cm    C :       1.8 :     1.8 :     1.9 :     1.8 :
//TEMP. GRUNTU na 50 cm    C :       1.3 :     1.2 :     1.3 :     1.2 :
//WILGOTNOSC WZGLEDNA      % :        97 :      97 :      98 :      97 :
//TEMPERATURA PUNKTU ROSY  C :       3.3 :     3.1 :     3.4 :     2.8 :
//CISNIENIE              hPa :     989.9 :   989.8 :   989.9 :   989.7 :
//CISNIENIE QNH          hPa :     991.1 :   991.0 :   991.1 :   990.9 :
//CISNIENIE QFF          hPa :     991.1 :   991.0 :   991.1 :   990.9 :
//PREDKOSC WIATRU  2min  m/s :       3.3 :     2.2 :     3.8 :     1.1 :
//                 10min m/s :           :     2.1 :     2.8 :     1.3 :
//KIERUNEK WIATRU  2min   st :       242 :     251 :     287 :     214 :
//                 10min  st :           :     277 :     309 :     231 :
//C. PROMIENIOW. SLON.  W/m2 :      18.4 :    18.0 :    25.9 :    11.2 :
//NASLONECZNIENIE        min :         0 :       0 :         :         :
//ZASILANIE                V :      28.1 :         :         :         :

    FILE* f;

    f = fopen(fn, "rb+");
    if (!f)
    {
	printf("Nie moge otworzyc pliku logu: %s\n", fn);
	return 1;
    }

    int n;
    bool done = false;
    char stmp[256], stmp2[256];
    int lp = 2;
    int y, m, d, h, mi, zn;
    int err = 0;
    int WILG;
    float CNPS;
    int rec = 0;

    while (!done)
    {
	lp ++;
/*	printf("%d: -> ", ftell(f));*/
/*	fflush(stdout);*/
	n = fscanf(f, "%255s %255s %d-%d-%d %d:%d STACJA: %255s\n", stmp, stmp2, &y, &m, &d, &h, &mi, stmp);
/*	printf("%d\n", ftell(f));*/
/*	fflush(stdout);*/
	if (n != 8)
	{
	    zn = fgetc(f);
	    if (zn != EOF)
	    {
	    	printf("Blad(1) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    	err += 1;
	        if (!fixate_parse(f, lp)) done = true;
	    }
	    else
	    {
		printf("Wykryto koniec pliku\n");
		err *= -1;
		done = true;
	    }

	    continue;
	}

	//printf("%04d-%02d-%02d %02d:%02d\n", y, m, d, h, mi);

	lp ++;
	n = fscanf(f, "%255s %255s %255s %255s\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(2) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "TEMPERATURA POWIETRZA C : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(3) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "TEMP. GRUNTU na 5 cm C : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(4) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "TEMP. GRUNTU na 10 cm C : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(5) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "TEMP. GRUNTU na 20 cm C : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(6) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "TEMP. GRUNTU na 50 cm C : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(7) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "WILGOTNOSC WZGLEDNA %% : %255s : %255s : %255s : %255s :\n", stmp2, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(8) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	if (mi == 1 && !(h % 3))
	{
		if (sscanf(stmp2, "%d", &WILG) != 1) 
		{
	    		printf("Bad odczytu wilgotnosci powietrza z \"%s\", zakladam wilgotnosc 0\n", stmp2);
	    		WILG = 0;
		}
	}

	lp ++;
	n = fscanf(f, "TEMPERATURA PUNKTU ROSY C : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(9) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "CISNIENIE hPa : %255s : %255s : %255s : %255s :\n", stmp2, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(10) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	if (mi == 1 && !(h % 3))
	{
	 	if (sscanf(stmp2, "%f", &CNPS) != 1) 
		{
	    		printf("Bad odczytu CNPS z \"%s\", zakladam CNPS 0\n", stmp2);
	    		CNPS = 0.;
		}
	}

	lp ++;
	n = fscanf(f, "CISNIENIE QNH hPa : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(11) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "CISNIENIE QFF hPa : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(12) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "PREDKOSC WIATRU 2min m/s : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(13) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, " 10min m/s : : %255s : %255s : %255s :\n", stmp, stmp, stmp);
	if (n != 3)
	{
	    printf("Blad(14) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "KIERUNEK WIATRU 2min st : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(15) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, " 10min st : : %255s : %255s : %255s :\n", stmp, stmp, stmp);
	if (n != 3)
	{
	    printf("Blad(16) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "C. PROMIENIOW. SLON. W/m2 : %255s : %255s : %255s : %255s :\n", stmp, stmp, stmp, stmp);
	if (n != 4)
	{
	    printf("Blad(17) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "NASLONECZNIENIE min : %255s : %255s : : :\n", stmp, stmp);
	if (n != 2)
	{
	    printf("Blad(18) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp ++;
	n = fscanf(f, "ZASILANIE V : %255s : : : :\n", stmp);
	if (n != 1)
	{
	    printf("Blad(19) parsowania pliku logu, linia %d, plik: %s\n", lp, fn);
	    err += 1;
	    if (!fixate_parse(f, lp)) done = true;
	    continue;
	}

	lp += 3;
	rec ++;

	if (mi == 1 && !(h % 3))
	{
		//printf("%04d-%02d-%02d %02d:%02d\n", y, m, d, h, mi);
		MeteoDate dt;
		LOGData ld;

		dt.y = y;
		dt.m = m;
		dt.d = d;
		dt.h = h;
		dt.mi = 0;

		ld.WILG = WILG;
		ld.CNPS = CNPS;

		logVals.insert(pair<MeteoDate, LOGData>(dt, ld));
	}
    }

    fclose(f);


    if (err < 0) printf("Napotkano %d bledow skladni w pliku logu, ale udalo sie odczytac plik\n", -err);
    else if (err > 0) printf("Napotkano krytyczne bledy skladni w plik (%d)\n", err);
    else printf("Nie napotako bledow w pliku logu\n");

    printf("Dodano %d mapowan logu, rekordow w logu: %d\n", (int)logVals.size(), rec);

    if (err > 0) return err;
    else return 0;
}

int save_meteo_csv(char* fn)
{
    FILE* f;

    f = fopen(fn, "r");

    if (f)
    {
	fclose(f);
	printf("Plik %s juz istnieje, usun go najpierw wlasnorecznie, program nie nadpisuje danych!\n", fn);
	return 2;
    }

    f = fopen(fn, "w");

    if (!f)
    {
	printf("Nie moge zapisac pliku: %s\n", fn);
	return 1;
    }

    fprintf(f, "DATA;GODZ;H;VV;N;DD;FF;TTT;LOD;TTW;PPAR;WILG;TTTD;CNPS;CNPM;A;PPP;OPAD;RODZAJ;WW;W1;NH;CL;CM;CH\n");

    vector<MeteoValue>::iterator i;

    for (i=meteos.begin(); i != meteos.end(); ++i)
    {
	fprintf(f, "%04d-%02d-%02d;%d;", i->dt.y, i->dt.m, i->dt.d, i->dt.h);
	if (i->h > 200) fprintf(f, "/;");
	else fprintf(f, "%d;", i->h);

	fprintf(f, "%d;%d;%d;%d;", i->VV, i->N, i->dd, i->ff);
	fprintf(f, "%.1f;;;;%d;", i->TTT, i->WILG);
	fprintf(f, "%.1f;%.1f;%.1f;%d;%.1f;", i->TTTD, i->CNPS, i->CNPM, i->A, i->PPP);
	fprintf(f, "%.1f;;%d;%d;", i->OPAD, i->WW, i->W1);
	fprintf(f, "%d;%d;%d;%d\n", i->NH, i->CL, i->CM, i->CH);
    }


    fclose(f);
    return 0;
}

int assoc_txt_and_log()
{
    int err = 0;
    int p0, p1;
    FILE* f;

    p0 = p1 = 0;
    f = fopen("DUPA.txt", "w");

    map<MeteoDate, LOGData>::iterator iter;

    for (vector<MeteoValue>::iterator i=meteos.begin(); i != meteos.end(); ++i)
    {
	i->dt.mi = 0;
	iter = logVals.find( i->dt );

	if (iter != logVals.end())	// znaleziono wlogu
	{
	    p1 ++;
	    i->WILG = iter->second.WILG;
	    i->CNPS = iter->second.CNPS;
	}
	else		// nie znaleziono w logu
	{
	    p0 ++;
	    if (f)
	    {
		fprintf(f, "UWAGA: dla daty %04d-%02d-%02d %02d:00 nie znaleziono wpisu w logach, WILG i CNPS beda z DUPY\n"
		    , i->dt.y, i->dt.m, i->dt.d, i->dt.h);
		//FIXME: taki myk jak nie znajdzie ze CNPS = CNPM - 1.3
		i->CNPS = i->CNPM - 1.3;
	    }
	}
    }
    fclose(f);
	    
    if (p0 > 0)
    {
    	printf("UWAGA UWAGA MAM WAZNY KOMUNIKAT :P\n"
	       "Znaleziono w logu %d wpisow, nie znaleziono %d wpisow - dla nich WILG i CNPS beda z DUPY\n"
	       "Wszelkie takie przypadki zapisano w pliku DUPA.txt\n", p1, p0);
    }

    return err;
}

void meteo_conv(int n, char** fn)
{
    int err;
    FILE* f;

    f = fopen(fn[2], "r");

    if (f)
    {
	fclose(f);
	printf("UWAGA KURNA!\nPlik %s juz istnieje, usun go najpierw wlasnorecznie, program nie nadpisuje danych!\n", fn[2]);
	return;
    }

    printf("Wczytuje plik meteo TXT: %s\n", fn[1]);
    err = load_meteo_txt( fn[1] );
    if (err)
    {
	printf("Blad wczytywania pliku meteo txt: %s\n", fn[1]);
	return;
    }

    for (int i=3;i<n;i++)
    {
    	printf("Wczytuje plik meteo LOG: %s\n", fn[i]);
    	err = load_meteo_log( fn[i] );
    	if (err)
    	{
	    printf("Blad wczytywania pliku meteo log: %s\n", fn[i]);
	    return;
    	}

    }

    printf("Pliki wczytane poprawnie.\n");

    err = assoc_txt_and_log();
    if (err)
    {
	printf("Blad wyszukiwania brakujacych danych w plikach logu\n");
	return;
    }

    err = save_meteo_csv( fn[2] );
    if (err)
    {
	printf("Blad zapisywania pliku meteo csv: %s\n", fn[2]);
	return;
    }

    printf("Plik CSV zapisany poprawnie.\n");

}

void help(char* pname)
{
    printf("Uzycie programu:\n\t%s plik_meteo.txt plik_wyjsciowy_csv.csv [plik_logu1.log ...]\n", pname);
}

int main(int lb, char** par)
{
    if (lb < 3)
    {
	help( par[0] );
    }
    else 
    {
	meteo_conv(lb, par);
    }

    return 0;
}

