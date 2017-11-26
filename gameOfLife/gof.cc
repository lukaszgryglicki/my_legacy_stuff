#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

long msize = 10;
long prob = 10;
long wsec = 1;
long zlevel = 13;
long olevel = 99;
long deathmin = 2;
long deathmax = 4;
long raise = 3;
long endperc = 30;
vector< vector< long > > mat, mat2;

#define JQUAL 80
#include <jpeglib.h>
#include <setjmp.h>
#define ERR_CANNOTREAD 1
#define ERR_BADJPEG    2
#define ERR_GRAYJPEG   3
#define ERR_256CJPEG   4


static int img_counter = 0;

struct my_error_mgr
{
 struct jpeg_error_mgr pub;
 jmp_buf setjmp_buffer;
};
typedef struct my_error_mgr * my_error_ptr;
static my_error_ptr errptr = NULL;

static void my_error_exit (j_common_ptr cinfo)
{
 my_error_ptr myerr = (my_error_ptr) cinfo->err;
 (*cinfo->err->output_message) (cinfo);
 longjmp(myerr->setjmp_buffer, 1);
}

void jpeg_compute_pixels(unsigned char* pix, int x, int y)
{
 int i,j;
 unsigned char col;
 for (i=0;i<y;i++)
 for (j=0;j<x;j++)
 {
        if (mat[i][j]) col = rand() % 0x100;
	else col = 0;

	if (rand() % 2) pix[3*(x*((y-i)-1)+j)] = col;
        else pix[3*(x*((y-i)-1)+j)] = 0;

	if (rand() % 2) pix[3*(x*((y-i)-1)+j)+1] = col;
	else pix[3*(x*((y-i)-1)+j)+1] = 0;

	if (rand() % 2) pix[3*(x*((y-i)-1)+j)+2] = col;
	else pix[3*(x*((y-i)-1)+j)+2] = 0;
 }
}

void init_pixels(unsigned char** pix, int x, int y)
{
    *pix = (unsigned char*)malloc((x*y*3)*sizeof(unsigned char));
    jpeg_compute_pixels(*pix, x, y);
}

void free_pixels(unsigned char** pix)
{
 free(*pix);
 *pix = NULL;
}


int save_jpeg_file(FILE* outfile)
{
 struct jpeg_compress_struct cinfo;
 struct my_error_mgr jerr;
 unsigned char* pixels;
 JSAMPROW row_pointer[1];
 int row_stride, x, y;

 pixels = NULL;
 x = y = msize;
 
 init_pixels(&pixels, x, y);
 cinfo.err = jpeg_std_error(&jerr.pub);
 jerr.pub.error_exit = my_error_exit;
 errptr = &jerr;
 if (setjmp(jerr.setjmp_buffer))
 {
	jpeg_destroy_compress(&cinfo);
	return ERR_BADJPEG;
 }
 jpeg_create_compress(&cinfo);
 jpeg_stdio_dest(&cinfo, outfile);
 cinfo.image_width = x;
 cinfo.image_height = y;
 cinfo.input_components = 3;
 cinfo.in_color_space = JCS_RGB;

 jpeg_set_defaults(&cinfo);
 jpeg_set_quality(&cinfo, JQUAL, FALSE);
 jpeg_start_compress(&cinfo, TRUE);

 row_stride = x*3;
 while (cinfo.next_scanline < cinfo.image_height)
   {
    row_pointer[0] = &pixels[cinfo.next_scanline * row_stride];
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
   }
 jpeg_finish_compress(&cinfo);
 jpeg_destroy_compress(&cinfo);
 free_pixels(&pixels);
 return 0;
}

void write_output_jpg()
{
 FILE* plik;
 
 char fname[100];
 sprintf(fname, "%06d.jpeg", img_counter ++);
 
 plik = fopen(fname,"wb");
 if (!plik) 
 { 
	 printf("Error writing JPG: %s\n", fname); 
	 return ; 
 }

 save_jpeg_file(plik);

 fclose(plik);
}

bool life_step(vector < vector < long > >& m1, vector < vector < long > >& m2)
{
    long n, im, ip, jm, jp, l;

    l = 0;
    for (int i=0;i<msize;i++)
    {
	for (int j=0;j<msize;j++)
	{
	    im = rand() % 100;
	    if (im <= zlevel) m1[i][j] = 0;
	    else if (im >= olevel) m1[i][j] = 1;

	    if (m1[i][j]) 
	    {
		l ++;
//		cout << "**";
	    }
	    else 
	    {
//		cout << "  ";
	    }
	}
//	cout << endl;
    }

    write_output_jpg();

 //   cout << "Currently " << l << " cells alive" << endl;

    if (l == 0)
    {
	cout << "Your system died" << endl;
	return false;
    }
    else if (l * 100 > msize * msize * endperc)
    {
	cout << "System used all resources: " << l << " " << msize * msize<< endl;
	return false;
    }
    else cout << img_counter << ": " << (l * 100) / (msize * msize) << endl;

    for (int i=0;i<msize;i++)
	for (int j=0;j<msize;j++)
	{
	    n = 0;
	    l = m1[i][j];

	    im = i - 1;
	    ip = i + 1;
	    jm = j - 1;
	    jp = j + 1;

	    if (im < 0) im = msize - 1;
	    if (jm < 0) jm = msize - 1;
	    if (ip >= msize) ip = 0;
	    if (jp >= msize) jp = 0;

	    if (m1[im][jm] != 0) n ++;
	    if (m1[im][j]  != 0) n ++;
	    if (m1[im][jp] != 0) n ++;

	    if (m1[i][jm]  != 0) n ++;
	    if (m1[i][jp]  != 0) n ++;

	    if (m1[ip][jm] != 0) n ++;
	    if (m1[ip][j]  != 0) n ++;
	    if (m1[ip][jp] != 0) n ++;

	    if (l != 0 && (n < deathmin || n > deathmax)) l = 0;
	    if (l == 0 && n == raise) l = 1;

	    m2[i][j] = l;
	}
    return true;

}

void life()
{
    time_t tm;
    vector< long > vec;

    time( &tm );
    srand( (long)tm );

    cout << "Life started" << endl;

    for (int i=0;i<msize;i++)
    {
	vec.push_back( 0 );
    }

    for (int i=0;i<msize;i++)
    {
	mat.push_back( vec );
	mat2.push_back( vec );
    }

    long l = 0;


    for (int i=0;i<msize;i++)
	for (int j=0;j<msize;j++)
	{
	    if ( (rand() % 100) < prob) 
	    {
		mat[i][j] = 1;
		l ++;
	    }
	    else mat[i][j] = 0;

	}

   cout << "Beeings created: " << l << endl;

   bool alive;

   do
   {
       alive = life_step(mat, mat2);
       mat = mat2;

       fflush( stdout );
       sleep( wsec );
   }
   while (alive);
}

int main(int lb, char** par)
{
    cout << par[0] << ", params: n prob%% wait_secs zlevel:13 olevel:99 deathmin:2 deathmax:4 raise:3 endperc:30" << endl;

    if (lb < 9) return 1;

    msize = atoi(par[1]);
    prob  = atoi(par[2]);
    wsec  = atoi(par[3]);
    zlevel  = atoi(par[4]);
    olevel  = atoi(par[5]);
    deathmin  = atoi(par[6]);
    deathmax  = atoi(par[7]);
    raise  = atoi(par[8]);
    endperc = atoi(par[9]);

    life();
    return 0;
}

