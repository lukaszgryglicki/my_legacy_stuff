#include <stdio.h>			/*X11R6 required*/
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <termios.h>
typedef unsigned long ulong;
static FILE* _ran_device;		/* where to grt random values */

int mgetch()
{
 struct termios st,end;
 int ret;
 tcgetattr(fileno(stdin),&st);
 end = st;
 end.c_lflag &= ~ICANON;
 end.c_lflag &= ~ECHO;
 end.c_cc[VMIN] = 1;
 end.c_cc[VTIME] = 0;
 if (tcsetattr(fileno(stdin),TCSAFLUSH,&end)) return 0;
 st:
 ret = 0;
 ret = getchar();
 if (ret == 10) goto st;
 tcsetattr(fileno(stdin),TCSAFLUSH,&st);
 /* printf("%c",ret); */
 return ret;
}


int debug(char* fmt, ...)  /* print out DEBUG information if DEBUG defined */
{
#ifdef DEBUG
 va_list ap;
 int err;
 va_start(ap,fmt);
 err = vprintf(fmt,ap);
 va_end(ap);
 return err;
#endif
#ifndef DEBUG
 return 0;
#endif
}


int Debug(char* fmt, ...)  /* print out DEBUG information if DEBUG defined */
{
#ifdef DEBUGALG
 va_list ap;
 int err;
 va_start(ap,fmt);
 err = vprintf(fmt,ap);
 va_end(ap);
 return err;
#endif
#ifndef DEBUGALG
 return 0;
#endif
}


int Randomize()   /* init random engine */
{
 debug("Randomize\n");
 if ((_ran_device=fopen("/dev/urandom","r"))==NULL)
   {
    printf("RANLIB Ooops:\nCannot open device: /dev/urandom");
    return 0;
   }
 else return 1;
}


int Random(int lb)   /* randomize number from 0 to lb-1 */
{
 int ins[4],i;
 ulong result;
 debug("Random\n");
 for (i=0;i<4;i++)
     ins[i] = fgetc(_ran_device);
 result = ins[0] + 0x100*ins[1] + 0x10000*ins[2]+0x1000000*ins[3];
 result /= (0xffffffff/lb);
 return (int)result;
}


void Kill_random()  /* close random seed */
{
 debug("Kill_random\n");
  if (_ran_device) fclose(_ran_device);
}

static int selected = 0;		/* AKTUALNIE ZAZNACZONE POLE */
static int board[27];			/* board 0-empty, 1-human, 2-CPU */
static int done = 0;			/*quit when done*/
static int end_cond = 0;		/* CPU win or lost currently */
static int cpu_wins=0;
static int hum_wins=0;

void stats()
{
 printf("\n***********************\n");
 printf("\n\nHUMAN/CPU: %d/%d\n\n",hum_wins,cpu_wins);
 printf("\n***********************\n");
 if ((hum_wins-cpu_wins)>5)
   {
    printf("GRATULACJE WYGRALES(AS)\n");
    exit(0);
   }
 if ((hum_wins-cpu_wins)<-5)
   {
    printf("CPU WYGRAL 5 RAZY WIECEJ NIZ TY\n");
    exit(0);
   }
}


void init() /*create sine/cosine tables and board*/
{
 int i;
 debug("init\n");
 for (i=0;i<27;i++) board[i] = 0;
}


void check()			/* czy wszystkie dane sa sensowne ? */
{
#ifdef DEBUG
 printf("check_angles\n");
#endif
 if (selected<0)    selected += 27;
 if (selected>=27)  selected -= 27;
}


void move_x(int arg) /* move selected +/- X */
{
 int tst;
 int tst2;
 debug("move_x:%d\n", arg);
 tst = (selected%3);
 if (arg>0)
   {
    selected++;
    tst2 = (selected%3);
    if (tst2<tst) selected -=3;
   }
 else
   {
    selected--;
    if (selected<0) { selected += 3; return ; }
    tst2 = (selected%3);
    if (tst2>tst) selected +=3;
   }
}


void move_y(int arg) /* move selected +/- Y */
{
 int tst;
 int tst2;
 debug("move_y:%d\n", arg);
 tst = ((selected/3)%3);
 if (arg>0)
   {
    selected+=3;
    tst2 = ((selected/3)%3);
    if (tst2<tst) selected -=9;
   }
 else
   {
    selected-=3;
    if (selected<0) { selected +=9; return; }
    tst2 = ((selected/3)%3);
    if (tst2>tst) selected +=9;
   }
}


void move_z(int arg) /* move selected +/- Z */
{
 int tst;
 debug("move_z:%d\n", arg);
 tst = (selected/9);
 if (arg>0)
   {
    selected+=9;
    if (selected>=27) selected-=27;
   }
 else
   {
    selected-=9;
    if (selected<0) selected+=27;
   }
}


void set_at(int x, int y, int z, int value)
{
 board[x+3*y+9*z] = value;
 debug("set_at(%d,%d,%d=>%d,%d)\n",x,y,z,x+3*y+9*z,value);
}


int get_at(int x, int y, int z)
{
 debug("get_at(%d,%d,%d=>%d):%d\n",x,y,z,x+3*y+9*z,board[x+3*y+9*z]);
 return board[x+3*y+9*z];
}


int heuristic_count_moves_to_win(int player)
{
 int xc,yc,zc,i;
 int vec[3];
 int cmin;
 int min;
 debug("MOVES_TO_WIN FOR PLAYER: %d\n", player);
 min = 4;
 /* GO BY Z PARALLEL LINES */
 /* BY Z */
 for (xc=0;xc<3;xc++)
 for (yc=0;yc<3;yc++)
   {
    for (zc=0;zc<3;zc++) vec[zc] = get_at(xc,yc,zc);
    cmin=3;
    for (i=0;i<3;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* BY Y */
 for (xc=0;xc<3;xc++)
 for (zc=0;zc<3;zc++)
   {
    for (yc=0;yc<3;yc++) vec[yc] = get_at(xc,yc,zc);
    cmin=3;
    for (i=0;i<3;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* BY X */
 for (zc=0;zc<3;zc++)
 for (yc=0;yc<3;yc++)
   {
    for (xc=0;xc<3;xc++) vec[xc] = get_at(xc,yc,zc);
    cmin=3;
    for (i=0;i<3;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* PRZEKATNE 2D */
 /* WGLAB Z */
 for (zc=0;zc<3;zc++)
   {
    vec[0] = get_at(0,2, zc);
    vec[1] = get_at(1,1, zc);
    vec[2] = get_at(2,0, zc);
    cmin=3;
    for (i=0;i<3;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
    vec[0] = get_at(0,0, zc);
    vec[1] = get_at(1,1, zc);
    vec[2] = get_at(2,2, zc);
    cmin=3;
    for (i=0;i<3;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* WGLAB Y */
 for (yc=0;yc<3;yc++)
   {
    vec[0] = get_at(0,yc,2);
    vec[1] = get_at(1,yc,1);
    vec[2] = get_at(2,yc,0);
    cmin=3;
    for (i=0;i<3;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
    vec[0] = get_at(0,yc, 0);
    vec[1] = get_at(1,yc, 1);
    vec[2] = get_at(2,yc, 2);
    cmin=3;
    for (i=0;i<3;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* WGLAB X */
 for (xc=0;xc<3;xc++)
   {
    vec[0] = get_at(xc,2,0);
    vec[1] = get_at(xc,1,1);
    vec[2] = get_at(xc,0,2);
    cmin=3;
    for (i=0;i<3;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
    vec[0] = get_at(xc,0,0);
    vec[1] = get_at(xc,1,1);
    vec[2] = get_at(xc,2,2);
    cmin=3;
    for (i=0;i<3;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* PRZEKATNE 3D */
 vec[0] = get_at(0,0,0);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(2,2,2);
 cmin=3;
 for (i=0;i<3;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 cmin=6;
	 break;
	}
     if (vec[i]==player) cmin--;
    }
 if (cmin<min) min=cmin;
 vec[0] = get_at(2,0,0);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(0,2,2);
 cmin=3;
 for (i=0;i<3;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 cmin=6;
	 break;
	}
     if (vec[i]==player) cmin--;
    }
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,2,0);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(2,0,2);
 cmin=3;
 for (i=0;i<3;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 cmin=6;
	 break;
	}
     if (vec[i]==player) cmin--;
    }
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,0,2);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(2,2,0);
 cmin=3;
 for (i=0;i<3;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 cmin=6;
	 break;
	}
     if (vec[i]==player) cmin--;
    }
 if (cmin<min) min=cmin;
 /* KONIEC PRZEKATNYCH 3D */
 /* printf("MIN = %d\n", min);*/
 return min;
}


int heuristic_count_ways_to_win(int player)
{
 int ways;
 int vec[3];
 int xc,yc,zc,i;
 debug("WAYS_TO_WIN FOR PLAYER: %d\n", player);
 ways = 0;
 /* GO BY Z PARALLEL LINES */
 /* BY Z */
 for (xc=0;xc<3;xc++)
 for (yc=0;yc<3;yc++)
   {
    for (zc=0;zc<3;zc++) vec[zc] = get_at(xc,yc,zc);
    for (i=0;i<3;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* BY Y */
 for (xc=0;xc<3;xc++)
 for (zc=0;zc<3;zc++)
   {
    for (yc=0;yc<3;yc++) vec[yc] = get_at(xc,yc,zc);
    for (i=0;i<3;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* BY X */
 for (zc=0;zc<3;zc++)
 for (yc=0;yc<3;yc++)
   {
    for (xc=0;xc<3;xc++) vec[xc] = get_at(xc,yc,zc);
    for (i=0;i<3;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* BY Y */
 /* PRZEKATNE 2D */
 /* WGLAB Z */
 for (zc=0;zc<3;zc++)
   {
    vec[0] = get_at(0,2, zc);
    vec[1] = get_at(1,1, zc);
    vec[2] = get_at(2,0, zc);
    for (i=0;i<3;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
    vec[0] = get_at(0,0, zc);
    vec[1] = get_at(1,1, zc);
    vec[2] = get_at(2,2, zc);
    for (i=0;i<3;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* WGLAB Y */
 for (yc=0;yc<3;yc++)
   {
    vec[0] = get_at(0,yc,2);
    vec[1] = get_at(1,yc,1);
    vec[2] = get_at(2,yc,0);
    for (i=0;i<3;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
    vec[0] = get_at(0,yc, 0);
    vec[1] = get_at(1,yc, 1);
    vec[2] = get_at(2,yc, 2);
    for (i=0;i<3;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* WGLAB X */
 for (xc=0;xc<3;xc++)
   {
    vec[0] = get_at(xc,2,0);
    vec[1] = get_at(xc,1,1);
    vec[2] = get_at(xc,0,2);
    for (i=0;i<3;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
    vec[0] = get_at(xc,0,0);
    vec[1] = get_at(xc,1,1);
    vec[2] = get_at(xc,2,2);
    for (i=0;i<3;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* PRZEKATNE 3D */
 vec[0] = get_at(0,0,0);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(2,2,2);
 for (i=0;i<3;i++)
     if (vec[i] && vec[i]!=player)
	{
	 ways--;
	 break;
	}
 ways++;
 vec[0] = get_at(2,0,0);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(0,2,2);
 for (i=0;i<3;i++)
     if (vec[i] && vec[i]!=player)
	{
	 ways--;
	 break;
	}
 ways++;
 vec[0] = get_at(0,2,0);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(2,0,2);
 for (i=0;i<3;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 ways--;
	 break;
	}
    }
 ways++;
 vec[0] = get_at(0,0,2);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(2,2,0);
 for (i=0;i<3;i++)
     if (vec[i] && vec[i]!=player)
	{
	 ways--;
	 break;
	}
 ways++;
 /* KONIEC PRZEKATNYCH 3D */
 /* printf("WAYS = %d\n", ways); */
 return ways;
}


void clear_board()
{
 int i;
 debug("clear_board()\n");
 for (i=0;i<27;i++) board[i] = 0;
}

int get_current_heuristics(int*,int*,int*,int*,int,int);

int try_random_move(int player)
{
 int i,j;
 int* at;
 if (Random(54)) return 0;
 /* printf("Doing random move!\n"); */
 j=0;
 for (i=0;i<27;i++) if (!board[i]) j++;
 at = (int*)malloc(j<<2);
 j=0;
 for (i=0;i<27;i++) if (!board[i])
   {
    at[j] = i;
    j++;
   }
 board[at[Random(j)]] = player;
 free(at);
 return 1;
}


int forseen_best_move(int h1, int h2, int h3, int h4, int cpu, int opp)
{
 int i,j,c;
 int *hx1,*hx2,*hx3,*hx4, *at,*val;
 int *gmove;
 int nmoves,min,hmov,cmov;
 nmoves=0;
 Debug("foreseen_best_move()\n");
 for (i=0;i<27;i++) if (!board[i]) nmoves++;
 hx1 = (int*)malloc(nmoves<<2);
 hx2 = (int*)malloc(nmoves<<2);
 hx3 = (int*)malloc(nmoves<<2);
 hx4 = (int*)malloc(nmoves<<2);
 at  = (int*)malloc(nmoves<<2);
 val = (int*)malloc(nmoves<<2);
 j=0;
 for (i=0;i<27;i++)
   if (!board[i]) 		/* ten ruch mozliwy */
     {
      board[i] = cpu;		/* zrob go */
      get_current_heuristics(&hx1[j], &hx2[j], &hx3[j], &hx4[j], cpu, opp);
      				/* wez ststystyki po ruchu */
      at[j] = i;		/* gdzie */
      val[j] = 1;
      board[i] = 0;
      j++;			/* ilosc dozwolonych ruchow */
     }
 c=try_random_move(cpu);
 if (c)
   {
    get_current_heuristics(&hx1[0], &hx2[0], &hx3[0], &hx4[0], cpu, opp);
    if (hx1[0]<=0)
      {
       printf("HEURISTICS BEFORE: h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
       printf("WIN (RANDOM) HEURISTICS: hx1=%d,hx2=%d,hx3=%d,hx4=%d,at=%d,val=%d,i=%d\n",hx1[i],hx2[i],hx3[i],hx4[i],at[i],val[i],i);
       printf("ONBOARD:%d>> (%d,%d,%d)\n",i,i%3,(i/3)%3,i/9);
       printf("MSI COMPUTED RESULT\n");
       free(at);
       free(val);
       free(hx1);
       free(hx2);
       free(hx3);
       free(hx4);
       /* VICTORY HERE! */
       return 1;
      }
    return 0;
   }
 /* c=0;
 for (i=0;i<27;i++) if (board[i]==1) c++;
 if (c==1 && board[13]==1) 
	  printf("CPU: Uzytkownik zaczal od srodkowego pola, bedzie trudno wygrac!\n"); */
 Debug("h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
 for (i=0;i<nmoves;i++)
   Debug("hx1=%d,hx2=%d,hx3=%d,hx4=%d,at=%d,val=%d,i=%d\n",hx1[i],hx2[i],hx3[i],hx4[i],at[i],val[i],i);
 Debug("matrix:\n");
 for (i=0;i<nmoves;i++) if (hx1[i]<=0) /* gdy komputer moze wygrac */
   {
    board[at[i]] = cpu;
    printf("HEURISTICS BEFORE: h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
    printf("WIN HEURISTICS: hx1=%d,hx2=%d,hx3=%d,hx4=%d,at=%d,val=%d,i=%d\n",hx1[i],hx2[i],hx3[i],hx4[i],at[i],val[i],i);
    printf("ONBOARD:%d>> (%d,%d,%d)\n",i,i%3,(i/3)%3,i/9);
    printf("MSI COMPUTED RESULT\n");
    free(at);
    free(val);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    /* VICTORY HERE! */
    return 1;
   }
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 /* CO Z PRZECIWNIKIEM PO RUCHU */
 for (i=0;i<nmoves;i++) if (hx2[i]<=1) val[i] = 0;	/* GDY PO TYM RUCH PRZECIWNIK MOZE WYGRAC */
 c=0;
 for (i=0;i<nmoves;i++) if (val[i]) c++;
 if (c==1) for (i=0;i<nmoves;i++) if (val[i])   /* RATUJ SIE PRZED PRZEGRANA */
   {
    board[at[i]] = cpu;
    free(at);
    free(val);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    /* UARATOWANY? */
    return 0;
   }
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 /* ZEBY PO RUCHU (JAK NIE DA SIE WYGRAC I NIE MA BEZPOSREDNIEGO ZAGROZENIA PRZEGRANA)
  * BYC JAK NAJBLIZEJ ZWYCIESTWA */
 min = 6;
 for (i=0;i<nmoves;i++) if (hx1[i]<min) min = hx1[i];
 for (i=0;i<nmoves;i++) if (hx1[i]>min) val[i] = 0;
 c=0;
 for (i=0;i<nmoves;i++) if (val[i]) c++;
 if (c==1) for (i=0;i<nmoves;i++) if (val[i])   /* JAK TYLKO JEDEN MOZLIWY RUCH */
   {
    board[at[i]] = cpu;
    free(at);
    free(val);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    /* POWINNA BYC DOBRA SYTUACJA */
    return 0;
   }
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 /* ABY JAK NAJWIECEJ MOZLIWOSCI RUCHU PO AKTUALNYM*/
 cmov=0;
 for (i=0;i<nmoves;i++) if (hx3[i]>cmov) cmov = hx3[i];
 for (i=0;i<nmoves;i++) if (hx3[i]<cmov) val[i] = 0;
 c=0;
 for (i=0;i<nmoves;i++) if (val[i]) c++;
 if (c==1) for (i=0;i<nmoves;i++) if (val[i])   /* JAK TYLKO JEDEN MOZLIWY RUCH */
   {
    board[at[i]] = cpu;
    free(at);
    free(val);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    return 0;
   }
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 /* ABY CZLOWIEK MAIL JAK NAJMNIEJ MOZLIWOSCI RUCH PO AKTUALNYM */
 hmov=50;
 for (i=0;i<nmoves;i++) if (hx4[i]<hmov) hmov = hx4[i];
 for (i=0;i<nmoves;i++) if (hx4[i]>hmov) val[i] = 0;
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 c=0;
 for (i=0;i<nmoves;i++) if (val[i]) c++;
 if (c==1) for (i=0;i<nmoves;i++) if (val[i])   /* JAK TYLKO JEDEN MOZLIWY RUCH */
   {
    board[at[i]] = cpu;
    free(at);
    free(val);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    return 0;
   }
 Debug("\nThere are %d possible good moves\n", c);
 if (c==0) /* NIE MA DOBREGO RUCHU, PRAKTYCZNIE PRZEGRANA, ZROB COKOLWIEK */
 {
  Debug("SYSTUACJA JEST BARDZO ZLA!!!\n");
  hmov=4;
  for (i=0;i<nmoves;i++) if (hx1[i]<hmov)  hmov = hx1[i];
  for (i=0;i<nmoves;i++) if (hx1[i]==hmov) { board[at[i]] = cpu; break; }
  /* board[at[Random(nmoves)]] = cpu; */
 }
 else
   {
    gmove = (int*)malloc(c<<2);
    j=0;
    for (i=0;i<nmoves;i++)
      {
       if (val[i])
         {
          gmove[j] = at[i];
	  j++;
         }
      }
    board[gmove[Random(c)]] = cpu;
    free(gmove);
   }
 free(at);
 free(val);
 free(hx1);
 free(hx2);
 free(hx3);
 free(hx4);
 return 0;
}


int get_current_heuristics(int* h1, int* h2, int* h3, int* h4, int cpu, int opp)
{
 *h1 = heuristic_count_moves_to_win(cpu);				/*  CPU  */
 *h2 = heuristic_count_moves_to_win(opp);				/* HUMAN */
 *h3 = heuristic_count_ways_to_win(cpu);				/*  CPU  */
 *h4 = heuristic_count_ways_to_win(opp);	        		/* HUMAN */
 debug("H1=%d, H2=%d, H3=%d, H4=%d\n", *h1, *h2, *h3, *h4);
 if (*h2<=0) return -1;
 else return 0;
}


void cpu_msi_move()
{
 int h1,h2,h3,h4,ret;
 debug("CPU_MSI_MOVE()\n");
 end_cond=0;
 /*set_at(Random(3),Random(3),Random(3), 2);*/
 ret = get_current_heuristics(&h1, &h2, &h3, &h4, 2, 1);
 if (ret==-1)
   {
    printf("HEURISTICS WHEN DEFETED: h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
    printf("PANIC: MSI COMPUTED RESULT, CPU LOST\n");
    printf("%c%c%c\nCPU: I HAVE LOST!%c%c%c\n",7,7,7,7,7,7);
    end_cond=1;
    hum_wins++;
    stats();
    return ;
   }
 ret = forseen_best_move(h1,h2,h3,h4, 2, 1);
 if (ret)
   {
    printf("%c%c%c\nCPU: I HAVE WIN!%c%c%c\n",7,7,7,7,7,7);
    end_cond=1;
    cpu_wins++;
    stats();
   }
 /*set_at(Random(3),Random(3),Random(3), 2);*/
}


void DrawScene()		/* Rysuje cala scene */
{
 int i;
 printf("\n");
 printf("/---+---+---\\\n|");
 for (i=0;i<27;i++)
   {
    if ((i%9) && !(i%3)) printf("|");
    if (i==selected && board[i]==1) printf("X");
    if (i!=selected && board[i]==1) printf("x");
    if (i==selected && board[i]==2) printf("O");
    if (i!=selected && board[i]==2) printf("o");
    if (i==selected && board[i]==0) printf("+");
    if (i!=selected && board[i]==0) printf(" ");
    if ((i%9)==8) printf("|\n|");
   }
 printf("---+---+---/\n");
 printf("\n");
}


void help()
{
 printf("Program by Morgoth DBMA - Lukasz Gryglicki MiNI M1: morgothdbma@o2.pl; 693582014\n");
 printf(
	"Z             REFRESH SCENE\n"
	"H             HELP\n"
	"J/L           MOVE SELECTED +/-X\n"
	"K/I           MOVE SELECTED +/-Y\n"
	"M/O           MOVE SELECTED +/-Z\n"
	"SPACE         MARK HERE\n"
	"Q             EXIT PROGRAM\n");
}


int main(int lb, char** par)		/* tutaj UNIX przekazuje sterowanie do Xengine */
{
  int zn;
  help();
  init();
  Randomize();
  if (Random(20)>=11) cpu_msi_move();
  while (!done)
    {
     DrawScene();
     printf("\n>> ");
     zn = mgetch();
     printf("\n");
     if (zn>='A' && zn <= 'Z') zn += 0x20;
     if (zn=='q') done=1;
     if (zn=='k') move_z(1);
     if (zn=='i') move_z(-1);
     if (zn=='j') move_x(-1);
     if (zn=='l') move_x(1);
     if (zn=='m') move_y(-1);
     if (zn=='o') move_y(1);
     if (zn=='h') help();
     if (zn=='z') DrawScene();
     if (zn==' ')
                {
	         DrawScene();
		 if (!board[selected])
		   {
		    board[selected] = 1;
		    cpu_msi_move();
		   }
		 DrawScene();
		 if (end_cond)
		   {
		    clear_board();
  		    if (Random(20)>=11) cpu_msi_move();
		    printf("\n%c%c%c\n", 7,7,7);
		    end_cond=0;
		   }
		}
    }
 printf("\n%c%c%cMSI CPU ShutDown\n\n%c%c%c\n", 7,7,7,7,7,7);
 return 0;
}

/* CopyLeft Morgoth DBMA, +48693582014, morgothdbma@o2.pl, heroine@o2.pl */
