#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
/*#include <varargs.h>*/
/*#define SPEED*/
#define UINT8  unsigned char
#define SINT8  signed char
#define UINT16 unsigned short
#define SINT16 signed short
#define UINT32 unsigned int
#define SINT32 signed int
#define UINT64 unsigned long long
#define SINT64 signed long long
#define BYTE   unsigned char
#define SBYTE  char


/* TREE with left, right, up, sum and val */
typedef struct _tree
{
  struct _tree *l, *r, *u;
  UINT32 val;
  UINT32 sum;
} tree;

/* TREE with left, right, sum and val */
typedef struct _nutree
{
  struct _nutree *l, *r;
  UINT32 sum;
  UINT32 val;
} nutree;

/* TREE with left, right, and val */
typedef struct _shtree
{
  struct _shtree *l, *r;
  UINT32 val;
} shtree;

/*
typedef struct _list
{
 struct _list *n;
 UINT32 val;
} list;
*/

typedef struct _btable
{
 UINT8 nbits;
 UINT8 nbytes;
 UINT16* bits;				/* on the EDGE of optimisation, all 16bit are used/moved/truncated/masked etc */
} btable;

typedef struct _btree
{
  struct _btree *l, *r;
  UINT32 idx;
  btable b;
} btree;

UINT8 g_err_code = 0;
BYTE* prev_buffer = NULL;
BYTE* buffer = NULL;
BYTE* outbuffer_hdr = NULL;
BYTE* outbuffer_data = NULL;
char* gcout = NULL;
UINT32 block_size;
UINT32 header_length = 0;
UINT32 fsize, fbits;
UINT32 current_bit = 0;
UINT32 bits = 8;
UINT32 g_htree = 0;
UINT32 g_max_depth = 0;
UINT32 h_len = 0;
UINT32 h_idx = 0;
UINT32 data_size = 0;
UINT32 data_size_bits = 0;
UINT32 outbuffer_hdr_len = 0;
UINT32 outbuffer_data_len = 0;
UINT32 btab_len = 0;
UINT32 bbits_len = 0;
UINT32 debug_mode = 1;
UINT32 *h_tab = NULL;
UINT8  *chunks_8bit = NULL;
UINT16 *chunks_16bit = NULL;
UINT32 *chunks_32bit = NULL;
UINT32 n_chunks, chunk_nbytes;
UINT32 stdin_mode, stdout_mode;
UINT32 want_gtree;
tree *htree;
nutree *stree, *otree;
shtree *s_htree;
btree* idxtree;
/*list *dlist_head, *dlist_tail;*/
/*btable *btab;*/
double cratioh, cratiod, gcratio;
void travel_htree_postorder(tree* t, UINT32);
void travel_shtree_postorder(shtree* t, UINT32);
void check_htree(tree*, UINT32);

#ifndef SPEED
UINT32 debug(UINT32 debug_level, char* fmt, ...)
{
 va_list ap;
 UINT32 err;
 if (debug_level > debug_mode) return 1;
 va_start(ap, fmt);
 err = vprintf(fmt,ap);
 va_end(ap);
 return err;
}
#endif

void travel_htree_gtree_full( tree* t, SBYTE** gtree, UINT32 depth, UINT32 pos)
{
 UINT32 save_pos, temp_pos, i, j;
 SBYTE temp[9];
 save_pos = pos;
 temp_pos = pos;
 i = 3 * depth;
 j = (temp_pos * 10) + 1;
 sprintf(temp, "%08x", t->val);
 memcpy(&gtree[i][j], temp, 8);
 i ++;
 sprintf(temp, "%08d", t->sum);
 memcpy(&gtree[i][j], temp, 8);
 if (t->l && t->r)        sprintf(temp, " /    \\ ");
 else if (t->l && !t->r)  sprintf(temp, " /   0  ");
 else if (!t->l && t->r)  sprintf(temp, "  0   \\ ");
 else if (!t->l && !t->r) sprintf(temp, "  0  0  ");
 i ++;
 memcpy(&gtree[i][j], temp, 8);
 if (t->l)
 {
 	temp_pos = save_pos - (1 << (g_max_depth - depth - 2));
	travel_htree_gtree_full( t->l, gtree, depth + 1, temp_pos );
 }
 if (t->r)
 {
 	temp_pos = save_pos + (1 << (g_max_depth - depth - 2));
	travel_htree_gtree_full( t->r, gtree, depth + 1, temp_pos );
 }
}

void travel_htree_gtree_chars( tree* t, SBYTE** gtree, UINT32 depth, UINT32 pos)
{
 UINT32 save_pos, temp_pos, i, j, leaf;
 UINT16 z;
 SBYTE temp[10], zc[10];
 save_pos = pos;
 temp_pos = pos;
/* printf("v = %d, s = %d\n", t->val, t->sum);*/
 if (t->val > 0x10000) leaf = 0;
 else leaf = 1;
 i = 3 * depth;
 j = temp_pos * 4;
 if (t->sum > 9999) sprintf(temp, "!999");
 else sprintf(temp, "%4d", t->sum);
 memcpy(&gtree[i][j], temp, 4);
 i ++;
 if (leaf)
 {
	 z = t->val & 0xFF;
/*	 printf("z1 = %d\n", z);*/
	 if (z >= 0x20 && z <= 0x80) sprintf(temp, "'%2c'", (SBYTE)z);
	 else sprintf(temp, " %02x ", z);
 }
 else
 {
     sprintf(temp, "    ");
 }
 memcpy(&gtree[i][j], temp, 4);
 i ++;
 if (leaf)
 {
	 z = (t->val & 0xFF00) >> 0x8;
/*	 printf("z2 = %d\n", z);*/
	 if (z >= 0x20 && z <= 0x80) sprintf(zc, "%2c", (SBYTE)z);	
	 else sprintf(zc, "%02x", z);
 }
 else
 {
     sprintf(zc, "  ");

 }
 if (t->l && t->r)        sprintf(temp, "/%s\\", zc);
 else if (t->l && !t->r)  sprintf(temp, "/%s ", zc);
 else if (!t->l && t->r)  sprintf(temp, " %s\\", zc);
 else if (!t->l && !t->r) sprintf(temp, " %s ", zc);

 memcpy(&gtree[i][j], temp, 4);
 if (t->l)
 {
 	temp_pos = save_pos - (1 << (g_max_depth - depth - 2));
	travel_htree_gtree_chars( t->l, gtree, depth + 1, temp_pos );
 }
 if (t->r)
 {
 	temp_pos = save_pos + (1 << (g_max_depth - depth - 2));
	travel_htree_gtree_chars( t->r, gtree, depth + 1, temp_pos );
 }
}

void travel_htree_gtree_asterisk( tree* t, SBYTE** gtree, UINT32 depth, UINT32 pos)
{
 UINT32 save_pos, temp_pos, i, j;
/* printf("travel; pos=%d, depth=%d\n", pos, depth);*/
/* fflush(stdout);*/
 save_pos = pos;
 temp_pos = pos;
 i = depth;
 j = temp_pos;
 gtree[i][j] = '*';
 if (t->l)
 {
 	temp_pos = save_pos - (1 << (g_max_depth - depth - 2));
	travel_htree_gtree_asterisk( t->l, gtree, depth + 1, temp_pos );
 }
 if (t->r)
 {
 	temp_pos = save_pos + (1 << (g_max_depth - depth - 2));
	travel_htree_gtree_asterisk( t->r, gtree, depth + 1, temp_pos );
 }
}

void gtree_out_htree_full( tree* t)
{
 UINT32 len, h, i, j, pos;
 SBYTE** gtree;
 SBYTE* sep;
 len = ((1 << g_max_depth ) - 1) * 10;
 h = 3 * ( g_max_depth );
 gtree = (SBYTE**)malloc(h * sizeof(SBYTE*));
 sep = (SBYTE*)malloc((len + 1) * sizeof(SBYTE));
 for (j=0;j<len;j++) sep[j] = '#';
 sep[len] = 0;
 for (i=0;i<h;i++) 
 {
     gtree[i] = (SBYTE*)malloc((len + 1) * sizeof(SBYTE));
     for (j=0;j<len;j++) gtree[i][j] = 0x20;
     gtree[i][len] = 0;
 }
 pos = (1 << (g_max_depth - 1)) -1;
 travel_htree_gtree_full( t, gtree, 0, pos);

 printf("%s##\n", sep);
 for (i=0;i<h;i++) printf("#%s#\n", gtree[i]);
 printf("%s##\n", sep);
}

void gtree_out_htree_chars( tree* t)
{
 UINT32 len, h, i, j, pos;
 SBYTE** gtree;
 SBYTE* sep;
 if (bits > 0x10) return;
 len = ((1 << g_max_depth ) - 1) * 4;
 h = 3 * ( g_max_depth );
 gtree = (SBYTE**)malloc(h * sizeof(SBYTE*));
 sep = (SBYTE*)malloc((len + 1) * sizeof(SBYTE));
 for (j=0;j<len;j++) sep[j] = '#';
 sep[len] = 0;
 for (i=0;i<h;i++) 
 {
     gtree[i] = (SBYTE*)malloc((len + 1) * sizeof(SBYTE));
     for (j=0;j<len;j++) gtree[i][j] = 0x20;
     gtree[i][len] = 0;
 }
 pos = (1 << (g_max_depth - 1)) - 1;
 travel_htree_gtree_chars( t, gtree, 0, pos);

 printf("%s##\n", sep);
 for (i=0;i<h;i++) printf("#%s#\n", gtree[i]);
 printf("%s##\n", sep);
}

void gtree_out_htree_asterisk( tree* t)
{
 UINT32 len, h, i, j, pos;
 SBYTE** gtree;
 SBYTE* sep;
 len = (1 << g_max_depth) - 1;
 h = g_max_depth;
 gtree = (SBYTE**)malloc(h * sizeof(SBYTE*));
 sep = (SBYTE*)malloc((len + 1) * sizeof(SBYTE));
 for (j=0;j<len;j++) sep[j] = '#';
 sep[len] = 0;
 for (i=0;i<h;i++) 
 {
     gtree[i] = (SBYTE*)malloc((len + 1) * sizeof(SBYTE));
     for (j=0;j<len;j++) gtree[i][j] = 0x20;
     gtree[i][len] = 0;
 }
/* printf("max_depth = %d\n", g_max_depth);*/
 pos = (1 << (g_max_depth - 1)) - 1;
 travel_htree_gtree_asterisk( t, gtree, 0, pos);

 printf("%s##\n", sep);
 for (i=0;i<h;i++) printf("#%s#\n", gtree[i]);
 printf("%s##\n", sep);
}

void gtree_out_shtree( shtree* t )
{

}

void readbuffer(FILE* f)
{
 SINT32 i, zn, gbs;
 fseek(f, 0, SEEK_END);
 gbs = 0;
 fsize = ftell(f);
#ifndef SPEED
 debug(0, "Size is: %8ld bytes\n", (long)fsize);
#endif
 fseek(f, 0, SEEK_SET);
 if (fsize == 0 && stdin_mode) 
 {
     fsize = 1 << 10;
     gbs = 1; 
 }
 buffer = (BYTE*)malloc(fsize + 8);
 if (!buffer) g_err_code = 4;
 i = 0;
 while ((zn = fgetc(f)) != EOF) buffer[i++] = (BYTE)zn;
 if (gbs) fsize = i;
 fbits = 8*fsize;
#ifndef SPEED
 debug(0, "Read %8ld/%8ld bits, %8ld bytes\n", i*8, fbits, fsize);
#endif
 for (i=0;i<6;i++) buffer[fsize+i] = 0;
}


char* binout(UINT32 val)
{
#ifndef SPEED
 UINT32 mask;
 SINT32 i;
#endif
 if (!gcout) gcout = malloc(34);
#ifndef SPEED
 gcout[32] = 0;
 for (i=31;i>=0;i--)
 {
     mask = 1 << i;
     if (val & mask) gcout[31-i] = '1';
     else gcout[31-i] = '0';
 }
#else
 gcout[0] = 1;
#endif
 return gcout;
}


UINT32 get_next_chunk(UINT32* chunk)
{
 UINT32 start_bit, start_byte;
 UINT32 start_bit_byte, tmp;
 register UINT32 bit_offset, bit_mask;

 start_bit = current_bit;
 tmp = 0;
#ifndef SPEED
 debug(4, "gnc: start_bit(DEC) = %08lx\n", start_bit);
 if (start_bit >= fbits) { *chunk = 0; return 0; }
 start_byte = start_bit >> 3;
 debug(4, "start_byte(DEC) = %08lx\n", start_byte);
 start_bit_byte = start_byte << 3;
 debug(4, "start_bit_byte(DEC) = %08lx\n", start_bit_byte);
 bit_offset = start_bit - start_bit_byte;
 debug(4, "bit_offset(DEC) = %08lx\n", bit_offset);
 bit_mask = (1 << bits) - 1;
 debug(4, "bit_mask = %s\n", binout(bit_mask));
 bit_mask <<= bit_offset;
 debug(4, "bit_mask = %s\n", binout(bit_mask));
 memcpy((void*)(&tmp), (void*)(buffer + start_byte), 4);
 debug(4, "tmp1 =     %s\n", binout(tmp));
 tmp &= bit_mask;
 debug(4, "tmp2 =     %s\n", binout(tmp));
 tmp >>= bit_offset;
 debug(3, "gnc:chunk= %s\n", binout(tmp));
 *chunk = tmp;
 current_bit += bits;
 return 1;
#else
 if (start_bit >= fbits) { *chunk = 0; return 0; }
 start_byte = start_bit >> 3;
 start_bit_byte = start_byte << 3;
 bit_offset = start_bit - start_bit_byte;
 bit_mask = ((1 << bits) - 1) << bit_offset;
 memcpy((void*)(&tmp), (void*)(buffer + start_byte), 4);
 *chunk = (tmp & bit_mask) >> bit_offset;
 current_bit += bits;
 return 1;
#endif
}


void correct_htree_up(tree* p)
{
 tree* tmp;
#ifndef SPEED
 debug(5, "correct htree up\n");
#endif
 if (!p) return;
 if (p->l && p->r)
 {			/* both nodes */
/*     printf("both nodes\n");*/
     if (p->l->sum > p->r->sum)
     {			/* swap nodes */
#ifndef SPEED
         debug(5, "nodes swap\n");
#endif
	 tmp  = p->l;
	 p->l = p->r;
	 p->r = tmp;
	 		/* update sum */
     }
     p->sum = p->l->sum + p->r->sum;
     correct_htree_up(p->u);
 }
 else if (!p->l && p->r)
 {			/* only right node */
/*     printf("right node\n");*/
     p->sum = p->r->sum;
     correct_htree_up(p);
 }
 else if (p->l && !p->r)
 {			/* only left node */
/*     printf("left node\n");*/
     p->sum = p->l->sum;
     correct_htree_up(p->u);
 }
 else if (!p->l && !p->r) return;	/* shouldn't happen */
}


void insert_htree_node(tree** t, tree* parent, UINT32 sum, UINT32 chunk)
{
 if (!(*t))
 {
#ifndef SPEED
     debug(5, "insert htree node: s=%8ld, %08lx\n", sum, chunk);
#endif
     /* rolled to the leaf */
     *t = (tree*)malloc(sizeof(tree));
     (*t)->u = parent;
     (*t)->l = NULL;
     (*t)->r = NULL;
     (*t)->val = chunk;
     (*t)->sum = sum;
     /* make parent node a sum node, and roll its down to create new node */
     if ((*t)->u)	/* if we have parent */
     {
		if ((*t)->u->sum < (*t)->sum)		/* move parent to left */
		{
#ifndef SPEED
		     debug(5, "move parent to left\n");
#endif
			/* new node left from parent */
			(*t)->u->r = *t;				/* connect us */
			(*t)->u->l = (tree*)malloc(sizeof(tree));	/* make new note from parent left */
			(*t)->u->l->u = (*t)->u;			/* connect it to parent */
			(*t)->u->l->l = NULL;			/* gave it no children */
			(*t)->u->l->r = NULL;
			(*t)->u->l->sum = (*t)->u->sum;		/* copy prent val to it */
			(*t)->u->l->val = (*t)->u->val;
			/* update parent */
			(*t)->u->sum += sum;			/* update parent sum */
			(*t)->u->val = 0x7FFFFFFF;			/* special for internal node */
			correct_htree_up((*t)->u);
		}
		else					/* move parent to right */
		{
#ifndef SPEED
		     debug(5, "move parent to right\n");
#endif
			/* new node right from parent */
			(*t)->u->l = *t;				/* connect us */
			(*t)->u->r = (tree*)malloc(sizeof(tree));	/* make new note from parent right */
			(*t)->u->r->u = (*t)->u;			/* connect it to parent */
			(*t)->u->r->l = NULL;			/* gave it no children */
			(*t)->u->r->r = NULL;
			(*t)->u->r->sum = (*t)->u->sum;		/* copy prent val to it */
			(*t)->u->r->val = (*t)->u->val;
			/* update parent */
			(*t)->u->sum += sum;			/* update parent sum */
			(*t)->u->val = 0x7FFFFFFF;			/* special for internal node */
			correct_htree_up((*t)->u);
		}
     }
/* 	 printf("htree\n");*/
/*	 travel_htree_postorder(htree);*/
/*	 printf("check htree\n");*/
/*	 check_htree(htree, 1);*/
     return;
 }
 if (sum <= (*t)->sum) insert_htree_node(&((*t)->l), *t, sum, chunk);
 else insert_htree_node(&((*t)->r), *t, sum, chunk);
}


void insert_stree_sum(nutree** t, UINT32 sum, UINT32 chunk)
{
 UINT32 where;
#ifndef SPEED
 debug(5, "iss: %8ld - %08lx\n", sum, chunk);
#endif
 where = rand() % 2;
 if (!(*t))
 {
     *t = (nutree*)malloc(sizeof(nutree));
     /*(*t)->u = parent;*/
     (*t)->l = NULL;
     (*t)->r = NULL;
     (*t)->val = chunk;
     (*t)->sum = sum;
     return;
 }
 if (where)		/* randomize tree */
 {
	if (sum <= (*t)->sum) insert_stree_sum(&((*t)->l), sum, chunk);
	if (sum > (*t)->sum) insert_stree_sum(&((*t)->r), sum, chunk);
 }
 else
 {
	if (sum < (*t)->sum) insert_stree_sum(&((*t)->l), sum, chunk);
	if (sum >= (*t)->sum) insert_stree_sum(&((*t)->r), sum, chunk);
 }
}


void insert_otree_chunk(nutree** t, UINT32 chunk)
{
 if (!(*t))
 {
     *t = (nutree*)malloc(sizeof(nutree));
     /*(*t)->u = parent;*/
     (*t)->l = NULL;
     (*t)->r = NULL;
     (*t)->val = chunk;
     (*t)->sum = 1;
     return;
 }
 if ((*t)->val == chunk)
 {
     (*t)->sum ++;
 }
 /*if (chunk < (*t)->val) insert_otree_chunk(&((*t)->l), *t, chunk);*/
 /*if (chunk > (*t)->val) insert_otree_chunk(&((*t)->r), *t, chunk);*/
 if (chunk < (*t)->val) insert_otree_chunk(&((*t)->l), chunk);
 if (chunk > (*t)->val) insert_otree_chunk(&((*t)->r), chunk);
}


void travel_otree_postorder(nutree* t)
{
#ifndef SPEED
 debug(4, "t=%8p: l=%8p r=%8p v=%08x s=%08d\n",
	 (void*)t, (void*)t->l, (void*)t->r, t->val, t->sum);
#endif
 insert_stree_sum(&stree, t->sum, t->val);
 if (t->l)
 {
#ifndef SPEED
     debug(5, "going left\n");
#endif
     travel_otree_postorder(t->l);
 }
 if (t->r)
 {
#ifndef SPEED
     debug(5, "going right\n");
#endif
     travel_otree_postorder(t->r);
 }
}


void travel_stree_inorder(nutree* t)
{
 /* reverse order, we need sums from hihgest to lowest */
 if (t->r)
 {
#ifndef SPEED
     debug(5, "going right\n");
#endif
     travel_stree_inorder(t->r);
 }
#ifndef SPEED
 debug(4, "t=%8p: l=%8p r=%8p v=%08x s=%08d\n",
	 (void*)t, (void*)t->l, (void*)t->r, t->val, t->sum);
#endif
 insert_htree_node(&htree, NULL, t->sum, t->val);
 if (t->l)
 {
#ifndef SPEED
     debug(5, "going left\n");
#endif
     travel_stree_inorder(t->l);
 }
}


void add_buffer(tree* t)
{
 UINT32 start_bit, start_byte;
 UINT32 start_bit_byte, tmp;
 register UINT32 bit_offset, bit_mask;
 /*UINT32 val;*/
 start_bit = current_bit;
 start_byte = start_bit >> 3;
 start_bit_byte = start_byte << 3;
 bit_offset = start_bit - start_bit_byte;
#ifndef SPEED
 debug(4, "addb: current_bit = %08lx\n", current_bit);
 debug(4, "start_bit = %08lx\n", start_bit);
 debug(4, "start_byte = %08lx\n", start_byte);
 debug(4, "start_byte_bit = %08lx\n", start_bit_byte);
 debug(4, "bit_offset = %08lx\n", bit_offset);
#endif
 tmp = 0;
 if (t->l && t->r)
 {
#ifdef SPEED
 	bit_mask = 1 << bit_offset;
 	memcpy((void*)(&tmp), (void*)(outbuffer_hdr + start_byte), 4);
	tmp |= bit_mask;
 	memcpy((void*)(outbuffer_hdr + start_byte), (void*)(&tmp), 4);
 	current_bit ++;
#else
        debug(4, "internal node\n");
/*    val = 0;*/
/*	debug(4, "ival     = %s, %08lx\n", binout(val), val); */
 	bit_mask = 1 << bit_offset;
	debug(4, "bit_mask = %s\n", binout(bit_mask));
 	memcpy((void*)(&tmp), (void*)(outbuffer_hdr + start_byte), 4);
	debug(4, "tmp1     = %s, %08lx\n", binout(tmp), tmp);
	tmp |= bit_mask;
	debug(4, "tmp2     = %s, %08lx\n", binout(tmp), tmp);
 	memcpy((void*)(outbuffer_hdr + start_byte), (void*)(&tmp), 4);
 	current_bit ++;
#endif
 }
 else if (!t->l && !t->r)
 {
#ifdef SPEED
	bit_mask = t->val << (1 + bit_offset);
 	memcpy((void*)(&tmp), (void*)(outbuffer_hdr + start_byte), 4);
	tmp |= bit_mask;
 	memcpy((void*)(outbuffer_hdr + start_byte), (void*)(&tmp), 4);
 	current_bit += bits + 1;
#else
    debug(4, "leaf node %08lx %s\n", t->val, binout(t->val));
    /*val = t->val;*/
 	/*bit_mask = 1 << bits;*/
	bit_mask = t->val;
	debug(4, "bit_mask1 = %s\n", binout(bit_mask));
	/*bit_mask += val;*/
	bit_mask <<= 1;
	debug(4, "bit_mask2 = %s\n", binout(bit_mask));
	bit_mask <<= bit_offset;
	debug(4, "bit_mask3 = %s\n", binout(bit_mask));
 	memcpy((void*)(&tmp), (void*)(outbuffer_hdr + start_byte), 4);
	debug(4, "tmp1      = %s, %08lx\n", binout(tmp), tmp);
	tmp |= bit_mask;
	debug(3, "addb:tmp2 = %s, %08lx\n", binout(tmp), tmp);
 	memcpy((void*)(outbuffer_hdr + start_byte), (void*)(&tmp), 4);
 	current_bit += bits + 1;
#endif
 }
 else
 {
#ifndef SPEED
     debug(0, "add_buffer: error\n");
#endif
 }
}


void add_buffer_bits(UINT32 chunk, UINT32 n_bits)
{
 UINT32 start_bit, start_byte, tmp;
 UINT32 start_bit_byte;
 register UINT32 bit_offset, bit_mask;

 start_bit = current_bit;
 start_byte = start_bit >> 3;
 start_bit_byte = start_byte << 3;
 bit_offset = start_bit - start_bit_byte;
 tmp = 0;
#ifndef SPEED
 debug(3, "addb_bits: chunk=%08lx, n_bits=%08lx\n", chunk, n_bits);
 debug(4, "addb_bits: current_bit = %08lx\n", current_bit);
 debug(4, "start_bit = %08lx\n", start_bit);
 debug(4, "start_byte = %08lx\n", start_byte);
 debug(4, "start_byte_bit = %08lx\n", start_bit_byte);
 debug(4, "bit_offset = %08lx\n", bit_offset);
#endif
#ifndef SPEED
	debug(4, "val       = %s, %08lx\n", binout(chunk), chunk);
	bit_mask = chunk;
	debug(4, "bit_mask2 = %s\n", binout(bit_mask));
	bit_mask <<= bit_offset;
	debug(4, "bit_mask3 = %s\n", binout(bit_mask));
 	memcpy((void*)(&tmp), (void*)(outbuffer_data + start_byte), 4);
	debug(4, "tmp1      = %s, %08lx\n", binout(tmp), tmp);
	tmp |= bit_mask;
	debug(4, "tmp2      = %s, %08lx\n", binout(tmp), tmp);
 	memcpy((void*)(outbuffer_data + start_byte), (void*)(&tmp), 4);
 	current_bit += n_bits;
#else
	/* value should be cleared before memcpy */
	bit_mask = chunk << bit_offset;
 	memcpy((void*)(&tmp), (void*)(outbuffer_data + start_byte), 4);
	tmp |= bit_mask;
 	memcpy((void*)(outbuffer_data + start_byte), (void*)(&tmp), 4);
 	current_bit += n_bits;
#endif
}


void add_buffer_bytes(btable* bt)
{
	UINT32 i;
	UINT32 n_b;
#ifndef SPEED
	debug(3, "Add_bytes: %08lx (%08lx bits)\n", bt->nbytes, bt->nbits);
#endif
/*	for (i=bt->nbytes-1;i>=0;i--)*/
	for (i=0;i<bt->nbytes;i++)
	{
		if (i == (bt->nbytes - 1))	/* last byte */
		{
		    n_b = (bt->nbits % 0x10);
			if (!n_b) n_b = 0x10;
#ifndef SPEED
			debug(3, "Add last bits: %s, %08lx\n", binout(bt->bits[i]), n_b);
#endif
			add_buffer_bits(bt->bits[i], n_b);
		}
		else if (bt->nbytes > 1)		/* add entire 16 bits */
		{
#ifndef SPEED
		    debug(3, "Adding bytes: bytes=%08lx, bits=%08lx\n", bt->nbytes, bt->nbits);
			debug(3, "Add internal bits: %s, %08lx\n", binout(bt->bits[i]), 16);
			n_b = (bt->nbits % 0x10);
			if (!n_b) n_b = 0x10;
			debug(3, "Add internal prev: %s, %08lx\n", binout(bt->bits[i+1]), n_b);
#endif
			add_buffer_bits(bt->bits[i], 0x10);
		}
	}
}

/*

void travel_htree_btable(tree* t, long current_bit, long* tab)
{
 long current_byte, bytes_needed, bit_mask, bit_offset;
 long *tab_store, current_bit_store;
 int i;
 current_byte = current_bit >> 4;
 if (current_bit && !(current_bit % 0x10))
 {
     current_byte --;
 }
 bytes_needed = current_byte + 1;
 if (!t->r && !t->l)
 {
        if (bytes_needed > 1) printf("leaf: bytes_needed: %08lx, cur_bit = %08lx\n", bytes_needed, current_bit);
	btab[ t->val ].nbytes = bytes_needed;
	btab[ t->val ].nbits = current_bit;
	btab[ t->val ].bits  = (long*)malloc((bytes_needed + 1)*sizeof(long));
	for (i=0;i<bytes_needed;i++)
	{
		btab[ t->val ].bits[i] = tab[i];
		btab[ t->val ].bits[i] >>= 1;
	}
 }
 tab_store = (long*)malloc((bytes_needed+1) * sizeof(long));
 for (i=0;i<=bytes_needed;i++) tab_store[i] = tab[i];
 current_bit_store = current_bit;
 if (t->l)
 {
	 current_bit ++;
         travel_htree_btable(t->l, current_bit, tab);
 }
 for (i=0;i<=bytes_needed;i++) tab[i] = tab_store[i];
 current_bit = current_bit_store;
 if (t->r)
 {
	 current_bit ++;
	 if ((current_bit >> 4) >= 1 && (current_bit % 0x10) == 1)
	 {
	         printf("node: next byteR for curr_bit = %08lx\n", current_bit);
		 tab[current_byte + 1] |= 1;
	 }
	 else
	 {
	     	 bit_mask = 0;
		 bit_offset = current_bit % 0x10;
		 if (current_bit && !bit_offset) bit_offset = 0x10;
		 bit_mask = 1 << bit_offset;
		 tab[current_byte] |= bit_mask;
	 }
     travel_htree_btable(t->r, current_bit, tab);
 }
 if (tab_store) free((void*)tab_store);
 tab_store = NULL;
}

*/

void insert_btree(btree** t, btable* b, UINT32 idx)
{
 if (!(*t))
 {
     *t = (btree*)malloc(sizeof(btree));
     (*t)->l = NULL;
     (*t)->r = NULL;
	 (*t)->b.bits = b->bits;
	 (*t)->b.nbits = b->nbits;
	 (*t)->b.nbytes = b->nbytes;
	 (*t)->idx = idx;
#ifndef SPEED
     debug(4, "ibtree: b=(bytes=%08lx, bits=%08lx), idx=%08lx\n", b->nbytes, b->nbits, idx);
#endif
     return;
 }
 if (idx <= (*t)->idx) insert_btree(&((*t)->l), b, idx);
 if (idx > (*t)->idx) insert_btree(&((*t)->r), b, idx);
}


void travel_htree_btable(tree* t, UINT32 current_bit, UINT32* tab)
{
 UINT32 current_byte, bytes_needed;
 UINT32 *tab_store, current_bit_store;
 register UINT32 bit_offset, bit_mask, trunc_bit, i;
 btable btemp;
 current_byte = current_bit >> 4;
 if (current_bit && !(current_bit % 0x10))
 {
     current_byte --;
#ifndef SPEED
     debug(5, "16th bit, current_byte = %08lx\n", current_byte);
#endif
 }
 bytes_needed = current_byte + 1;
#ifndef SPEED
 debug(4, "thb: current_bit = %08lx (byte=%08lx, bytes_need=%08lx)\n", current_bit, current_byte, bytes_needed);
#endif
 if (!t->r && !t->l)	/* leaf */
 {
#ifndef SPEED
	debug(4, "write leaf idx: %08lx\n", t->val);
#endif
	/*current_bit ++;*/
/*        if (bytes_needed > 1) printf("leaf: bytes_needed: %08lx, cur_bit = %08lx\n", bytes_needed, current_bit);*/
	btemp.nbytes = (BYTE)bytes_needed;
	btemp.nbits = (BYTE)current_bit;
	/*btemp.bits  = (UINT32*)malloc((bytes_needed + 1)*sizeof(UINT32));*/
	btemp.bits  = (UINT16*)malloc((bytes_needed + 1)*sizeof(UINT16));
	for (i=0;i<bytes_needed;i++)
	{
		btemp.bits[i] = tab[i];
/*		btab[ t->val ].bits[i] = tab[i];*/
/*		btab[ t->val ].bits[i] >>= 1;*/
	}
	if (current_bit == 0)
	{
		btemp.nbits = 1;
		btemp.bits[0] = 0;
	}
	/*btab[ t->val ].nbytes = bytes_needed; */
	/*btab[ t->val ].nbits = current_bit;*/
	/*btab[ t->val ].bits  = (long*)malloc((bytes_needed + 1)*sizeof(long));*/
	/* FIXME: will it store just value-copied pointer created from local stack */
	/* seems so :-) --> works Okay */
	insert_btree( &idxtree, &btemp, t->val);
 }
 /* tab store */
 tab_store = (UINT32*)malloc((bytes_needed+1) * sizeof(UINT32));
 for (i=0;i<=bytes_needed;i++) tab_store[i] = tab[i];
 current_bit_store = current_bit;
 if (t->l)
 {
	 current_bit ++;
     travel_htree_btable(t->l, current_bit, tab);
 }
 /* tab restore */
 for (i=0;i<=bytes_needed;i++) tab[i] = tab_store[i];
 current_bit = current_bit_store;
 if (t->r)
 {
	 if ((current_bit >> 4) >= 1 && (current_bit % 0x10) == 0) /* 17th, 33th, 49th ... bit, time to change long */
	 {
#ifdef SPEED
	     bit_mask = 0;
		 trunc_bit = current_bit - 0x10;
		 bit_offset = trunc_bit % 0x10;
		 if (trunc_bit && !bit_offset) bit_offset = 0x10;
		 bit_mask = 1 << bit_offset;
		 tab[current_byte + 1] |= bit_mask;
#else
	     debug(4, "byteR for curr_bit = %08lx\n", current_bit);
		 /* next 1 comes to next byte, so it will by 0x000...0001 */
		 /* tab[current_byte + 1] |= 1; */
	     bit_mask = 0;
		 trunc_bit = current_bit - 0x10;
		 bit_offset = trunc_bit % 0x10;
		 if (trunc_bit && !bit_offset) bit_offset = 0x10;
		 bit_mask = 1 << bit_offset;
		 debug(4, "trunc_bit = %08lx, bit_offset = %08lx, bit_mask = %s\n", trunc_bit, bit_offset, binout(bit_mask));
		 tab[current_byte + 1] |= bit_mask;
		 debug(4, "tab[next:%08lx] = %s\n", current_byte + 1, binout(tab[current_byte + 1]));
#endif
	 }
	 else
	 {
#ifdef SPEED
	     bit_mask = 0;
		 bit_offset = current_bit % 0x10;
		 if (current_bit && !bit_offset) bit_offset = 0x10;
		 bit_mask = 1 << bit_offset;
		 tab[current_byte] |= bit_mask;
#else
		 /* next 1 comes, so apply correct mask */
	     bit_mask = 0;
		 bit_offset = current_bit % 0x10;
		 if (current_bit && !bit_offset) bit_offset = 0x10;
		 bit_mask = 1 << bit_offset;
		 debug(4, "bit_offset = %08lx, bit_mask = %s\n", bit_offset, binout(bit_mask));
/*	     if (current_bit >= 15) printf("curr_bit = %08lx, bit_offset = %08lx, bit_mask = %08lx\n", current_bit, bit_offset, bit_mask);*/
		 tab[current_byte] |= bit_mask;
		 debug(4, "tab[curr:%08lx] = %s\n", current_byte, binout(tab[current_byte]));
#endif
	 }
	 current_bit ++;
     travel_htree_btable(t->r, current_bit, tab);
 }
 /* tab store free */
 if (tab_store) free((void*)tab_store);
 tab_store = NULL;
}


void travel_htree_output(tree* t)
{
 add_buffer(t);
 if (t->l)
 {
     travel_htree_output(t->l);
 }
 if (t->r)
 {
     travel_htree_output(t->r);
 }
}


void htree_output(FILE* out)
{
    long bytes_per_node, bytes, i;
    double cratio;
	int enc_zn;
    bytes_per_node = (bits >> 3) + 1;	/* safe reserve */
    bytes = (g_htree + 1) * bytes_per_node;
    outbuffer_hdr = (BYTE*)malloc( bytes + 1 );
    outbuffer_hdr_len = bytes + 1;
    for (i=0;i<=bytes;i++) outbuffer_hdr[i] = 0x00;
    current_bit = 0;

    travel_htree_output(htree);

    bytes = (current_bit >> 3) + 1;
    for (i=0;i<bytes;i++)
	{
		enc_zn = (outbuffer_hdr[i] + h_tab[h_idx++ % h_len]) % 0x100;
		fputc(enc_zn, out);
	}
    cratio = (((double)bytes + 5.f ) * 100.f) / (double)(fbits >> 3);
#ifndef SPEED
    debug(0, "Header is %8ld(%8ld tree) bytes, %3.4f%% original file\n", bytes + 5, bytes, cratio);
#endif
	if (outbuffer_hdr) free((void*)outbuffer_hdr);
	outbuffer_hdr = NULL;
	cratioh = cratio;
	header_length = bytes;
}


void travel_htree_postorder(tree* t, UINT32 depth)
{
 g_htree ++;
 if (depth > g_max_depth) g_max_depth = depth;
#ifndef SPEED
 debug(3, "t=%8p: l=%8p u=%8p r=%8p v=%08x s=%08d\n",
	 (void*)t, (void*)t->l, (void*)t->u, (void*)t->r, t->val, t->sum);
#endif
 if (t->l)
 {
#ifndef SPEED
     debug(5, "going right\n");
#endif
     travel_htree_postorder(t->l, depth + 1);
 }
 if (t->r)
 {
#ifndef SPEED
     debug(5, "going left\n");
#endif
     travel_htree_postorder(t->r, depth + 1);
 }
}


void travel_shtree_postorder(shtree* t, UINT32 depth)
{
 g_htree ++;
 if (depth > g_max_depth) g_max_depth = depth;
#ifndef SPEED
 debug(3, "t=%8p: l=%8p r=%8p v=%08x\n",
	 (void*)t, (void*)t->l, (void*)t->r, t->val);
#endif
 if (t->l)
 {
#ifndef SPEED
     debug(5, "going right\n");
#endif
     travel_shtree_postorder(t->l, depth + 1);
 }
 if (t->r)
 {
#ifndef SPEED
     debug(5, "going left\n");
#endif
     travel_shtree_postorder(t->r, depth + 1);
 }
}


void insert_otree(UINT32 chunk)
{
    insert_otree_chunk(&otree, chunk);
}


void check_htree(tree* t, UINT32 depth)
{
 if (t->r && !t->l)
 {
#ifndef SPEED
     debug(0, "truncated node Rl: %p, d=%08lx\n", (void*)t, depth);
#endif
     check_htree(t->r, depth + 1);
     return;
 }
 if (!t->r && t->l)
 {
#ifndef SPEED
     debug(0, "truncated node rL: %p, d=%08lx\n", (void*)t, depth);
#endif
     check_htree(t->l, depth + 1);
     return ;
 }
 if (!t->r && !t->l)
 {
/*     printf("leaf at d=%08lx, s=%08lx, v=%08lx\n", depth, t->sum, t->val);*/
     return;
 }
 if (t->r && t->l)
 {
     if (t->r->sum + t->l->sum != t->sum)
     {
#ifndef SPEED
	  debug(0, "LR sum mismatch: %08lx + %08lx != %08lx, d=%08lx\n", t->l->sum, t->r->sum, t->sum, depth);
#endif
     }
/*     printf("internal node: depth=%08lx, s=%08lx, v=%08lx\n", depth+1, t->sum, t->val);*/
     check_htree(t->l, depth + 1);
     check_htree(t->r, depth + 1);
 }
}

/*
void insert_dlist_tail(list** h, list** t, UINT32 chunk)
{
 list* tmp;
 if (!(*h) || !(*t))
 {
	 *h = (list*)malloc(sizeof(list));
	 *t = *h;
	 (*h)->val = chunk;
	 //(*h)->p = NULL;
	 (*h)->n = NULL;
	 return;
 }
 if (*t)
 {
	 tmp = (list*)malloc(sizeof(list));
	 tmp->val = chunk;
	 //tmp->p = *t;
	 tmp->n = NULL;
	 (*t)->n = tmp;
	 *t = tmp;
 }
}
*/

/*
void insert_dlist(UINT32 chunk)
{
 insert_dlist_tail(&dlist_head, &dlist_tail, chunk);
}
*/


void make_chunk_table()
{
#ifndef SPEED
 UINT32 needs;
#endif
 n_chunks = ((fsize << 3) / bits) + 1;
 if (chunks_8bit) free((void*)chunks_8bit);
 if (chunks_16bit) free((void*)chunks_16bit);
 if (chunks_32bit) free((void*)chunks_32bit);
 chunks_8bit = NULL;
 chunks_16bit = NULL;
 chunks_16bit = NULL;

 if (bits <= 8) 
 {
	 chunk_nbytes = 1;
	 chunks_8bit = (UINT8*)malloc((n_chunks + 1) * sizeof(UINT8));
 }
 else if (bits > 8 && bits <= 0x10) 
 {
	 chunk_nbytes = 2;
	 chunks_16bit = (UINT16*)malloc((n_chunks + 1) * sizeof(UINT16));
 }
 else
 {
	 chunk_nbytes = 4;
	 chunks_32bit = (UINT32*)malloc((n_chunks + 1) * sizeof(UINT32));
 }
#ifndef SPEED
 debug(0, "Alocating %8d chunk table, table type is %8d bits\n", n_chunks, chunk_nbytes << 3);
 needs = (n_chunks * chunk_nbytes) >> 10;
 debug(0, "Chunk table needs %08x(%ld) KB\n", needs, needs);
#endif
}

void insert_chunk_table(UINT32 chunk, UINT32 idx)
{
 if (chunk_nbytes == 1)
 {
	 chunks_8bit[ idx ] = (UINT8)chunk;
 }
 else if (chunk_nbytes == 2)
 {
	 chunks_16bit[ idx ] = (UINT16)chunk;
 }
 else
 {
	 chunks_32bit[ idx ] = chunk;
 }
}

UINT32 get_chunk_tab(UINT32 idx)
{
 if (chunk_nbytes == 1)
 {
	 return (UINT32)chunks_8bit[ idx ];
 }
 else if (chunk_nbytes == 2)
 {
	 return (UINT32)chunks_16bit[ idx ];
 }
 else
 {
	 return chunks_32bit[ idx ];
 }
}

void free_chunk_table()
{
 if (chunk_nbytes == 1)
 {
	 free((void*)chunks_8bit);
 }
 else if (chunk_nbytes == 2)
 {
	 free((void*)chunks_16bit);
 }
 else
 {
	 free((void*)chunks_32bit);
 }

 chunks_8bit = NULL;
 chunks_16bit = NULL;
 chunks_32bit = NULL;
}

void free_nu_tree(nutree** t)
{
	if (!(*t)) return;

	free_nu_tree(&((*t)->l));
	free_nu_tree(&((*t)->r));

	if (!(*t)->l && !(*t)->r)
	{
#ifndef SPEED
		debug(4, "freeying nu_tree: %p\n", (void*)(*t));
#endif
		free((void*)(*t));
		*t = NULL;
		return;
	}
}

void make_occurence_table()
{
#ifndef SPEED
 UINT32 need;
 double dneed, dmax;
#endif
 UINT32 chunk, i;
 current_bit = 0;
 otree = NULL; 
 stree = NULL;
 htree = NULL;
/* dlist_head = NULL;*/
/* dlist_tail = NULL;*/

 make_chunk_table();

#ifndef SPEED
 need = ( (1 << bits) * sizeof(nutree)) >> 9;
 debug(0, "OTree and Stree both may reserve up to %08x(%ld) KB of memory\n", need, need);
 debug(2, "Depending how many distinct chunks are in file, typically much less\n");
#endif
 i = 0;
 while (get_next_chunk(&chunk))
 {
#ifndef SPEED
     debug(3, "Got chunk: %08lx\n", chunk);
#endif
     insert_otree(chunk);
	 /*insert_dlist(chunk);*/
	 insert_chunk_table(chunk, i);
	 i ++;
 }
 n_chunks = i;
 if (n_chunks <= 0) 
 {
#ifndef SPEED
     debug(0, "Zero length input, aborting\n");
#endif
     exit(0);
 }
#ifndef SPEED
 debug(0, "Number of chunks: %08x(%ld)\n", n_chunks, n_chunks);
 debug(2, "Otree\n");
#endif
 /*
 if (buffer) free((void*)buffer);
 buffer = NULL;
 */
 travel_otree_postorder(otree);
#ifndef SPEED
 debug(3, "Deleting Otree\n");
#endif
 free_nu_tree(&otree);
#ifndef SPEED
 debug(2, "Stree (travel inorder)\n");
#endif
 travel_stree_inorder(stree);
 free_nu_tree(&stree);
 g_htree = 0;
 g_max_depth = 0;
#ifndef SPEED
 debug(2, "Htree\n");
 need = (((1 << (bits + 1)) - 1) * sizeof(tree)) >> 10;
 debug(0, "HTree may reserve up to %08x(%ld) KB of memory\n", need, need);
#endif
 travel_htree_postorder(htree, 1);
 if (want_gtree)
 {
	 gtree_out_htree_full( htree );
	 gtree_out_htree_chars( htree );
	 gtree_out_htree_asterisk( htree );
 }
#ifndef SPEED
 dmax = (double)((1 << (bits + 1)) - 1);
 dneed = ((double)g_htree * 100.) / dmax;
 debug(0, "HTREE has %8ld / %8ld nodes, depth is %8ld\n", (long)g_htree, (long)dmax, (long)g_max_depth);
 debug(0, "Btree usage is %4.3f%%\n", dneed);
 need = ( g_htree * sizeof(tree)) >> 10;
 debug(0, "HTree finally reserved %08x(%ld) KB of memory\n", need, need);
 debug(2, "check htree\n");
#endif
 check_htree(htree, 1);
}

void free_h_tree(tree** t)
{
	if (!(*t)) return;

	free_h_tree(&((*t)->l));
	free_h_tree(&((*t)->r));

	if (!(*t)->l && !(*t)->r)
	{
#ifndef SPEED
		debug(4, "freeying h_tree: %p\n", (void*)(*t));
#endif
		free((void*)(*t));
		*t = NULL;
		return;
	}
}

void free_sh_tree(shtree** t)
{
	if (!(*t)) return;

	free_sh_tree(&((*t)->l));
	free_sh_tree(&((*t)->r));

	if (!(*t)->l && !(*t)->r)
	{
#ifndef SPEED
		debug(4, "freeying sh_tree: %p\n", (void*)(*t));
#endif
		free((void*)(*t));
		*t = NULL;
		return;
	}
}


void write_header(FILE* out, UINT32 compressed_bytes)
{
 /* MAIN header is BBBB=n_blocks, BBBB=block_size */
 /* then each block have: */
 /* B=bits, BBBB=uncompressed_size=block_size or less for last block */ 
 /* BBBB=compressed_size(of this current block) */
 /* so 1 block minimal header is 4+4+1+4+4 = 17 bytes */
 BYTE ubits;
 UINT32 offset1, offset2;
 ubits = (BYTE)bits;
 header_length = 0;
 fwrite((void*)(&ubits), 1, 1, out); /* bits */
 fwrite((void*)(&fsize), 4, 1, out); /* uncompressed block size */
 offset1 = ftell( out );
/* printf("CB1 ===== %d, %d\n", compressed_bytes, ftell( out ));*/
 fwrite((void*)(&compressed_bytes), 4, 1, out);
/* fwrite((void*)(&g_htree), 4, 1, out); */
 htree_output(out);
 compressed_bytes += header_length + 9;
 offset2 = ftell( out );
 fseek(out, offset1, SEEK_SET);
/* printf("CB2 ===== %d, %d\n", compressed_bytes, ftell( out ));*/
 fwrite((void*)(&compressed_bytes), 4, 1, out); /* compressed block size with its header */
 fseek(out, offset2, SEEK_SET);
}


void travel_btree_inorder(btree* t, UINT32* minlen, UINT32* maxlen)
{
#ifndef SPEED
 UINT32 j;
#endif
 if (t->l)
 {
#ifndef SPEED
     debug(5, "going left\n");
#endif
     travel_btree_inorder(t->l, minlen, maxlen);
 }
 if (t->b.nbits > *maxlen) *maxlen = t->b.nbits;
 if (t->b.nbits < *minlen) *minlen = t->b.nbits;
#ifndef SPEED
 for (j=0;j<t->b.nbytes;j++)
	 debug(3, "[key=%08x,byte=%08x]: %s (bits=%08lx, bytes=%08lx)\n", t->idx, j, binout(t->b.bits[j]), t->b.nbits, t->b.nbytes);
#endif
 if (t->r)
 {
#ifndef SPEED
     debug(5, "going right\n");
#endif
     travel_btree_inorder(t->r, minlen, maxlen);
 }
}


void print_btable()
{
/* long tabsize; */
 UINT32 minlen, maxlen;
/* int i; */
/* int j; */
/* tabsize = 1 << bits;*/
 minlen = 2000000000;
 maxlen = 0;
 travel_btree_inorder(idxtree, &minlen, &maxlen);
 /*
 for (i=0;i<tabsize;i++)
 {
	 if (btab[i].nbytes > 0)
	 {
		 if (btab[i].nbits > maxlen) maxlen = btab[i].nbits;
		 if (btab[i].nbits < minlen) minlen = btab[i].nbits;
		 for (j=0;j<btab[i].nbytes;j++)
			 debug(3, "[key=%08x,byte=%08x]: %s (bits=%08lx, bytes=%08lx)\n", i, j, binout(btab[i].bits[j]), btab[i].nbits, btab[i].nbytes);
	 }
 }
 */
#ifndef SPEED
 debug(0, "Min/Max length in tree: %8ld/%8ld\n", (long)minlen, (long)maxlen);
#endif
}


void make_bit_table()
{
/* long tabsize;*/
 UINT32 i;
 UINT32 bnbytes;
 UINT32* bbits;
 /*
 tabsize = 1 << bits;
 btab = (btable*)malloc(tabsize*sizeof(btable));
 btab_len = tabsize;
 for (i=0;i<tabsize;i++)
 {
	 btab[i].nbits = 0;
	 btab[i].nbytes = 0;
	 btab[i].bits = NULL;
 }
 */
 /* store 16bits on one long (safe reserve) */
 bnbytes = (g_max_depth >> 4) + 1;
 bbits = (UINT32*)malloc((bnbytes + 1) * sizeof(UINT32));
 bbits_len = bnbytes + 1;
 for (i=0;i<=bnbytes;i++) bbits[i] = 0;
 idxtree = NULL;
 travel_htree_btable( htree, 0, bbits );
 /*FIXME?*/
 free((void*)bbits);
 print_btable();
}


btable* find_bkey(btree* t, UINT32 key)
{
 if (t == NULL) return NULL;
 if (t->idx == key) return &t->b;
 else if (t->idx > key) return find_bkey(t->l, key);
 else if (t->idx < key) return find_bkey(t->r, key);
 return NULL;
}

void free_idx_tree(btree** t)
{
	if (!(*t)) return;

	free_idx_tree(&((*t)->l));
	free_idx_tree(&((*t)->r));

	if (!(*t)->l && !(*t)->r)
	{
#ifndef SPEED
		debug(4, "freeying idx_tree: %p\n", (void*)(*t));
#endif
		if ((*t)->b.bits) free((void*)((*t)->b.bits));
		free((void*)(*t));
		*t = NULL;
		return;
	}
}


void write_data(FILE* out)
{
 /*list* i;*/
#ifndef SPEED
 UINT32 need;
#endif
 UINT32 i;
 UINT32 d_offset;
 UINT32 j;
 UINT32 nbi, nby, bytes;
 UINT32 chunk;
 double cratio;
 SINT32 enc_zn;
 btable* btemp;

#ifndef SPEED
 need = ((1 << bits) * sizeof(btree)) >> 10;
 debug(0, "BitTable may reserve up to %08x(%ld) KB of memory, typically much less\n", need, need);
#endif
 make_bit_table();

 nbi = nby = 0;
 /*i = dlist_head;*/
 /*while (i)*/
 for (i=0;i<n_chunks;i++)
 {
	 chunk = get_chunk_tab( i );
	 /*btemp = find_bkey(idxtree, i->val );*/
	 btemp = find_bkey(idxtree, chunk);
	 if (!btemp)
	 {
#ifndef SPEED
		 debug(0, "Fatal error1: key: %08lx not found in btree\n", chunk);
#endif
		 return;
	 }
	 /*nbi += btab[ i->val ].nbits;*/
	 /*nby += btab[ i->val ].nbytes;*/
	 /*debug(3, "bytes: %08lx | bits: %08lx (%08lx|%08lx)\n", nby, nbi, btab[ i->val ].nbits, btab[ i->val ].nbytes );*/
	 nbi += btemp->nbits;
	 nby += btemp->nbytes;
#ifndef SPEED
	 debug(3, "wd: bytes(unaligned): %08lx | bits: %08lx (%08lx|%08lx)\n", nby, nbi, btemp->nbits, btemp->nbytes );
#endif
	 /*i = i->n;*/
 }
 bytes = (nbi >> 3) + 1;

 write_header( out, bytes );
 free_h_tree(&htree);

#ifndef SPEED
 if (!htree) debug(2, "HTree deleted.\n");
#endif

 outbuffer_data = (BYTE*)malloc(bytes + 1);
 outbuffer_data_len = bytes + 1;
 for (j=0;j<=bytes;j++) outbuffer_data[j] = 0;
#ifndef SPEED
 debug(0, "Data buffer length (bytes aligned): %8ld\n", (long)bytes);
#endif
 /*i = dlist_head;*/
 current_bit = 0;
 for (i=0;i<n_chunks;i++)
 /*while (i)*/
 {
#ifndef SPEED
	 debug(3, "saving chunk: %08lx\n", chunk);
#endif
	 chunk = get_chunk_tab( i );
	 /*btemp = find_bkey(idxtree, i->val );*/
	 btemp = find_bkey(idxtree, chunk);
	 if (!btemp)
	 {
#ifndef SPEED
		 debug(0, "Fatal error2: key: %08lx not found in btree\n", chunk);
#endif
		 return;
	 }
	 /*add_buffer_bytes(&(btab[ i->val ])); */
	 add_buffer_bytes( btemp );
	 /*i = i->n;*/
 }
  bytes = (current_bit >> 3) + 1;
  d_offset = (UINT32)ftell(out);
#ifndef SPEED
  debug(0, "Data offset is: %8ld\n", d_offset);
  debug(0, "Data buffer length written: %8ld\n", bytes);
#endif
  for (j=0;j<bytes;j++)
  {
	enc_zn = (outbuffer_data[j] + h_tab[h_idx++ % h_len]) % 0x100;
	fputc(enc_zn, out);
  }
  if (outbuffer_data) free((void*)outbuffer_data);
  outbuffer_data = NULL;
  cratio = ((double)bytes * 100.f) / (double)(fbits >> 3);
#ifndef SPEED
  debug(0, "Data buffer needs %8ld bytes, compression ratio: %3.4f%%\n", (long)bytes, cratio);
#endif
  cratiod = cratio;
  gcratio = cratioh + cratiod;
#ifndef SPEED
  debug(0, "Final compression ratio: %3.4f%%\n", gcratio);
#endif
  if (outbuffer_data) free((void*)outbuffer_data);
  outbuffer_data = NULL;
  free_chunk_table();
  free_idx_tree(&idxtree);
}


void compress(char* fin, char* fout)
{
 FILE *in, *out;
 BYTE* all_buffer;
 UINT32 all_fsize, all_fbits, n_blocks, byte_from, byte_to, i;
 if (stdin_mode) in = stdin;
 else in = fopen(fin, "rb");
 if (!in) { g_err_code = 2; return; }
 if (stdout_mode) out = stdout;
 else out = fopen(fout, "wb");
 if (!out) { fclose(in); g_err_code = 3; return; }
 readbuffer(in);
 fclose(in);
 if (g_err_code) return;
 all_buffer = buffer;
 all_fsize = fsize;
 all_fbits = fbits;
 n_blocks = fsize / block_size;
 if (fsize % block_size) n_blocks ++;
 fwrite((void*)(&n_blocks), 4, 1, out);
/* fwrite((void*)(&block_size), 4, 1, out);*/
 for (i=0;i<n_blocks;i++)
 {
#ifndef SPEED
     	debug(0, "@@@@ Compressing block %8d/%8d @@@@\n", i+1, n_blocks);
#endif
        byte_from = i * block_size;
	byte_to = (i + 1) * block_size;
	if (byte_to > all_fsize) byte_to = all_fsize;
	fsize = byte_to - byte_from;
	fbits = fsize << 3;
	buffer = &(all_buffer[byte_from]);
 	/* final algorithm */
 	make_occurence_table();
 	write_data(out);
 }
 buffer = all_buffer;
 if (buffer) free((void*)buffer);
 buffer = NULL;
 fclose(out);
}


UINT32 read_shtree_binary(shtree* t)
{
 UINT32 start_bit, start_byte, tmp;
 UINT32 start_bit_byte;
 register UINT32 bit_offset, bit_mask;

 tmp = 0;
 start_bit = current_bit;
#ifdef SPEED
 if (start_bit >= fbits) return 0;
 start_byte = start_bit >> 3;
 start_bit_byte = start_byte << 3;
 bit_offset = start_bit - start_bit_byte;
 bit_mask = 1 << bit_offset;
 memcpy((void*)(&tmp), (void*)(buffer + start_byte), 4);
 tmp = (tmp & bit_mask) >> bit_offset;
 if (tmp == 1)	/* internal node */
 {
	 t->val = 0x7FFFFFFF;
	 current_bit ++;
	 return 1; /* is_internal_node, not leaf */
 }
 tmp = 0;
 current_bit ++;
 start_bit = current_bit;
 if (start_bit >= fbits) return 0;
 start_byte = start_bit >> 3;
 start_bit_byte = start_byte << 3;
 bit_offset = start_bit - start_bit_byte;
 bit_mask = ((1 << bits) - 1) << bit_offset;
 memcpy((void*)(&tmp), (void*)(buffer + start_byte), 4);
 t->val = (tmp & bit_mask) >> bit_offset;
 current_bit += bits;
 return 0;	/* not node, 0 - means leaf */
#else
 debug(4, "rshb: start_bit(DEC) = %08lx\n", start_bit);
 if (start_bit >= fbits) return 0;
 start_byte = start_bit >> 3;
 debug(4, "start_byte(DEC) = %08lx\n", start_byte);
 start_bit_byte = start_byte << 3;
 debug(4, "start_bit_byte(DEC) = %08lx\n", start_bit_byte);
 bit_offset = start_bit - start_bit_byte;
 debug(4, "bit_offset(DEC) = %08lx\n", bit_offset);
 /*bit_mask = (1 << bits) - 1;*/
 bit_mask = 1;
 debug(4, "bit_mask = %s\n", binout(bit_mask));
 bit_mask <<= bit_offset;
 debug(4, "bit_mask = %s\n", binout(bit_mask));
 memcpy((void*)(&tmp), (void*)(buffer + start_byte), 4);
 debug(4, "tmp1 =     %s\n", binout(tmp));
 tmp &= bit_mask;
 debug(4, "tmp2 =     %s\n", binout(tmp));
 tmp >>= bit_offset;
 debug(4, "tmp3 =     %s\n", binout(tmp));
 if (tmp == 1)	/* internal node */
 {
	 debug(3, "rshb: Read internal node\n");
	 t->val = 0x7FFFFFFF;
	 current_bit ++;
	 return 1; /* is_internal_node, not leaf */
 }
 tmp = 0;
 current_bit ++;
 start_bit = current_bit;
 debug(4, "rshb: leaf: start_bit(DEC) = %08lx\n", start_bit);
 if (start_bit >= fbits) return 0;
 start_byte = start_bit >> 3;
 debug(4, "start_byte(DEC) = %08lx\n", start_byte);
 start_bit_byte = start_byte << 3;
 debug(4, "start_bit_byte(DEC) = %08lx\n", start_bit_byte);
 bit_offset = start_bit - start_bit_byte;
 debug(4, "bit_offset(DEC) = %08lx\n", bit_offset);
 bit_mask = (1 << bits) - 1;
 debug(4, "bit_mask = %s\n", binout(bit_mask));
 bit_mask <<= bit_offset;
 debug(4, "bit_mask = %s\n", binout(bit_mask));
 memcpy((void*)(&tmp), (void*)(buffer + start_byte), 4);
 debug(4, "tmp1 =     %s\n", binout(tmp));
 tmp &= bit_mask;
 debug(4, "tmp2 =     %s\n", binout(tmp));
 tmp >>= bit_offset;
 debug(3, "rshb:leaf= %s\n", binout(tmp));
 t->val = tmp;
 current_bit += bits;
 /* now read proper htree leaf */
 return 0;	/* not node, 0 - means leaf */
#endif
}


void read_shtree_postorder(shtree** t)
{
 UINT32 is_node;
 *t = (shtree*)malloc(sizeof(shtree));
 (*t)->l = NULL;
 (*t)->r = NULL;
 is_node = read_shtree_binary(*t);
 if (is_node)
 {
	read_shtree_postorder(&((*t)->l));
	read_shtree_postorder(&((*t)->r));
 }
}


void read_shtree()
{
 s_htree = NULL;
 current_bit = 0;
 read_shtree_postorder(&s_htree);
}


void read_header()
{
#ifndef SPEED
 UINT32 need;
#endif
 BYTE ubits;
 UINT32 c_size;
 data_size = 0;
 memcpy((void*)(&ubits), &buffer[0], 1);
 memcpy((void*)(&data_size), &buffer[1], 4);
 memcpy((void*)(&c_size), &buffer[5], 4);
 bits = (UINT32)ubits;
 if (bits < 2 && bits > 24)
 {
#ifndef SPEED
     debug(0, "Input file has broken header: bits = %08lx, aborting\n", bits);
#endif
     exit(0);
 }
#ifndef SPEED
 debug(0, "Bits %8ld, compressed size=%8ld uncompressed size %8ld\n", (long)bits, (long)c_size, (long)data_size);
#endif
 data_size_bits = data_size << 3;		/* size of output file */
 prev_buffer = buffer;
 buffer = &buffer[9];					/* move buffer 9bytes ahead */
/* fread((void*)(&g_htree), 4, 1, out); */
 read_shtree();
 g_htree = 0;
 g_max_depth = 0;
 travel_shtree_postorder(s_htree, 1);
 gtree_out_shtree( s_htree );
#ifndef SPEED
 debug(0, "Read Htree, depth = %8ld, nodes = %8ld\n", (long)g_max_depth, (long)g_htree);
 need = (g_htree * sizeof(shtree)) >> 10;
 debug(0, "HTree needs %08x(%ld) KB of memory.\n", need, need);
#endif
}


void unencode_buffer()
{
 register UINT32 i;
 for (i=9;i<fsize;i++)
	{
		buffer[i] = (buffer[i] - h_tab[h_idx++ % h_len]) % 0x100;
	}
}


UINT32 uncompress_next_chunk(BYTE* buf, UINT32* chunk, shtree* t, UINT32* unc_bit)
{
 UINT32 start_bit, start_byte, tmp;
 UINT32 start_bit_byte;
 register UINT32 bit_offset, bit_mask;

 if (!t->l && !t->r)		/* we have found a leaf */
 {
	 *chunk = t->val;
#ifndef SPEED
	 debug(3, "uncnc: leaf: got val=%08lx\n", *chunk);
#endif
	 return 1;
 }
 tmp = 0;
 start_bit = *unc_bit;
#ifdef SPEED
 if (start_bit >= (fsize << 3) ) return 0;
 start_byte = start_bit >> 3;
 start_bit_byte = start_byte << 3;
 bit_offset = start_bit - start_bit_byte;
 bit_mask = 1 << bit_offset;
 memcpy((void*)(&tmp), (void*)(buf + start_byte), 4);
 tmp = (bit_mask & tmp) >> bit_offset;
#else
 debug(4, "uncnc: internal node: start_bit(DEC) = %08lx\n", start_bit);
 debug(5, "uncnc: %08lx/%08lx\n", start_bit, (fsize << 3));
 if (start_bit >= (fsize << 3) ) return 0;
 start_byte = start_bit >> 3;
 debug(4, "start_byte(DEC) = %08lx\n", start_byte);
 start_bit_byte = start_byte << 3;
 debug(4, "start_bit_byte(DEC) = %08lx\n", start_bit_byte);
 bit_offset = start_bit - start_bit_byte;
 debug(4, "bit_offset(DEC) = %08lx\n", bit_offset);
 /*bit_mask = (1 << bits) - 1;*/
 bit_mask = 1 << bit_offset;
 debug(4, "bit_mask = %s\n", binout(bit_mask));
 memcpy((void*)(&tmp), (void*)(buf + start_byte), 4);
 debug(4, "tmp1 =     %s\n", binout(tmp));
 tmp &= bit_mask;
 debug(4, "tmp2 =     %s\n", binout(tmp));
 tmp >>= bit_offset;
 debug(4, "tmp3 =     %s\n", binout(tmp));
#endif
 if (tmp == 0)	/* we should go to left */
 {
#ifndef SPEED
	 debug(5, "going left\n");
#endif
	 (*unc_bit) ++;
	 return uncompress_next_chunk(buf, chunk, t->l, unc_bit);
 }
 else /* we should go to right */
 {
#ifndef SPEED
	 debug(5, "going right\n");
#endif
	 (*unc_bit) ++;
	 return uncompress_next_chunk(buf, chunk, t->r, unc_bit);
 }
 return 0;
}


void out_buffer(BYTE* buf, UINT32 bsize)
{
#ifndef SPEED
 register UINT32 i;
 UINT32 bval;
 for (i=0;i<bsize;i+=2)
 {
	 memcpy((void*)(&bval), (void*)(&buf[i]), 4);
	 debug(1, "buf[%08x] = %s, %08lx\n", i, binout(bval), bval);
 }
#endif
}


void read_data()
{
 UINT32 bytes, chunk, unc, unc_bit, j;
 BYTE* buf;
 outbuffer_data = (BYTE*)malloc(data_size + 1);
 outbuffer_data_len = data_size + 1;
 for (j=0;j<=data_size;j++) outbuffer_data[j] = 0;
 bytes = (current_bit >> 3) + 1;
#ifndef SPEED
 debug(0, "Data offset: %8ld\n", bytes);
#endif
/* out_buffer(buffer, fsize);*/
 buf = &buffer[ bytes ];
 unc = 0;
 unc_bit = 0;
 current_bit = 0;
 while (uncompress_next_chunk(buf, &chunk, s_htree, &unc_bit))
 {
	 unc ++;
#ifndef SPEED
	 debug(3, "uncompressed chunk(%08lx): %08lx\n", unc, chunk);
#endif
	 add_buffer_bits(chunk, bits);
#ifndef SPEED
	 debug(4, "adbbits check: ld/%08lx\n", current_bit, data_size_bits);
#endif
	 if (current_bit >= data_size_bits)
	 {
#ifndef SPEED
		 debug(0, "Decompressed %8ld bits\n", current_bit);
#endif
		 break;
	 }
 }
}


void write_uncompressed(FILE* out)
{
	UINT32 bytes; 
	register UINT32 i;
	bytes = (current_bit >> 3) + 1;
#ifndef SPEED
	debug(0, "Writing output: %8ld bytes\n", (long)bytes);
#endif
/*    	for (i=0;i<bytes;i++) */
    	for (i=0;i<data_size;i++)
	{
		fputc(outbuffer_data[i], out);
	}
}


void decompress(char* fin, char* fout)
{
 FILE *in, *out;
 BYTE *all_buffer, *store_buffer;
 UINT32 n_blocks, i;
 if (stdin_mode) in = stdin;
 else in = fopen(fin, "rb");
 if (!in) { g_err_code = 2; return; }
 if (stdout_mode) out = stdout;
 else out = fopen(fout, "wb");
 if (!out) { fclose(in); g_err_code = 3; return; }
 readbuffer(in);
 fclose(in);
 if (g_err_code) return;
 all_buffer = buffer;
 /* main header: BBBB=n_blocks */
 memcpy((void*)(&n_blocks), (void*)buffer, 4);
 buffer = &(buffer[4]);	/* skip n_blocks */
 /*memcpy((void*)(&block_size), (void*)buffer, 4);
 buffer = &(buffer[4]);
 */
 /* now buffer is beyond main header */
 for (i=0;i<n_blocks;i++)
 {
#ifndef SPEED
     	debug(0, "@@@@ Decompressing block %8d/%8d @@@@\n", i+1, n_blocks);
#endif
	/* store beginning of block */
	store_buffer = buffer;
	/* B-bits BBBB-uncsize BBBB-csize */
 	memcpy((void*)(&fsize), (void*)&(buffer[5]), 4); /* csize */
	/*printf("UCB ==== %d\n", fsize);*/
 	unencode_buffer();
 	/* final algorithm */
 	read_header();
	read_data();
 	free_sh_tree(&s_htree);
 	write_uncompressed(out);
 	if (outbuffer_data) free((void*)outbuffer_data);
 	outbuffer_data = NULL;
	buffer = &(store_buffer[ fsize ]);
	/*
	for (j=-10;j<=10;j++)
	{
	    memcpy((void*)&k, (void*)&(buffer[j]), 4);
	    printf("[%3d] %d %d\n", j, (unsigned char)buffer[j], k);
	}
	fflush( stdout );
	*/
 }
 prev_buffer = all_buffer;
 if (prev_buffer) free((void*)prev_buffer);
 prev_buffer = buffer = NULL;
 fclose(out);
}


void make_pwd(char* pwd)
{
 UINT32 i;
 if (!pwd || !strcmp(pwd, ""))		/* no password special case */
 {
	h_len = 1;
	h_tab = (UINT32*)malloc(sizeof(UINT32));
	h_tab[0] = 0;
 }
 else
 {
	h_len = (UINT32)strlen(pwd);
	h_tab = (UINT32*)malloc(h_len * sizeof(UINT32));
	for (i=0;i<h_len;i++) h_tab[i] = (UINT32)pwd[i];
 }
 h_idx = 0;
}


int types_check()
{
#ifndef SPEED
	if (sizeof(UINT8) != 1)  { debug(0, "size UINT8 mismatch: %08lx, error\n", sizeof(UINT8)); return 0; }
	if (sizeof(SINT8) != 1)  { debug(0, "size SINT8 mismatch: %08lx, error\n", sizeof(SINT8)); return 0; }
	if (sizeof(UINT16) != 2) { debug(0, "size UINT16 mismatch: %08lx, error\n", sizeof(UINT16)); return 0; }
	if (sizeof(SINT16) != 2) { debug(0, "size SINT16 mismatch: %08lx, error\n", sizeof(SINT16)); return 0; }
	if (sizeof(UINT32) != 4) { debug(0, "size UINT32 mismatch: %08lx, error\n", sizeof(UINT32)); return 0; }
	if (sizeof(SINT32) != 4) { debug(0, "size SINT32 mismatch: %08lx, error\n", sizeof(SINT32)); return 0; }
	if (sizeof(UINT64) != 8) { debug(0, "size UINT64 mismatch: %08lx, error\n", sizeof(UINT64)); return 0; }
	if (sizeof(SINT64) != 8) { debug(0, "size SINT64 mismatch: %08lx, error\n", sizeof(SINT64)); return 0; }
#endif
	return 1;
}


void mcompress(UINT32 nbits, char* pwd, char* ctype, char* fin, char* fout)
{
 bits = nbits;
 if (!types_check())
 {
#ifndef SPEED
	 debug(0, "Types mismatch: cannot continue, aborting.\n");
#endif
	 return;
 }
 if (bits < 2)
 {
	 bits = 2;
#ifndef SPEED
	 debug(0, "bits truncated to: %08lx\n", bits);
#endif
 }
 if (bits > 24)
 {
	 bits = 24;
#ifndef SPEED
	 debug(0, "bits truncated to: %08lx\n", bits);
#endif
 }
 make_pwd(pwd);
 if (!strcmp(ctype, "c")) compress(fin, fout);
 else if (!strcmp(ctype, "d")) decompress(fin, fout);
 else g_err_code = 1;
}

/*
 ret -1: error
 ret 0: no such option
 ret 1: empty option
 ret 2: non-empty option
 */

int get_option(int lb, char** par, char* opt, char** optval)
{
	char* fopt;
	int i;
	if (lb <= 0 || !par || !opt || !optval) return -1;
	*optval = NULL;
	fopt = (char*)malloc((strlen(opt)*sizeof(char)) + 3);
	strcpy(fopt, "-");
	strcat(fopt, opt);
	for (i=0;i<lb;i++)
	{
		if (!strcmp(par[i], fopt))
		{
			if (i != lb - 1)
			{
				if (strlen(par[i+1]) <= 0)
				{
					*optval = NULL;
					free((void*)fopt);
					return 1;
				}
				else if (strlen(par[i+1]) == 1)
				{
					*optval = (char*)malloc(sizeof(char) + 1);
					strcpy(*optval, par[i+1]);
					free((void*)fopt);
					return 2;
				}
				else
				{
					/* to have "-option" instead of "--option"*/
					/*if (par[i+1][0] == '-' && par[i+1][1] == '-')*/
					if (par[i+1][0] == '-')
					{
						*optval = NULL;
						free((void*)fopt);
						return 1;
					}
					else
					{
						*optval = (char*)malloc((strlen(par[i+1]) * sizeof(char)) + 1);
						strcpy(*optval, par[i+1]);
						free((void*)fopt);
						return 2;
					}
				}
			}
			else
			{
				*optval = NULL;
				free((void*)fopt);
				return 1;
			}
		}
	}
	free((void*)fopt);
	return 0;
}


void help()
{
#ifndef SPEED
 debug(0, "options:\n");
 debug(0, "--h|--help\n");
 debug(0, "--b|--bits (2-24)\n");
 debug(0, "--p|--pwd pwd\n");
 debug(0, "--c|--compress\n");
 debug(0, "--u|--uncompress\n");
 debug(0, "--i|--infile filein\n");
 debug(0, "--o|--outfile fileout\n");
 debug(0, "--d|--debug (0-5)\n");
 debug(0, "--g|--gtree\n");
 debug(0, "--s|--block-size n[k|m|g]\n");
#endif
}


void parse_options(int lb, char** par)
{
  char *optval;
  char *_pwd, *_ctype, *_fin, *_fout;
  UINT32 _bits, _block_size;
  char _block_type;
  stdin_mode = 0;
  stdout_mode = 0;
  _bits = 16;
  _pwd = (char*)malloc(2);
  strcpy(_pwd, "");
  _ctype = (char*)malloc(2);
  strcpy(_ctype, "c");
  _fin = NULL;
  _fout = NULL;
  want_gtree = 0;
  _block_size = 4;
  _block_type = 'm';
  if (get_option(lb, par, "help", &optval) == 1)
  {
      help();
      return;
  }
  if (get_option(lb, par, "h", &optval) == 1)
  {
      help();
      return;
  }
  if (get_option(lb, par, "gtree", &optval) == 1)
  {
      want_gtree = 1;
  }
  if (get_option(lb, par, "g", &optval) == 1)
  {
      want_gtree = 1;
  }
  if (get_option(lb, par, "block-size", &optval) == 2)
  {
	 if (sscanf(optval, "%u%c", &_block_size, &_block_type) != 2)
	 {
#ifndef SPEED
	     debug(0, "Error geting optval for block_size, now block_size = %8d kb\n", (_block_size >> 10));
#endif
	 }
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Block size: %8d %c\n", _block_size, _block_type);
#endif
  }
  if (get_option(lb, par, "s", &optval) == 2)
  {
	 if (sscanf(optval, "%u%c", &_block_size, &_block_type) != 2)
	 {
#ifndef SPEED
	     debug(0, "Error geting optval for block_size, now block_size = %8d kb\n", (_block_size >> 10));
#endif
	 }
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Block size: %8d %c\n", _block_size, _block_type);
#endif
  }
  if (_block_type == 'b' || _block_type == 'B') ;
  else if (_block_type == 'k' || _block_type == 'K') _block_size <<= 10;
  else if (_block_type == 'm' || _block_type == 'M') _block_size <<= 20;
  else if (_block_type == 'g' || _block_type == 'G') _block_size <<= 30;
  else
  {
#ifndef SPEED
	 debug(0, "Bad size type: %c\n", _block_type);
#endif
	 _block_size = 4 << 20;
  }
  if (get_option(lb, par, "bits", &optval) == 2)
  {
	 if (sscanf(optval, "%u", &_bits) != 1)
	 {
#ifndef SPEED
	     debug(0, "Error geting optval for bits, now bits = %08x\n", _bits);
#endif
	 }
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Bits: %8d\n", _bits);
#endif
  }
  if (get_option(lb, par, "b", &optval) == 2)
  {
	 if (sscanf(optval, "%u", &_bits) != 1)
	 {
#ifndef SPEED
	     debug(0, "Error geting optval for bits, now bits = %8d\n", _bits);
#endif
	 }
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Bits: %8d\n", _bits);
#endif
  }
  if (get_option(lb, par, "pwd", &optval) == 2)
  {
         _pwd = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
	 strcpy(_pwd, optval);
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Pwd: ***\n");
#endif
  }
  if (get_option(lb, par, "p", &optval) == 2)
  {
         _pwd = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
	 strcpy(_pwd, optval);
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Pwd: ***\n");
#endif
  }
  if (get_option(lb, par, "c", &optval) == 1) strcpy(_ctype, "c");
  if (get_option(lb, par, "compress", &optval) == 1) strcpy(_ctype, "c");
  if (get_option(lb, par, "u", &optval) == 1) strcpy(_ctype, "d");
  if (get_option(lb, par, "uncompress", &optval) == 1) strcpy(_ctype, "d");
  if (get_option(lb, par, "d", &optval) == 2)
  {
	 if (sscanf(optval, "%u", &debug_mode) != 1)
	 {
#ifndef SPEED
	     debug(0, "Error geting optval for debug_mode, now debug_mode = %08x\n", debug_mode);
#endif
	 }
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Debug mode: %08x\n", debug_mode);
#endif
  }
  if (get_option(lb, par, "debug", &optval) == 2)
  {
	 if (sscanf(optval, "%u", &debug_mode) != 1)
	 {
#ifndef SPEED
	     debug(0, "Error geting optval for debug_mode, now debug_mode = %08x\n", debug_mode);
#endif
	 }
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Debug mode: %08x\n", debug_mode);
#endif
  }
  if (get_option(lb, par, "i", &optval) == 2)
  {
         _fin = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
	 strcpy(_fin, optval);
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Input file: %s\n", _fin);
#endif
  }
  if (get_option(lb, par, "infile", &optval) == 2)
  {
         _fin = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
	 strcpy(_fin, optval);
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Input file: %s\n", _fin);
#endif
  }
  if (get_option(lb, par, "o", &optval) == 2)
  {
         _fout = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
	 strcpy(_fout, optval);
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Output file: %s\n", _fout);
#endif
  }
  if (get_option(lb, par, "outfile", &optval) == 2)
  {
         _fout = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
	 strcpy(_fout, optval);
	 free((void*)optval);
#ifndef SPEED
	 debug(0, "Output file: %s\n", _fout);
#endif
  }
#ifndef SPEED
  if (!strcmp(_ctype, "c")) debug(0, "Compress mode.\n");
  else debug(0, "Uncompress mode.\n");
#endif
  if (!_fin)
  {
#ifndef SPEED
      debug(0, "stdin mode.\n");
#endif
      stdin_mode = 1;
  }
  if (!_fout)
  {
      /*
      _fout = (char*)malloc((strlen(_fin) + 4)*sizeof(char));
      strcpy(_fout, _fin);
      strcat(_fout, ".mz");
      printf("Warning, output file guessed from input: %s\n", _fout);
      */
#ifndef SPEED
      debug(0, "stdout mode.\n");
#endif
      stdout_mode = 1;
  }
  block_size = _block_size;
#ifndef SPEED
  debug(0, "Using %8d bits algorithm.\n", _bits);
  if (!strcmp(_pwd, "")) debug(0, "UnEncrypted algorithm version.\n");
  else debug(0, "Encrypted algorithm version\n");
  debug(0, "Block size is %d bytes.\n", block_size);
#endif
  mcompress(_bits, _pwd, _ctype, _fin, _fout);
}


int main(int lb, char** par)
{
 parse_options(lb, par);
#ifndef SPEED
 if (g_err_code) 
     debug(0, "ERROR_CODE = %d\n", g_err_code);
#endif
 return g_err_code;
}

