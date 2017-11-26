#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

/* */
/*#define DEBUG 1*/
#define START 0
#define STORE 1
#define BLACK       0
#define WHITE       1
#define SEA_LIT     2
#define SEA_UNLIT   3
#define SKY         4
#define NSTRIP 8
#define BAND_BASE   5
#define N_BANDS     3
#define BAND_SIZE   80
#define DEF_COL     (BAND_BASE + (N_BANDS * BAND_SIZE))
#define MIN_COL     (BAND_BASE + (N_BANDS * 2))
#define COL_RANGE   65535
#define TRUE	    1
#define FALSE       0
#define Gun unsigned short
#define Col int
#define PI 3.14159265
#define SIDE 1.0
#define fold_param p
#define GW 600
#define GH 450
#define TWOPI 6.2831853
#ifdef DEBUG
#define TRACE printf("file: %s line: %d\n", __FILE__,__LINE__);
#endif
#ifndef DEBUG
#define TRACE
#endif

typedef struct _Graph
{
  int graph_height; 
  int graph_width;     
  int width;            
  float ambient;        
  float contrast;       
  float contour;
  float vfract;         
  float altitude;
  float distance;
  double phi;           
  double alpha;         
  float base_shift;    
  float sealevel;
  double stretch;       
  int n_col;
  int band_size;
  int levels;
  int stop;
  int map;
  int reflec;
  int repeat;
  int pos;
  int scroll;
} Graph;


typedef struct _Strip
{
  struct _Fold *f;
  float *d;
} Strip;

typedef struct _Fold
{
  int level;                
  int count;                
  float length;            
  float scale;             
  float midscale;          
  struct _Parm *p;           
  struct _Strip *s[NSTRIP];  
  struct _Strip *save;       
  int stop;                 
  int state;                
  struct _Fold *next;        
  struct _Fold *parent;      
} Fold;

typedef struct _Parm
{
  float mean;              
  int rg1;                  
  int rg2;
  int rg3;
  int cross;                
  int force_front;          
  int force_back;           
  float forceval;          
  float mix;                
  float midmix;             
  float fdim;
} Parm;

typedef struct _Uni_save
{
  float u[98];
  float c;
  float cd;
  float cm;
  int ui;
  int uj;
} Uni_save;

Graph g;
Parm p;
Fold *top = NULL;
Uni_save uni_data;
int base=0;
unsigned long pix[GW+1][GH+1];
unsigned char* pex  = NULL;
unsigned long table[0x100];
int plot_x, plot_y1, plot_y2;
unsigned long plot_col;
int plot_saved=FALSE;
float *shadow=NULL;             
float *a_strip=NULL, *b_strip=NULL;  
time_t t1, t2;
int fps;
int gh1, gh2;
double cos_phi;
double sin_phi;
double tan_phi;
double x_fact;
double y_fact;
double vangle;
double vscale;
double tan_vangle;                                     
float varience;
float delta_shadow;
float shift;
double shadow_slip;
double shadow_register;
float viewpos;       
float viewheight;   
float focal;
float vstrength;
float lstrength;
unsigned int graph_width  = 500;
unsigned int graph_height = 500;


void init_parameters(Graph* g, Parm* f)
{
  TRACE
  g->graph_height=GH;
  g->graph_width=GW;
  g->levels = 10;
  g->stop=2;
  g->n_col=DEF_COL;
  g->band_size=BAND_SIZE;
  g->ambient=0.3;
  g->contrast=1.0;
  g->contour=0.3;
  g->vfract=0.6;
  g->altitude=2.5;
  g->distance=4.0;
  g->phi=(40.0 * PI)/180.0;
  g->alpha=0.0;
  g->base_shift=0.5;
  g->sealevel=0.0;
  g->stretch=0.6;
  g->map=FALSE;
  g->reflec=TRUE;
  g->repeat=20;
  g->pos=0;
  g->scroll=0;
  f->mean=0;
  f->rg1=FALSE;
  f->rg2=FALSE;
  f->rg3=TRUE;
  f->cross=TRUE;
  f->force_front=TRUE;
  f->force_back=FALSE;
  f->forceval=-1.0;
  f->fdim = 0.65;
  f->mix   =0.0;
  f->midmix=0.0;
}


void set_clut(int max_col,Gun* red,Gun* green,Gun* blue)
{
  int band,shade;
  float top, bot;
  float intensity;
  int tmp;
  float rb[N_BANDS];
  float gb[N_BANDS];
  float bb[N_BANDS];
  TRACE
  rb[0] = 0.450; rb[1] = 0.600; rb[2] = 1.000;
  gb[0] = 0.500; gb[1] = 0.600; gb[2] = 1.000;
  bb[0] = 0.333; bb[1] = 0.000; bb[2] = 1.000;
  red[BLACK]       = 0;
  green[BLACK]     = 0;
  blue[BLACK]      = 0;
  red[WHITE]       = COL_RANGE;
  green[WHITE]     = COL_RANGE;
  blue[WHITE]      = COL_RANGE;
  red[SKY]         = 0.404*COL_RANGE;
  green[SKY]       = 0.588*COL_RANGE;
  blue[SKY]        = COL_RANGE;
  red[SEA_LIT]     = 0;
  green[SEA_LIT]   = 0.500*COL_RANGE;
  blue[SEA_LIT]    = 0.700*COL_RANGE;
  red[SEA_UNLIT]   = 0;
  green[SEA_UNLIT] = ((g.ambient+(g.vfract/(1.0+g.vfract)))*0.500)*COL_RANGE;
  blue[SEA_UNLIT]  = ((g.ambient+(g.vfract/(1.0+g.vfract)))*0.700)*COL_RANGE;
  if( MIN_COL > max_col )
  {
    printf("set_clut: less than the minimum number of colours available\n");
    exit(1);
  }
  while( (BAND_BASE +g.band_size*N_BANDS) > max_col )
  {
    g.band_size--;
  }
  for( band=0 ; band<N_BANDS; band++)
  {
    for(shade=0 ; shade < g.band_size ; shade++)
    {
      if( (BAND_BASE + (band*g.band_size) + shade) >= max_col )
      {
        printf("INTERNAL ERROR, overflowed clut\n");
        exit(1);
      }
      top = rb[band];
      bot = g.ambient * top;
      intensity = bot + ((shade * (top - bot))/(g.band_size-1));
      tmp = COL_RANGE * intensity;
      if (tmp < 0)
      {
        printf("set_clut: internal error: invalid code %d\n",tmp);
        exit(2);
      }
      if( tmp > COL_RANGE )
      {
        tmp = COL_RANGE;
      }
      red[BAND_BASE + (band*g.band_size) + shade] = tmp;
      top = gb[band];
      bot = g.ambient * top;
      intensity = bot + ((shade * (top - bot))/(g.band_size-1));
      tmp = COL_RANGE * intensity;
      if (tmp < 0)
      {
        printf("set_clut: internal error: invalid code %d\n",tmp);
        exit(2);
      }
      if( tmp > COL_RANGE )
      {
        tmp = COL_RANGE;
      }
      green[BAND_BASE + (band*g.band_size) + shade] = tmp;
      top = bb[band];
      bot = g.ambient * top;
      intensity = bot + ((shade * (top - bot))/(g.band_size-1));
      tmp = COL_RANGE * intensity;
      if (tmp < 0)
      {
        printf("set_clut: internal error: invalid code %d\n",tmp);
        exit(2);
      }
      if( tmp > COL_RANGE )
      {
        tmp = COL_RANGE;
      }
      blue[BAND_BASE + (band*g.band_size) + shade] = tmp;
    }
  }
}

float uni()
{
	register float luni;
  TRACE
	luni = uni_data.u[uni_data.ui] - uni_data.u[uni_data.uj];
	if (luni < 0.0)
		luni += 1.0;
	uni_data.u[uni_data.ui] = luni;
	if (--uni_data.ui == 0)
		uni_data.ui = 97;
	if (--uni_data.uj == 0)
		uni_data.uj = 97;
	if ((uni_data.c -= uni_data.cd) < 0.0)
		uni_data.c += uni_data.cm;
	if ((luni -= uni_data.c) < 0.0)
		luni += 1.0;
	return ((float) luni);
}


void rstart(int i,int j,int k,int l)
{
	int ii, jj, m;
	float s, t;
  TRACE
	for (ii = 1; ii <= 97; ii++) {
		s = 0.0;
		t = 0.5;
		for (jj = 1; jj <= 24; jj++) {
			m = ((i*j % 179) * k) % 179;
			i = j;
			j = k;
			k = m;
			l = (53*l+1) % 169;
			if (l*m % 64 >= 32)
				s += t;
			t *= 0.5;
		}
		uni_data.u[ii] = s;
	}
	uni_data.c  = 362436.0   / 16777216.0;
	uni_data.cd = 7654321.0  / 16777216.0;
	uni_data.cm = 16777213.0 / 16777216.0;
	uni_data.ui = 97;
	uni_data.uj = 33; 
}

void seed_uni(int ijkl)
{
 int i, j, k, l, ij, kl;
  TRACE
 if (ijkl == 0)
   {
    ijkl = time((time_t *) 0);
    ijkl %= 900000000;
   }
 if ((ijkl < 0) || (ijkl > 900000000))
   {
    printf("seed_uni: ijkl = %d -- out of range\n\n", ijkl);
    exit(3);
   }
	ij = ijkl/30082;
	kl = ijkl - (30082 * ij);
	i = ((ij/177) % 177) + 2;
	j = (ij % 177) + 2;
	k = ((kl/169) % 178) + 1;
	l = kl % 169;
        rstart(i, j, k, l);
}

float gaussian()
{
	register double ran1, ran2;
  TRACE
	do {
		ran1 = (double) uni();
	} while (ran1 == 0.);
	ran2 = (double) uni();
	return (float)(sqrt(-2.*log(ran1))*cos(TWOPI*ran2));
}

void vside_update(Fold *fold,float scale, float mix, Strip *a)
{
  int i;
  int count=fold->count;
  float w;
  float *mp;
  TRACE
  if( !a ) return;
  w = (1.0 - mix)*0.5;
  mp=a->d;
  if( mix <= 0.0){
    for(i=0; i<count-2; i+=2)
    {
      mp[1] = 0.5 * ( mp[0] + mp[2] )
            + (scale * gaussian());
      mp+=2;
    }
  }else if(mix >= 1.0){
    for(i=0; i<count-2; i+=2)
    {
      mp[1] = mp[1]
            + (scale * gaussian());
      mp+=2;
    }
  }else{
    for(i=0; i<count-2; i+=2)
    {
      mp[1] = (mix * mp[1]) + w * ( mp[0] + mp[2] )
            + (scale * gaussian());
      mp+=2;
    }
  }
}    


void hside_update(Fold *fold,float scale, float mix, Strip *a, Strip *b, Strip *c)
{
  int i;
  int count=fold->count;
  float w;
  float *mp, *lp, *rp;
  TRACE
  if( !a || !c ) return;
  if( !b )
  {
    printf("x_update: attempt to update NULL strip\n");
    exit(1);
  }
  w = (1.0 - mix)*0.5;
  mp=b->d;
  lp=a->d;
  rp=c->d;
  if( mix <= 0.0 ){
    for(i=0; i<count; i+=2)
    {
      mp[0] = 0.5 * ( lp[0] + rp[0] )
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }else if(mix >= 1.0){
    for(i=0; i<count; i+=2)
    {
      mp[0] = mp[0]
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }else{
    for(i=0; i<count; i+=2)
    {
      mp[0] = (mix * mp[0]) + w * ( lp[0] + rp[0] )
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }
}    

void x_update(Fold *fold,float scale, float mix, Strip *a, Strip *b, Strip *c)
{
  int i;
  int count=fold->count;
  float w;
  float *mp, *lp, *rp;
  TRACE
  if( !a || !c ) return;
  if( !b )
  {
    printf("x_update: attempt to update NULL strip\n");
    exit(1);
  }
  w = (1.0 - mix)*0.25;
  mp=b->d;
  lp=a->d;
  rp=c->d;
  if( mix <= 0.0 ){
    for(i=0; i<count-2; i+=2)
    {
      mp[1] = 0.25 * ( lp[0] + rp[0] + lp[2] + rp[2])
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }else if( mix >= 1.0 ){
    for(i=0; i<count-2; i+=2)
    {
      mp[1] = mp[1]
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }else{
    for(i=0; i<count-2; i+=2)
    {
      mp[1] = (mix * mp[1]) + w * ( lp[0] + rp[0] + lp[2] + rp[2])
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }
}

void v_update(Fold *fold,float scale, float mix, Strip *a, Strip *b, Strip *c)
{
  int i;
  int count=fold->count;
  float w, we;
  float *mp, *lp, *rp;
  TRACE
  if( !a || !c ) return;
  if( !b )
  {
    printf("v_update: attempt to update NULL strip\n");
    exit(1);
  }
  w = (1.0 - mix)*0.25;
  we = (1.0 - mix)*0.5;
  mp=b->d;
  lp=a->d;
  rp=c->d;
  if( mix <= 0.0){
    mp[0] = 0.5 * ( lp[1] + rp[1] )
            + (scale * gaussian());
    mp++;
    lp++;
    rp++;
    for(i=1; i<count-3; i+=2)
    {
      mp[1] = 0.25 * ( lp[0] + rp[0] + lp[2] + rp[2] )
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
    mp[1] = 0.5 * ( lp[0] + rp[0] )
            + (scale * gaussian());
  }else if(mix >= 1.0){
    for(i=0; i<count; i+=2)
    {
      mp[0] = mp[0]
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }else{
    mp[0] = (mix * mp[0]) + we * ( lp[1] + rp[1] )
            + (scale * gaussian());
    mp++;
    lp++;
    rp++;
    for(i=1; i<count-3; i+=2)
    {
      mp[1] = (mix * mp[1]) + w * ( lp[0] + rp[0] + lp[2] + rp[2] )
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
    mp[1] = (mix * mp[1]) + we * ( lp[0] + rp[0] )
            + (scale * gaussian());
  }
}    

void t_update(Fold *fold,float scale, float mix, Strip *a, Strip *b, Strip *c)
{
  int i;
  int count=fold->count;
  float w, we;
  float *mp, *lp, *rp;
  float third=(1.0/3.0);
  TRACE
  if( !a || !c ) return;
  if( !b )
  {
    printf("t_update: attempt to update NULL strip\n");
    exit(1);
  }
  w = (1.0 - mix)*0.25;
  we = (1.0 - mix)*third;
  mp=b->d;
  lp=a->d;
  rp=c->d;

  if( mix <= 0.0){
    mp[0] = third * ( lp[0] + rp[0] + mp[1] )
            + (scale * gaussian());
    mp++;
    lp++;
    rp++;
    for(i=1; i<count-3; i+=2)
    {
      mp[1] = 0.25 * ( lp[1] + rp[1] + mp[0] + mp[2] )
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
    mp[1] = third * ( lp[1] + rp[1] + mp[0] )
          + (scale * gaussian());
  }else if(mix >= 1.0){
    for(i=0; i<count; i+=2)
    {
      mp[0] = mp[0]
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }else{
    mp[0] = (mix * mp[0]) + we * ( lp[0] + rp[0] + mp[1] )
            + (scale * gaussian());
    mp++;
    lp++;
    rp++;
    for(i=1; i<count-3; i+=2)
    {
      mp[1] = (mix * mp[1]) + w * ( lp[1] + rp[1] + mp[0] + mp[2] )
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
    mp[1] = (mix * mp[1]) + we * ( lp[1] + rp[1] + mp[0] )
          + (scale * gaussian());
  }
}    

void p_update(Fold *fold,float scale, float mix, Strip *a, Strip *b, Strip *c)
{
  int i;
  int count=fold->count;
  float w;
  float *mp, *lp, *rp;
  TRACE
  if( !a || !b ) return;
  if( !c )
  {
    vside_update(fold,scale,mix,b);
    return;
  }
  w = (1.0 - mix)*0.25;
  mp=b->d;
  lp=a->d;
  rp=c->d;
  if( mix <= 0.0 ){
    for(i=0; i<count-2; i+=2)
    {
      mp[1] = 0.25 * ( lp[1] + rp[1] + mp[0] + mp[2] )
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }else if(mix >= 1.0){
    for(i=0; i<count-2; i+=2)
    {
      mp[1] = mp[1]
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }else{
    for(i=0; i<count-2; i+=2)
    {
      mp[1] = (mix * mp[1]) + w * ( lp[1] + rp[1] + mp[0] + mp[2] )
            + (scale * gaussian());
      mp+=2;
      lp+=2;
      rp+=2;
    }
  }
}    


void free_strip(Strip *p) 
{
  TRACE
  if( p->d )
  {
    free(p->d);
    p->d = NULL;
  }
  free(p);
}



Strip *make_strip(Fold *f) 
{
  Strip *p;
  int n;
  TRACE
  p = (Strip *) malloc( sizeof(Strip) );
  if( p == NULL )
  {
    printf("make_strip: malloc failed\n");
    exit(1);
  }
  p->f = f;
  n = f->count;
  p->d = (float*)malloc( n * sizeof(float) );
  if( p->d == NULL )
  {
    printf("make_strip: malloc failed\n");
    exit(1);
  }
  return(p);
}

Strip *set_strip(Fold *f, float value)
{
  int i;
  Strip *s;
  float *h;
  int count;
  TRACE
  s = make_strip(f);
  h = s->d;
  count = f->count;
  for( i=0 ; i < count ; i++)
  {
    *h = value;
    h++;
  }
  return(s);
}

Strip *double_strip(Strip *s) 
{
  Strip *p;
  float *a, *b;
  int i;
  int count;
  TRACE
  p = make_strip(s->f->parent);
  a = s->d;
  b = p->d;
  count = s->f->count;
  for(i=0; i < count-1; i++)
  {
    *b = *a;
    a++;
    b++;
    *b = 0.0;
    b++;
  }
  *b = *a;
  return(p);
}

Strip *random_strip(Fold *f)
{
  Strip *result;
  int i, count;
  TRACE
  result=make_strip(f);
  count = f->count;
  for( i=0 ; i < count ; i++)
  {
    result->d[i] = f->p->mean + (f->scale * gaussian());
  }
  return(result);
}

Strip *next_strip(Fold *fold) 
{
  Strip *result=NULL;
  Strip *tmp;
  Strip **t;
  int i, iter;
  int count=fold->count;
  TRACE
  if( fold->level == fold->stop)
  {
    result=random_strip(fold);
  }else{
    while( result == NULL )
    {
      switch(fold->state)
      {
        case START:
          t=fold->s;
          tmp =next_strip(fold->next);
          t[0] = double_strip(tmp);
          free_strip(tmp);
          t[1]=set_strip(fold,0.0);
          if( ! t[2] )
          {
            t[2]=t[0];
            tmp =next_strip(fold->next);
            t[0] = double_strip(tmp);
            free_strip(tmp);
          }
          x_update(fold,fold->midscale,0.0,t[0],t[1],t[2]);
          if(fold->p->rg1)
          {
            if( t[3] == NULL )
            {
              v_update(fold,fold->midscale,1.0,t[1],t[2],t[1]);
            }else{
              v_update(fold,fold->midscale,fold->p->midmix,t[1],t[2],t[3]);
            }
            t+=2;
          }
          if( fold->p->cross )
          {
            t_update(fold,fold->scale,0.0,t[0],t[1],t[2]);
            p_update(fold,fold->scale,0.0,t[1],t[2],t[3]);
            t+=2;
          }else{
            hside_update(fold,fold->scale,0.0,t[0],t[1],t[2]);
            vside_update(fold,fold->scale,0.0,t[2]);
            t+=2;
          }
          if(fold->p->rg2)
          {
            if( fold->p->cross )
            {
              if( t[2] == NULL )
              {
                p_update(fold,fold->scale,fold->p->mix,t[0],t[1],t[0]);
              }else{
                p_update(fold,fold->scale,fold->p->mix,t[0],t[1],t[2]);
              }
            }else{
              vside_update(fold,fold->scale,fold->p->mix,t[1]);
            }
          }
          t++;
          if(fold->p->rg3)
          {
            if( t[2] == NULL )
            {
              t_update(fold,fold->scale,1.0,t[0],t[1],t[0]);
            }else{
              t_update(fold,fold->scale,fold->p->mix,t[0],t[1],t[2]);
            }
            t++;
          }
          result=t[1];
          fold->save=t[0];
          t[0]=t[1]=NULL;
          fold->state = STORE;
          break;
        case STORE:
          result = fold->save;
          fold->save=NULL;
          for(i=NSTRIP-1;i>1;i--)
          {
            fold->s[i] =fold->s[i-2];
          }
          fold->s[0] = fold->s[1]=NULL;
          fold->state = START;
          break;
          /* }}} */
          printf("next_strip: invalid state level %d state %d\n",
               fold->level,fold->state);
          exit(3);
      }
    }
  }
  iter = fold->level - fold->stop;
  if( fold->p->force_front > iter){
   result->d[0] = fold->p->forceval;
  }
  if( fold->p->force_back > iter){
    result->d[count-1] = fold->p->forceval;
  }
  return(result);
}

float* extract(Strip* s)
{
  int i;
  float *p;
  TRACE
  p = s->d;
  free(s);
  for(i=0 ; i<g.width; i++ )
  {
    p[i] = shift + (vscale * p[i]);
  }
  return(p);
}

void reset_fold(Fold *ff) 
{
  float scale, midscale;
  double root2;
  TRACE
  root2=sqrt((double) 2.0 );
  scale = pow((double) ff->length, (double) (2.0 * ff->p->fdim));
  midscale = pow((((double) ff->length)*root2), (double) (2.0 * ff->p->fdim));
  ff->scale = scale;
  ff->midscale = midscale;
  if( ff->next ){
    reset_fold(ff->next);
  }
}

Fold *make_fold(Fold *parent,Parm *param, int levels, int stop, float length) 
{
  Fold *p;
  int i;
  TRACE
  if( (levels < stop) || (stop<0) )
  {
    printf("make_fold: invalid parameters\n");
    printf("make_fold: levels = %d , stop = %d \n",levels,stop);
    exit(1);
  }
  p = (Fold *)malloc(sizeof(Fold));
  if( p == NULL )
  {
    fprintf(stderr,"make_fold: malloc failed\n");
    exit(1);
  }
  p->length=length;
  p->level = levels;
  p->count = (1 << levels) +1;
  p->stop = stop;
  p->state = START;
  p->save =NULL;
  p->p = param;
  for(i=0;i<NSTRIP;i++)
  {
    p->s[i] = NULL;
  }
  p->parent=parent;
  p->next = NULL;
  reset_fold(p);
  if( levels > stop )
  {
    p->next = make_fold(p,param,(levels-1),stop,(2.0*length));
  }else{
    p->next = NULL;
  }
  return( p );
}

void init_artist_variables()
{
  float dh, dd;
  int pwidth; 
  TRACE
  g.width= (1 << g.levels)+1;
  pwidth= (1 << (g.levels - g.stop))+1;
  cos_phi = cos( g.phi );
  sin_phi = sin( g.phi );
  tan_phi = tan( g.phi );
  x_fact = cos_phi* cos(g.alpha);
  y_fact = cos_phi* sin(g.alpha);
  vscale = g.stretch * pwidth; 
  delta_shadow = tan_phi /cos(g.alpha);
  shadow_slip = tan(g.alpha);
  varience = pow( SIDE ,(2.0 * fold_param.fdim));
  varience = vscale * varience ;
  shift = g.base_shift * varience;
  varience = varience + shift;
  viewheight = g.altitude * g.width;
  viewpos = - g.distance * g.width;
  dh = viewheight;
  dd = (g.width / 2.0) - viewpos;
  focal = sqrt( (dd*dd) + (dh*dh) );
  tan_vangle = (double) ((double)(viewheight-g.sealevel)/(double) - viewpos);
  vangle = atan ( tan_vangle );
  vangle -= atan( (double) (g.graph_height/2) / focal ); 
  top=make_fold(NULL, &fold_param, g.levels,g.stop,(SIDE / pwidth));
  shadow = extract(next_strip(top));
  a_strip = extract( next_strip(top) ); 
  b_strip = extract( next_strip(top) );
  vstrength = g.vfract * g.contrast /( 1.0 + g.vfract );
  lstrength = g.contrast /( 1.0 + g.vfract );
  if( g.repeat >= 0 ){
    g.pos=0;
  }else{
    g.pos=g.graph_width-1;
  }	
}

void blank_region(int lx,int ly,int ux,int uy)
{
 int i,j;
  TRACE
 for (i=lx;i<=ux;i++)
 for (j=lx;j<=ux;j++) pix[i][j] = 0;
}

void plot_pixel(int x, int y, Gun value)
{
  int do_draw, draw_x, draw_y1, draw_y2;
  register int i;
  unsigned long draw_colour;
  TRACE
  if(! plot_saved)
  {
    plot_x = x;
    plot_y1=plot_y2 = y;
    plot_col=table[value];
    do_draw=FALSE;
    plot_saved = (x >=0);
  }else{
    if( x < 0 )
    {
      draw_x=plot_x;
      draw_y1=plot_y1;
      draw_y2=plot_y2;
      draw_colour = plot_col;
      plot_saved=FALSE;
      do_draw=TRUE;
    }else{
      if( (x==plot_x) && (plot_col == table[value]))
      {
        if(y<plot_y1) plot_y1=y;
        if(y>plot_y2) plot_y2=y;
        do_draw=FALSE;
      }else{
        draw_x=plot_x;
        draw_y1=plot_y1;
        draw_y2=plot_y2;
        draw_colour=plot_col;
        do_draw=TRUE;
        plot_x=x;
        plot_y1=plot_y2=y;
        plot_col=table[value];
      }
    }
  }
/*  printf("plot_pixel: STUB!\n");*/
  if( do_draw ) for (i=draw_y1;i<=draw_y2;i++) pix[draw_x][i] = draw_colour;
  /*if( do_draw )
  {
    XSetForeground(dpy,gc,draw_colour);
    if( draw_y1 == draw_y2 )
    {
      XDrawPoint(dpy,pix,gc,draw_x,draw_y1);
    }else{
      XDrawLine(dpy,pix,gc,draw_x,draw_y1,draw_x,draw_y2);
    }
  }*/
}

void flush_region(int x, int y, int w, int h)
{
  TRACE
  plot_pixel(-1,0,0);
  /*XCopyArea(dpy,pix,win,gc,x,y,w,h,x,y);*/
/*  printf("flush_region: STUB!\n");*/
}

Col get_col(float p,float p_minus_x,float p_minus_y,float shadow)
{
  float delta_x, delta_y;
  float delta_x_sqr, delta_y_sqr;
  float hypot_sqr;
  double norm, dshade;
  float effective;
  Col result;
  int band, shade;
  TRACE
  if ( p < g.sealevel )
  {
    if( shadow > g.sealevel )
    {
      return( SEA_UNLIT );
    }else{
      return( SEA_LIT );
    }
  }
  delta_x = p - p_minus_x;
  delta_y = p - p_minus_y;
  delta_x_sqr = delta_x * delta_x;
  delta_y_sqr = delta_y * delta_y;
  hypot_sqr = delta_x_sqr + delta_y_sqr;
  norm = sqrt( 1.0 + hypot_sqr );
  effective = (p + (varience * g.contour *
          (1.0/ ( 1.0 + hypot_sqr))));
  band = ( effective / varience) * N_BANDS;
  if ( band < 0 )
  {
    band = 0;
  }
  if( band > (N_BANDS - 1))
  {
    band = (N_BANDS -1);
  }
  result = (BAND_BASE + (band * g.band_size));
  dshade = vstrength;
  if( p >= shadow )
  {
    dshade += ((double) lstrength *
               ((delta_x * x_fact) + (delta_y * y_fact) + sin_phi));
  }
  dshade /= norm;
  shade = dshade * (double) g.band_size;
  if( shade > (g.band_size-1))
  {
    shade = (g.band_size-1);
  }
  if( shade < 0 )
  {
      shade = 0;
  }
  result += shade;
  /*if( (result >= g.n_col) || (result < 0) )
  {
    printf("INTERNAL ERROR colour out of range %d max %d\n",result,g.n_col);
    exit(1);
  }*/
  return(result);
}

Col *makemap (float* a,float* b,float* shadow)
{
Col *res;
int i;
  TRACE
  res = (Col *) malloc(g.width * sizeof(Col) );
  if (res == NULL)
  {
    printf("malloc failed for colour strip\n");
    exit(1);
  }
  res[0] = BLACK;
  for(i=1 ; i<g.width ; i++)
  {
    res[i] = get_col(b[i],a[i],b[i-1],shadow[i]);
  }
  return(res);
}

int project(int x,float y)
{
  register int pos;
  register double theta;
  TRACE
  theta = atan((double)((viewheight-y)/(x-viewpos)));
  theta -= vangle;
  pos = (gh2)-(focal*tan(theta));
  if(pos>(gh1))
  {
    pos = g.graph_height-1;
  }
  else if( pos < 0 )
  {
    pos = 0;
  }
  return( pos );
}

Col *mirror(float* a,float* b,float* shadow)
{
  Col *res, *map;
  Col last_col;
  int i,j, top, bottom, coord;
  int last_top, last_bottom;
  float pivot;
  TRACE
  res = (Col *) malloc( g.graph_height * sizeof(Col) );
  if( res == NULL )
  {
    printf("malloc failed for picture strip\n");
    exit(1);
  }
  last_col=SKY;
  last_top=g.graph_height-1;
  last_bottom=0;
  map=makemap(a,b,shadow);
  pivot=2.0*g.sealevel;
  for(i=g.width-1;i>0;i--)
  {
    if(map[i] < BAND_BASE)
    {
      for(j=last_bottom;j<=last_top;j++)
      {
        res[j]=last_col;
      }
      last_col=map[i];
      last_bottom=g.graph_height;
      last_top= -1;
      coord=1+project(i,g.sealevel);
      for(j=0;j<coord;j++)
      {
        if( (j+base)%2 || (res[j]<BAND_BASE) )
        {
          res[j]=map[i];
        }
      }
      while(map[i]==last_col)
      {
        i--;
      }
      i++; 
    }else{
      top = project(i,a[i]);
      bottom=project(i,pivot-a[i]);
      if(last_col == map[i])
      {
        if( top > last_top)
        {
          last_top=top;
        }
        if( bottom < last_bottom)
        {
          last_bottom=bottom;
        }
      }else{
        if(top < last_top)
        {
          for(j=top+1;j<=last_top;j++)
          {
            res[j]=last_col;
          }
        }
        if(bottom > last_bottom)
        {
          for(j=last_bottom;j<bottom;j++)
          {
            res[j]=last_col;
          }
        }
        last_top=top;
        last_bottom=bottom;
        last_col=map[i];
      }
    }
  }
  for(j=last_bottom;j<=last_top;j++)
  {
    res[j]=last_col;
  }
  if( a[0] < g.sealevel )
  {
    coord=1+project(0,g.sealevel);
  }else{
    coord=1+project(0,a[0]);
  }
  for(j=0;j<coord;j++)
  {
    res[j] = map[0];
  }
  base=1-base;
  free(map);
  return(res);
}


Col *camera(float* a,float* b,float* shadow)
{
  int i, coord, last;
  Col *res, col;
  TRACE
  res = (Col *) malloc( g.graph_height * sizeof(Col) );
  if( res == NULL )
  {
    fprintf(stderr,"malloc failed for picture strip\n");
    exit(1);
  }
  for( i=0, last=0 ; (i < g.width)&&(last < g.graph_height) ; i++ )
  {
    if( a[i] < g.sealevel )
    {
      a[i] = g.sealevel;
    }
    coord = 1 + project( i, a[i] );
    if( coord > last )
    {
      if( i==0 )
      {
        col = BLACK;
      }else{
        col = get_col(b[i],a[i],b[i-1],shadow[i]);
      }
      if( coord > g.graph_height )
      {
        coord = g.graph_height;
      }
      for(;last<coord;last++)
      {
        res[last]=col;
      }
    }
  }
  for(;last<g.graph_height;last++)
  {
    res[last]=SKY;
  }
  return(res);
}

Col *next_col(int paint, int reflec)
{
  Col *res;
  int i,offset=0;
  TRACE
  if(paint)
  {
    if(reflec)
    {
      res = mirror( a_strip,b_strip,shadow);
    }else{
      res = camera( a_strip,b_strip,shadow);
    }
  }else{
    res = makemap(a_strip,b_strip,shadow);
  }
  if (a_strip) free(a_strip);
  a_strip=b_strip;
  b_strip = extract( next_strip(top) );
  shadow_register += shadow_slip;
  if( shadow_register >= 1.0 )
  {
    while( shadow_register >= 1.0 )
    {
      shadow_register -= 1.0;
      offset++;
    }
    for(i=g.width-1 ; i>=offset ; i--)
    {
      shadow[i] = shadow[i-offset]-delta_shadow;
      if( shadow[i] < b_strip[i] )
      {
        shadow[i] = b_strip[i];
      }
      if( shadow[i] < g.sealevel )
      {
        shadow[i] = g.sealevel;
      }
    }
    for(i=0;i<offset;i++)
    {
      shadow[i] = b_strip[i];
      if( shadow[i] < g.sealevel )
      {
        shadow[i] = g.sealevel;
      }
    }
  }else if( shadow_register <= -1.0 ){
    while( shadow_register <= -1.0 )
    {
      shadow_register += 1.0;
      offset++;
    }
    for(i=0 ; i<g.width-offset ; i++)
    {
      shadow[i] = shadow[i+offset]-delta_shadow;
      if( shadow[i] < b_strip[i] )
      {
        shadow[i] = b_strip[i];
      }
      if( shadow[i] < g.sealevel )
      {
        shadow[i] = g.sealevel;
      }
    }
    for(;i<g.width;i++)
    {
      shadow[i] = b_strip[i];
      if( shadow[i] < g.sealevel )
      {
        shadow[i] = g.sealevel;
      }
    }
  }else{
    for(i=0 ; i<g.width ; i++)
    {
      shadow[i] -= delta_shadow;
      if( shadow[i] < b_strip[i] )
      {
        shadow[i] = b_strip[i];
      }
      if( shadow[i] < g.sealevel )
      {
        shadow[i] = g.sealevel;
      }
    }
  }
  return(res);
}
void scroll_screen(int dist )
{
  TRACE
  printf("scroll_screen: STUB!\n");
}

void plot_column(Graph* g)
{
  Col *l;
  int j;
  int mapwid;
  TRACE
  if( g->repeat >= 0){
    if(g->pos == 0){
      blank_region(0,0,g->graph_width,g->graph_height);
      flush_region(0,0,g->graph_width,g->graph_height);
    }
  }else{
    if( g->pos == g->graph_width-1){
      blank_region(0,0,g->graph_width,g->graph_height);
      flush_region(0,0,g->graph_width,g->graph_height);
    }
  }
  if( g->scroll ){
    scroll_screen(g->scroll);
  }
  l = next_col(1-g->map,g->reflec); 
  if( g->map )
  {
    if( g->graph_height > g->width ){
      mapwid=g->width;
    }else{
      mapwid=g->graph_height;
    }
    for( j=0 ;j<(g->graph_height-mapwid); j++)
    {
      plot_pixel(g->pos,((g->graph_height-1)-j),BLACK);
    }
    for(j=0; j<mapwid ; j++)
    {
      plot_pixel(g->pos,((mapwid-1)-j),l[j]);
    }
  }else{
    for(j=0 ; j<g->graph_height ; j++)
    {
      if( l[j] != SKY )
      {
        plot_pixel(g->pos,((g->graph_height-1)-j),l[j]);
      }
    }
  }
  free(l);
  flush_region(g->pos,0,1,g->graph_height);
  g->scroll = 0;
  if( g->repeat >=0 ){
    g->pos++;
    if(g->pos >= g->graph_width)
    {
      g->pos -=  g->repeat;
      if( g->pos < 0 || g->pos > g->graph_width-1 )
      {
        g->pos=0; 
      }else{
        g->scroll = g->repeat;
      }
    }
  }else{
    g->pos--;
    if( g->pos < 0 ){
      g->pos -=   g->repeat;
      if( g->pos < 0 || g->pos > (g->graph_width-1) ){
	g->pos=g->graph_width-1;
      }else{
	g->scroll = g->repeat;
      }
    }
  }
}

unsigned long packRGB(Gun r, Gun g, Gun b)
{
  TRACE
 return (r<<16)+(g<<8)+b;
}

void init_colmap(Gun* red, Gun* green, Gun* blue)
{
 int i;
  TRACE
 for(i=0;i<g.n_col;i++)  table[i] = packRGB(red[i]>>8, green[i]>>8, blue[i]>>8);
}

/* */



void keyboard(unsigned char key, int x, int y)
{
  TRACE
 switch (key)
   {
    case 27: case 'q': exit(0); break;
    default: break;
   }
}

void anim(void)
{
  TRACE
 glutPostRedisplay();
}

void visible(int vis)
{
  TRACE
 if (vis == GLUT_VISIBLE) glutIdleFunc(anim);
 else                     glutIdleFunc(NULL);
}

void resize(int w, int h)
{
  TRACE
 glViewport(0, 0, (GLsizei)w, (GLsizei)h);
 glPixelZoom((GLdouble)w/(GLdouble)GW, (GLdouble)h/(GLdouble)GH);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1., 1., -1., 1., 1.5, 20.);
 glMatrixMode(GL_MODELVIEW);
}

void time_counter()
{
 char tstr[48];
  TRACE
 if (t1 == (time_t)0)
   {
    time(&t1);
    time(&t2);
    return;
   }
 fps++;
 time(&t2);
 if (t2 > t1)
   {
    sprintf(tstr, "Random Fractal Surfaces: FPS: %d", fps/(int)(t2-t1));
    t1 = t2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }
}

void render(void)
{
  TRACE
 glViewport(0, 0, GW, GH);
 glClear(GL_COLOR_BUFFER_BIT);
 glLoadIdentity();
 gluLookAt(0.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
 glRasterPos3d(3.3, -3.3, 0.);
 glDrawPixels(GW, GH, GL_RGB, GL_UNSIGNED_BYTE, pex);
 time_counter();
 glFlush();
 glutSwapBuffers();
}

void Init()
{
 int i,j;
 Gun *clut[3];
  TRACE
 for (i=0;i<GW;i++)
 for (j=0;j<GH;j++) pix[i][j] = packRGB(255, 0, 0);
 init_parameters(&g, &p);
 for(i=0;i<3;i++) clut[i] = (Gun*)malloc(g.n_col*sizeof(Gun));
 set_clut(g.n_col,clut[0], clut[1], clut[2]);
 init_colmap(clut[0], clut[1], clut[2]);
 for(i=0;i<3;i++) free(clut[i]);
 seed_uni(0);
 init_artist_variables();
 gh2 = g.graph_height/2;
 gh1 = g.graph_height-1;
 for (i=0;i<GW;i++) plot_column(&g);	/* FIXME: ok? */
 pex = (unsigned char*)malloc(GW*GH*3*sizeof(unsigned char));
 for (i=0;i<GH;i++)
 for (j=0;j<GW;j++)
   {
    pex[3*(GW*i+j)]   = pix[j][GH-1-i] >> 16;
    pex[3*(GW*i+j)+1] = (pix[j][GH-1-i] >> 8) & 0xFF;
    pex[3*(GW*i+j)+2] = pix[j][GH-1-i] &  0xFF;
   }
}

int main(int lb, char** par)
{
  TRACE
 Init();
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(GW, GH);
 glutInitWindowPosition(10, 10);
 glutCreateWindow(par[0]);
 glutDisplayFunc(render);
 glutReshapeFunc(resize);
 glutKeyboardFunc(keyboard);
 glutVisibilityFunc(visible);
 glutMainLoop();
 return 0;
}

