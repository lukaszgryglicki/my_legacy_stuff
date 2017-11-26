#include "udf.h"
/* Ur defined function */

unsigned long color_comix(unsigned long x)
{
    register volatile unsigned long r, g, b;
    const unsigned long shl = 6;

    r = ((x & 0xff0000) >> (0x10 + shl)) << shl;
    g = ((x & 0xff00) >> (0x8 + shl)) << shl;
    b = ((x & 0xff) >> shl) << shl;

    return  (r << 0x10) + (g << 0x8) + b;
}

unsigned long edge_detect_comix(unsigned long x1, unsigned long x2)
{
    unsigned long f, r1, g1, b1, r2, g2, b2;
    const unsigned long shl = 6;

    r1 = (x1 & 0xff0000) >> 0x10;
    g1 = (x1 & 0xff00) >> 0x8;
    b1 = x1 & 0xff;

    r2 = (x2 & 0xff0000) >> 0x10;
    g2 = (x2 & 0xff00) >> 0x8;
    b2 = x2 & 0xff;

    f = abs((r1 + g1 + b1) - (r2 + g2 + b2));

    if (f < 16)
    {
	r1 = (r1 >> shl) << shl;
	g1 = (g1 >> shl) << shl;
	b1 = (b1 >> shl) << shl;
    }
    else
    {
	if (r1 > g1)
	{
	    if (r1 > b1) { r1 = 0x0; g1 = b1 = 0xff; }
	    else { b1= 0x0; r1 = g1 = 0xff; }
	}
	else
	{
	    if (g1 > b1) { g1 = 0x0; r1 = b1 = 0xff; }
	    else { b1 = 0x0; r1 = b1 = 0xff; }
	}
    }

    return  (r1 << 0x10) + (g1 << 0x8) + b1;
}

unsigned long smooth_gs6(unsigned long* x)
{
    unsigned long i, r, g, b, gs[6], f;

    f = 0;
    for (i=0;i<6;i++)
    {
        r = (x[i] & 0xff0000) >> 0x10;
        g = (x[i] & 0xff00) >> 0x8;
        b = x[i] & 0xff;

        gs[i] = (r + g + b) / 3;

	f += gs[i];
    }

    f /= 6;

    return (f << 0x10) + (f << 0x8) + f;
}

unsigned long color_offset6(unsigned long* x)
{
    register volatile unsigned long r, g, b;

    r = (((x[0] & 0xff0000) >> 0x10) + ((x[1] & 0xff0000) >> 0x10)) >> 1;
    g = (((x[2] & 0xff00) >> 0x8) + ((x[3] & 0xff00) >> 0x8)) >> 1;
    b = ((x[4] & 0xff) + (x[5] & 0xff)) >> 1;

    return (r << 0x10) + (g << 0x8) + b;
}

unsigned long color_offset3(unsigned long x1, unsigned long x2, unsigned long x3)
{
    /*
    unsigned long r, g, b;

    r = (x1 & 0xff0000) >> 0x10;
    g = (x2 & 0xff00) >> 0x8;
    b = x3 & 0xff;

    return (r << 0x10) + (g << 0x8) + b;
    */
/*    return (((x1 & 0xff0000) >> 0x10) << 0x10) + (((x2 & 0xff00) >> 0x8) << 0x8) + (x3 & 0xff);*/
    return (x1 & 0xff0000) + (x2 & 0xff00) + (x3 & 0xff);
}


unsigned long smooth_gs(unsigned long x1, unsigned long x2, unsigned long x3)
{
    unsigned long r, g, b, gs1, gs2, gs3, gs;

    r = (x1 & 0xff0000) >> 0x10;
    g = (x1 & 0xff00) >> 0x8;
    b = x1 & 0xff;

    gs1 = (r + g + b) / 3;

    r = (x2 & 0xff0000) >> 0x10;
    g = (x2 & 0xff00) >> 0x8;
    b = x2 & 0xff;

    gs2 = (r + g + b) / 3;

    r = (x3 & 0xff0000) >> 0x10;
    g = (x3 & 0xff00) >> 0x8;
    b = x3 & 0xff;

    gs3 = (r + g + b) / 3;

    gs = (gs1 + gs2 + gs3) / 3;

    return (gs << 0x10) + (gs << 0x8) + gs;
}

unsigned long edge_detect(unsigned long x1, unsigned long x2, unsigned long x3)
{
    unsigned long r1, g1, b1, r2, g2, b2;
    x3 = 25000 / x3;

/*    printf("DIFF=%d\n", x3);*/

    r1 = (x1 & 0xff0000) >> 0x10;
    g1 = (x1 & 0xff00) >> 0x8;
    b1 = x1 & 0xff;

    r2 = (x2 & 0xff0000) >> 0x10;
    g2 = (x2 & 0xff00) >> 0x8;
    b2 = x2 & 0xff;

    r1 = abs(r1 - r2);
    g1 = abs(g1 - g2);
    b1 = abs(b1 - b2);

    if (r1 > x3) r1 = r2;
    else r1 = 0;
    if (g1 > x3) g1 = g2;
    else g1 = 0;
    if (b1 > x3) b1 = b2;
    else b1 = 0;

    return (r1 << 0x10) + (g1 << 0x8) + b1;
}

double alien_gs3(double x1, double x2, double x3)
{
    unsigned long lx, lr, lg, lb, l1, l2, l3;
    double r,g,b,gs;

    lx = (unsigned long)x1;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;
    gs = .5 * (-1. * cos(gs * 3. * M_PI) + 1.);
    l1 = (unsigned long)(gs * 255.);

    lx = (unsigned long)x2;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;
    gs = .5 * (-1. * cos(gs * 3. * M_PI) + 1.);
    l2 = (unsigned long)(gs * 255.);

    lx = (unsigned long)x3;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;
    gs = .5 * (-1. * cos(gs * 3. * M_PI) + 1.);
    l3 = (unsigned long)(gs * 255.);

    lx = (l1 << 0x10) + (l2 << 0x8) + l3;

    x1 = (double)lx;

    return x1;
}

double alien_gs(double x)
{
    unsigned long lx, lr, lg, lb;
    double r,g,b,gs;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;

    gs = .5 * (-1. * cos(gs * 3. * M_PI) + 1.);

    lx = (unsigned long)(gs * 255.);

    lx = (lx << 0x10) + (lx << 0x8) + lx;

    x = (double)lx;

    return x;
}

unsigned long alien_toon3(unsigned long x1, unsigned long x2, unsigned long x3, int na, int nb, int nc)
{
    unsigned long r, g, b, gs, i;

    r = (x1 & 0xFF0000) >> 0x10;
    g = (x1 & 0xFF00) >> 0x8;
    b = x1 & 0xFF;

    gs = ((r << 2) + g * 5 + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }

    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    x1 = (gs >> nb) << nb;

    r = (x2 & 0xFF0000) >> 0x10;
    g = (x2 & 0xFF00) >> 0x8;
    b = x2 & 0xFF;

    gs = ((r << 2) + g * 5 + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }

    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    x2 = (gs >> nb) << nb;

    r = (x3 & 0xFF0000) >> 0x10;
    g = (x3 & 0xFF00) >> 0x8;
    b = x3 & 0xFF;

    gs = ((r << 2) + g * 5 + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }

    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    x3 = (gs >> nb) << nb;

    x3 <<= 1;
    if (x3 > 0xff) x3 = 0xff;

    x1 += 1 << nb;
    if (x1 > 0xff) x1 = 0xff;

    x1 = (x1 << 0x10) + (x2 << 0x8) + x3;

    return x1;
}

unsigned long alien_toongs2(unsigned long x, int na, int nb, int nc, int vm)
{
    unsigned long r, g, b, gs, i;

    r = (x & 0xFF0000) >> 0x10;
    g = (x & 0xFF00) >> 0x8;
    b = x & 0xFF;

    gs = ((r << 2) + 5 * g + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }
    
    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    gs = (gs >> nb) << nb;

    r = g = b = gs;

    if (vm & 1)
    {
        r <<= 1;
        if (r > 0xff) r = 0xff;
    }

    if (vm & 2)
    {
        g <<= 1;
        if (g > 0xff) g = 0xff;
    }

    if (vm & 4)
    {
        b <<= 1;
        if (b > 0xff) b = 0xff;
    }

    x = (r << 0x10) + (g << 0x8) + b;

    return x;
}

unsigned long alien_toongs(unsigned long x, int na, int nb, int nc)
{
    unsigned long r, g, b, gs, i;

    r = (x & 0xFF0000) >> 0x10;
    g = (x & 0xFF00) >> 0x8;
    b = x & 0xFF;

    gs = ((r << 2) + 5 * g + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }
    
    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    gs = (gs >> nb) << nb;

    r = g = b = gs;

    b <<= 1;
    b += 0x10;
    if (b > 0xff) b = 0xff;

    /*
    if (nb > 1)
    {
        r += 1 << (nb - 1);
       if (r > 0xff) r = 0xff;
    }
    */

    if (r < 0x28) r = 0x28;

    x = (r << 0x10) + (g << 0x8) + b;

    return x;
}

unsigned long alien_toon(unsigned long x, int na, int nb, int nc)
{
    unsigned long r, g, b, r2, g2, b2, i;

    r = (x & 0xFF0000) >> 0x10;
    g = (x & 0xFF00) >> 0x8;
    b = x & 0xFF;

    for (i=0;i<na;i++)
    {
        if (r < 0x80) r = (0x7F - r) << 1;
        else r = (r - 0x80) << 1;

        if (g < 0x80) g = (0x7F - g) << 1;
        else g = (g - 0x80) << 1;

        if (b < 0x80) b = (0x7F - b) << 1;
        else b = (b - 0x80) << 1;
    }

    r2 = (5*r + 2*g + b) >> 0x3;
    g2 = (5*g + 2*b + r) >> 0x3;
    b2 = (5*b + 2*r + g) >> 0x3;
    
    for (i=0;i<nc;i++)
    {
        r2 = (r2 * r2) >> 0x8;
        g2 = (g2 * g2) >> 0x8;
        b2 = (b2 * b2) >> 0x8;
    }

    r2 = (r2 >> nb) << nb;
    g2 = (g2 >> nb) << nb;
    b2 = (b2 >> nb) << nb;

    b2 <<= 1;
    if (b2 > 0xff) b2 = 0xff;

    if (nb > 1)
    {
        r2 += 1 << (nb - 1);
        if (r2 > 0xff) r2 = 0xff;
    }

    x = (r2 << 0x10) + (g2 << 0x8) + b2;

    return x;
}

unsigned long alien_gsi(unsigned long x)
{
    register volatile unsigned long r, g, b, gs;

    r = (x & 0xFF0000) >> 0x10;
    g = (x & 0xFF00) >> 0x8;
    b = x & 0xFF;

    gs = r + g + b;

    if (gs < 0x100)
    {
	x = gs;
    }
    else if (gs >= 0x100 && gs < 0x200)
    {
	x = 0x1ff - gs;
    }
    else
    {
	x = gs - 0x200;
    }

    x = (x << 0x10) + (x << 0x8) + x;

    return x;
}

unsigned long alien_gsi6(unsigned long* x)
{
    register volatile unsigned long i, r, g, b, gs;

    for (i=0;i<6;i++)
    {
        r = (x[i] & 0xFF0000) >> 0x10;
        g = (x[i] & 0xFF00) >> 0x8;
        b = x[i] & 0xFF;

        gs = r + g + b;

        if (gs < 0x100)
        {
	    x[i] = gs;
        }
        else if (gs >= 0x100 && gs < 0x200)
        {
	    x[i] = 0x1ff - gs;
        }
        else
        {
	    x[i] = gs - 0x200;
        }
    }

    r = (x[0] + x[1]) >> 1;
    g = (x[2] + x[3]) >> 1;
    b = (x[4] + x[5]) >> 1;

    gs = (r << 0x10) + (g << 0x8) + b;

    return gs;
}

__inline unsigned long color_gsi3(unsigned long x1, unsigned long x2, unsigned long x3)
{
    return (x1 & 0xff0000) + (x2 & 0xff00) + (x3 & 0xff);
}

unsigned long alien_gsi3(unsigned long x1, unsigned long x2, unsigned long x3)
{
    register volatile unsigned long r, g, b, gs;

    r = (x1 & 0xFF0000) >> 0x10;
    g = (x1 & 0xFF00) >> 0x8;
    b = x1 & 0xFF;

    gs = r + g + b;

    if (gs < 0x100)
    {
	x1 = gs;
    }
    else if (gs >= 0x100 && gs < 0x200)
    {
	x1 = 0x1ff - gs;
    }
    else
    {
	x1 = gs - 0x200;
    }

    r = (x2 & 0xFF0000) >> 0x10;
    g = (x2 & 0xFF00) >> 0x8;
    b = x2 & 0xFF;

    gs = r + g + b;

    if (gs < 0x100)
    {
	x2 = gs;
    }
    else if (gs >= 0x100 && gs < 0x200)
    {
	x2 = 0x1ff - gs;
    }
    else
    {
	x2 = gs - 0x200;
    }

    r = (x3 & 0xFF0000) >> 0x10;
    g = (x3 & 0xFF00) >> 0x8;
    b = x3 & 0xFF;

    gs = r + g + b;

    if (gs < 0x100)
    {
	x3 = gs;
    }
    else if (gs >= 0x100 && gs < 0x200)
    {
	x3 = 0x1ff - gs;
    }
    else
    {
	x3 = gs - 0x200;
    }

    x1 = (x1 << 0x10) + (x2 << 0x8) + x3;

    return x1;
}

double alien_sine(double x)
{
    unsigned long lx, lr, lg, lb;
    double r,g,b;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    r = .5* (sin(r * 2. * M_PI) + 1.);
    g = .5* (sin(g * 2. * M_PI) + 1.);
    b = .5* (sin(b * 2. * M_PI) + 1.);

    r = pow(r, r);
    g = pow(g, g);
    b = pow(b, b);

    r = 3.125* (r - 0.68);
    g = 3.125* (g - 0.68);
    b = 3.125* (b - 0.68);

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    if (lr > 255) lr = 255;
    if (lg > 255) lg = 255;
    if (lb > 255) lb = 255;

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double alien_sine_ex(double x, double rmul, double rfi, double gmul, double gfi, double bmul, double bfi)
{
    unsigned long lx, lr, lg, lb;
    double r,g,b;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    r = .5* (sin((r * r * 2. * M_PI * rmul) + rfi) + 1.);
    g = .5* (sin((g * g * 2. * M_PI * gmul) + gfi) + 1.);
    b = .5* (sin((b * b * 2. * M_PI * bmul) + bfi) + 1.);

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    if (lr > 255) lr = 255;
    else if (lr < 0) lr = 0;
    if (lg > 255) lg = 255;
    else if (lg < 0) lg = 0;
    if (lb > 255) lb = 255;
    else if (lb < 0) lb = 0;

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double alien_col_mul(double x, double rmul, double rfi, double gmul, double gfi, double bmul, double bfi)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    r2 = .5* (sin((sqrt(b) * r * 2. * M_PI * rmul) + rfi) + 1.);
    g2 = .5* (sin((sqrt(r) * g * 2. * M_PI * gmul) + gfi) + 1.);
    b2 = .5* (sin((sqrt(g) * b * 2. * M_PI * bmul) + bfi) + 1.);

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    if (lr > 255) lr = 255;
    else if (lr < 0) lr = 0;
    if (lg > 255) lg = 255;
    else if (lg < 0) lg = 0;
    if (lb > 255) lb = 255;
    else if (lb < 0) lb = 0;

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_mul(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    r2 = r * g;
    g2 = g * b;
    b2 = b * r;

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    if (lr > 255) lr = 255;
    else if (lr < 0) lr = 0;
    if (lg > 255) lg = 255;
    else if (lg < 0) lg = 0;
    if (lb > 255) lb = 255;
    else if (lb < 0) lb = 0;

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

unsigned long color_invi3(unsigned long x1, unsigned long x2, unsigned long x3)
{
    unsigned long r, g, b;

    g = 0xFF - ((x1 & 0xFF00) >> 0x8);
    b = 0xFF - (x1 & 0xFF);

    x1 = (g + b) >> 0x1;

    if (x1 < 0x80) x1 = (0x7F - x1) << 1;
    else x1 = (x1 - 0x80) << 1;

    r = 0xFF - ((x2 & 0xFF0000) >> 0x10);
    b = 0xFF - (x2 & 0xFF);

    x2 = (b + r) >> 0x1;

    if (x2 < 0x80) x2 = (0x7F - x2) << 1;
    else x2 = (x2 - 0x80) << 1;

    r = 0xFF - ((x3 & 0xFF0000) >> 0x10);
    g = 0xFF - ((x3 & 0xFF00) >> 0x8);

    x3 = (r + g) >> 0x1;

    if (x3 < 0x80) x3 = (0x7F - x3) << 1;
    else x3 = (x3 - 0x80) << 1;

    return (x1 << 0x10) + (x2 << 0x8) + x3;
}

unsigned long color_invi(unsigned long x)
{
    unsigned long r, g, b, r2, g2, b2;

    r = 0xFF - ((x & 0xFF0000) >> 0x10);
    g = 0xFF - ((x & 0xFF00) >> 0x8);
    b = 0xFF - (x & 0xFF);

    r2 = (g + b) >> 0x1;
    g2 = (b + r) >> 0x1;
    b2 = (r + g) >> 0x1;

    if (r2 < 0x80) r2 = (0x7F - r2) << 1;
    else r2 = (r2 - 0x80) << 1;

    if (g2 < 0x80) g2 = (0x7F - g2) << 1;
    else g2 = (g2 - 0x80) << 1;

    if (b2 < 0x80) b2 = (0x7F - b2) << 1;
    else b2 = (b2 - 0x80) << 1;

    return (r2 << 0x10) + (g2 << 0x8) + b2;
}

double color_inv2(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = 1. - (double)lr / 255.;
    g = 1. - (double)lg / 255.;
    b = 1. - (double)lb / 255.;

    r2 = b * g;
    g2 = r * b;
    b2 = g * r;

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double alien_inv(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = 1. - (double)lr / 255.;
    g = 1. - (double)lg / 255.;
    b = 1. - (double)lb / 255.;

    /*
    r2 = sqrt(b) * g;
    g2 = sqrt(r) * b;
    b2 = sqrt(g) * r;
    */

    r2 = b * g;
    g2 = r * b;
    b2 = g * r;

    r = .5 * (-cos( r2 * 2. * M_PI ) + 1.);
    g = .5 * (-cos( g2 * 2. * M_PI ) + 1.);
    b = .5 * (-cos( b2 * 2. * M_PI ) + 1.);

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

void get_heat(double x, double l, double* or, double* og, double* ob)
{
    double f, r, g, b;
    /*
    0/6 = red
    1/6 = yellow
    2/6 = green
    3/6 = teal
    4/6 = blue
    5/6 = pink
    */

    if (x < 0.0 || x > 1.0 || l < 0.0 || l > 1.0)
    {
	printf("x=%f, l=%f --> error\n", x, l);
	*or = *og = *ob = 1.;
	return;
    }

    if (x <= 1./6.)  /* red --> yellow */
    {
	f = 6. * x;

	r = l;
	g = f * l;
	b = 0.;
    }
    else if (x <= 2./6.) /* yellow --> green */
    {
	f = 6. * x - 1.;

	r = (1. - f) * l;
	g = l;
	b = 0.;
    }
    else if (x <= 3./6.) /* green --> teal */
    {
	f = 6. * x - 2.;

	r = 0.;
	g = l;
	b = f * l;
    }
    else if (x <= 4./6.) /* teal --> blue */
    {
	f = 6. * x - 3.;

	r = 0.;
	g = (1. - f) * l;
	b = l;
    }
    else if (x <= 5./6.) /* blue --> pink */
    {
	f = 6. * x - 4.;

	r = f * l;
	g = 0.;
	b = l;
    }
    else if (x <= 6./6.) /* pink --> red */
    {
	f = 6. * x - 5.;

	r = l;
	g = 0.;
	b = (1. - f) * l;
    }
    else r = g = b = 1.;

/*    printf("(%f,%f) --> (%f,%f,%f)\n", x, l, r, g, b);*/

    *or = r;
    *og = g;
    *ob = b;
}

double color_hue2(double x)
{
    unsigned long lx, lr, lg, lb;
    double /*r, */g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

/*    r = (double)lr / 255.;*/
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    get_heat(b, g, &r2, &g2, &b2);

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;

}

double color_hue(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, gs, rr, gr, br, rg, gg, bg, rb, gb, bb;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;
    
    get_heat(r, gs, &rr, &rg, &rb);
    get_heat(g, gs, &gr, &gg, &gb);
    get_heat(b, gs, &br, &bg, &bb);

    r = (rr < rg) ? ((rr > rb) ? rr : rb) : ((rg > rb) ? rg : rb);
    g = (gr < gg) ? ((gr > gb) ? gr : gb) : ((gg > gb) ? gg : gb);
    b = (br < bg) ? ((br > bb) ? br : bb) : ((bg > bb) ? bg : bb);

    r = 1. - cos(r * M_PI) * cos(r * M_PI);
    g = 1. - cos(g * M_PI) * cos(g * M_PI);
    b = 1. - cos(b * M_PI) * cos(b * M_PI);

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;

}

double color_complex(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;
    double re1, im1, l1, fi1;
    double re2, im2, l2, fi2;
    double re3, im3, l3, fi3;
    double /*l, */fi;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    re1 = g/sqrt(2.);
    im1 = b/sqrt(2.);
    l1 = sqrt(re1 * re1 + im1 * im1);
    if (l1 > 0.) fi1 = acos(re1 / l1);
    else fi1 = 0.;

    re2 = r/sqrt(2.);
    im2 = g/sqrt(2.);
    l2 = sqrt(re2 * re2 + im2 * im2);
    if (l2 > 0.) fi2 = acos(re2 / l2);
    else fi2 = 0.;

    re3 = b/sqrt(2.);
    im3 = r/sqrt(2.);
    l3 = sqrt(re3 * re3 + im3 * im3);
    if (l3 > 0.) fi3 = acos(re3 / l3);
    else fi3 = 0.;

/*    l = l1 * l2 * l3;*/
    fi = fi1 + fi2 + fi3;

/*    printf("(r=%1.4f, g=%1.4f, b=%1.4f, re=%1.4f, im=%1.4f, l=%1.4f, fi=%1.4f)\n", r, g, b, re, im, l, fi);*/
    fi /= 1.5 * M_PI;

    get_heat( fi2 / M_PI, l2, &r2, &g2, &b2);

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}


double color_angle(double x)
{
    unsigned long lx, lr, lg, lb;
    double f, r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    f = sqrt(g*g + b*b);
    if (f > 0.) r2 = g / f;
    else r2 = 0.;
    
    f = sqrt(b*b + r*r);
    if (f > 0.) g2 = b / f;
    else g2 = 0.;

    f = sqrt(r*r + g*g);
    if (f > 0.) b2 = r / f;
    else b2 = 0.;

    r2 = asin(r2) / M_PI;
    g2 = asin(g2) / M_PI;
    b2 = asin(b2) / M_PI;

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

unsigned long color_heati2(unsigned long x)
{
    volatile register unsigned long r, g, b, gs, f;

    r = (x & 0xff0000) >> 0x10;
    g = (x & 0xff00) >> 0x8;
    b = x & 0xff;

    gs = r + g + b;
    f = gs;

    if (gs <= 110)	/* gray to violet */
    {
	f = gs << 1;

	r = 220 / 3;
	g = 220 / 3 - f / 3;
	b = 220 / 3 + f / 3;
    }
    else if (gs <= 220)	/* violet --> blue */
    {
	f = (gs - 110) << 1;

	r = 220 / 3 - f / 3;
	g = 0;
	b = 440 / 3 + f / 3;
    }
    else if (gs <= 330) /* blue --> teal */
    {
	f = gs - 220;

	r = 0;
	g = f; 
	b = 220 - f;
    }
    else if (gs <= 440) /* teal --> green */
    {
	f = gs - 330;

	r = 0;
	g = 110 + f;
	b = 110 - f;
    }
    else if (gs <= 550) /* green --> yellow */
    {
	f = gs - 440;

	r = f;
	g = 220 - f;
	b = 0;
    }
    else if (gs <= 660) /* yellow --> red */
    {
	f = gs - 550;

	r = 110 + f;
	g = 110 - f;
	b = 0;
    }
    else		/* read --> white */
    {
	f = (gs - 660) << 2;

	r = 220 - f / 3;
	g = f / 6;
	b = f / 6;
    }

    r += 0x10;
    g += 0x10;
    b += 0x10;

    f = (r << 0x10) + (g << 0x8) + b;

    return f;
}

unsigned long color_heati(unsigned long x)
{
    volatile register unsigned long r, g, b, gs, f;

    r = (x & 0xff0000) >> 0x10;
    g = (x & 0xff00) >> 0x8;
    b = x & 0xff;

    gs = r + g + b;
    f = gs;

    if (gs <= 110)
    {
	r = gs;
	g = 0;
	b = gs << 1;
    }
    else if (gs <= 220)
    {
	r = 220 - f;
	g = 0;
	b = 220;
    }
    else if (gs <= 330)
    {
	f = gs - 220;
	r = 0;
	g = f << 1;
	b = 220;
    }
    else if (gs <= 440)
    {
	f = gs - 330;
	r = 0;
	g = 220;
	b = 220 - (f << 1);
    }
    else if (gs <= 550)
    {
	f = gs - 440;
	r = f << 1;
	g = 220;
	b = 0;
    }
    else if (gs <= 660)
    {
	f = gs - 550;
	r = 220;
	g = 220 - (f << 1);
	b = 0;
    }
    else
    {
	f = gs - 660;
	r = 220;
	g = f << 1;
	b = f << 1;
    }

    f = (r << 0x10) + (g << 0x8) + b;

    return f;
}

double color_heat2(double x)
{
    unsigned long lx, lr, lg, lb;
    double gs, f, r, g, b;
/*    black 	(0,0,0)*/
/*    violet	(1,0,1)*/
/*    blue     (0,0,1)*/
/*    teal     (0,1,1)*/
/*    green    (0,1,0)*/
/*    yellow   (1,1,0)*/
/*    red      (1,0,0)*/
/*    white    (1,1,1)*/

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    gs = (double)(lr + lg + lb) / 765.;

    if (gs < (1./7.))
    {
	f = 7. * gs;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = .6 * f;
	g = 0.;
	b = f;
    }
    else if (gs >= (1./7.) && gs < (2./7.))
    {
	f = (7. * gs) - 1.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = .6 * (1. - f);
	g = 0.;
	b = 1.;
    }
    else if (gs >= (2./7.) && gs < (3./7.))
    {
	f = (7. * gs) - 2.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = 0.;
	g = f;
	b = 1.;
    }
    else if (gs >= (3./7.) && gs < (4./7.))
    {
	f = (7. * gs) - 3.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = 0.;
	g = 1.;
	b = 1. - f;
    }
    else if (gs >= (4./7.) && gs < (5./7.))
    {
	f = (7. * gs) - 4.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = f;
	g = 1.;
	b = 0.;
    }
    else if (gs >= (5./7.) && gs < (6./7.))
    {
	f = (7. * gs) - 5.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = 1.;
	g = 1. - f;
	b = 0.;
    }
    else if (gs >= (6./7.))
    {
	f = (7. * gs) - 6.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = 1.;
	g = f;
	b = f;
    }
    else
    {
	r = g = b = .5;
    }

    r = .5 * (-cos( r * M_PI) + 1.);
    g = .5 * (-cos( g * M_PI) + 1.);
    b = .5 * (-cos( b * M_PI) + 1.);


    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_heat(double x)
{
    unsigned long lx, lr, lg, lb;
    double gs, f, r, g, b;
/*    black 	(0,0,0)*/
/*    violet	(1,0,1)*/
/*    blue     (0,0,1)*/
/*    teal     (0,1,1)*/
/*    green    (0,1,0)*/
/*    yellow   (1,1,0)*/
/*    red      (1,0,0)*/
/*    white    (1,1,1)*/

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    gs = (double)(lr + lg + lb) / 765.;

    if (gs < (1./7.))
    {
	f = 7. * gs;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = .6 * f;
	g = 0.;
	b = f;
    }
    else if (gs >= (1./7.) && gs < (2./7.))
    {
	f = (7. * gs) - 1.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = .6 * (1. - f);
	g = 0.;
	b = 1.;
    }
    else if (gs >= (2./7.) && gs < (3./7.))
    {
	f = (7. * gs) - 2.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = 0.;
	g = f;
	b = 1.;
    }
    else if (gs >= (3./7.) && gs < (4./7.))
    {
	f = (7. * gs) - 3.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = 0.;
	g = 1.;
	b = 1. - f;
    }
    else if (gs >= (4./7.) && gs < (5./7.))
    {
	f = (7. * gs) - 4.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = f;
	g = 1.;
	b = 0.;
    }
    else if (gs >= (5./7.) && gs < (6./7.))
    {
	f = (7. * gs) - 5.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = 1.;
	g = 1. - f;
	b = 0.;
    }
    else if (gs >= (6./7.))
    {
	f = (7. * gs) - 6.;
        f = .5 * ( -cos( f * M_PI ) + 1. );

	r = 1.;
	g = f;
	b = f;
    }
    else
    {
	r = g = b = .5;
    }

    r = .5 * (-cos( r * M_PI) + 1.);
    g = .5 * (-cos( g * M_PI) + 1.);
    b = .5 * (-cos( b * M_PI) + 1.);


    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_ghost(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = 1. - (double)lr / 255.;
    g = 1. - (double)lg / 255.;
    b = 1. - (double)lb / 255.;

    r2 = cbrt(b) * g * g;
    g2 = cbrt(r) * b * b;
    b2 = cbrt(g) * r * r;

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_inv(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = 1. - (double)lr / 255.;
    g = 1. - (double)lg / 255.;
    b = 1. - (double)lb / 255.;

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_box(double x1, double x2)
{
    unsigned long lx, lr, lg, lb, l1, l2;
    int cx, cy;

    x2 = 768. - x2;

    cx = x1 / 256.;
    cy = x2 / 256.;

    l1 = (unsigned long)x1;
    l2 = (unsigned long)x2;

    l1 %= 0x100;
    l2 %= 0x100;

    lr = lg = lb = 0;

    if (cy == 0)
    {
	if (cx == 1)
	{
	    lr = l1;
	    lb = 0xff - l2;
	}
	else if (cx == 0)
	{
	    lr = l1;
	    lg = l2;
	    lb = .5*(l1+l2);
	}
	else if (cx == 2)
	{
	    lr = l1;
	    lg = .5*(l1+l2);
	    lb = l2;
	}
	else if (cx == 3)
	{
	    lr = .5*(l1+l2);
	    lg = l1;
	    lb = l2;
	}
    }
    else if (cy == 1)
    {
	if (cx == 0)
	{
	    lg = l2;
	    lb = 0xff - l1;
	}
	else if (cx == 1)
	{
	    lr = l1;
	    lg = l2;
	}
	else if (cx == 2)
	{
	    lr = 0xff;
	    lg = l2;
	    lb = l1;
	}
	else if (cx == 3)
	{
	    lr = 0xff - l1;
	    lg = l2;
	    lb = 0xff;
	}
    }
    else if (cy == 2)
    {
	if (cx == 1)
	{
	    lr = l1;
	    lg = 0xff;
	    lb = l2;
	}
	else if (cx == 0)
	{
	    lr = l2;
	    lg = l1;
	    lb = .5*(l1+l2);
	}
	else if (cx == 2)
	{
	    lr = l2;
	    lg = .5*(l1+l2);
	    lb = l1;
	}
	else if (cx == 3)
	{
	    lr = .5*(l1+l2);
	    lg = l2;
	    lb = l1;
	}
    }

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x1 = (double)lx;

    return x1;
}

double min_udf2(double x1, double x2, double x3)
{
    double v1, v2;

    v1 = udf2(x1, x2);
    v2 = udf2(x2, x3);

    return (v1 < v2) ? v1 : v2;
}

double udf3(double x1, double x2, double x3)
{
/*    return alien_sine_ex(x1, x2, x3, x2, x3, x2, x3);*/
/*    return color_ghost(x1);*/
    return alien_gs3(x1, x2, x3);
}


double udf2(double x1, double x2)
{
    /*
    double v1, v2;
    v1 = udf1(x1);
    v2 = udf1(x2);

    return (v1 > v2) ? v1 : v2;
    */

/*    return color_inv(x1);*/
    return color_box(x1, x2);
}

double udf1(double x)
{
/*    return alien_sine(x);*/
/*    return alien_sine_ex(x, 1.5, -.5*M_PI, 1.5, -.5*M_PI, 1.5, -.5*M_PI);*/
/*    return color_mul(x);*/
/*    return color_inv(x);*/
/*    return color_inv2(x);*/
/*    return alien_col_mul(x, 1.5, -.5*M_PI, 1.5, -.5*M_PI, 1.5, -.5*M_PI);*/
/*    return color_ghost(x);*/
/*    return color_inv3(x);*/
/*    return alien_inv(x);*/
/*    return color_heat(x);*/
/*    return color_angle(x);*/
/*    return color_complex(x);*/
/*    return color_hue(x);*/
/*    return color_hue2(x);*/
    return alien_gs(x);
/*    return partial_gs(x);*/
}

unsigned long udfi1(unsigned long x)
{
/*    return color_heati(x);*/
/*    return color_heati2(x);*/
/*    return color_comix(x);*/
    return alien_gsi(x);
/*    return alien_toon(x, 2, 3, 1);*/
/*    return alien_toongs(x, 2, 0, 1); */
/*    return alien_toongs(x, 2, 0, 1);*/ /* last */
/*    return color_invi(x);*/
/*    return alien_toongs2(x, 2, 5, 1, 2);*/
}

unsigned long udfi2(unsigned long x1, unsigned long x2)
{
/*    return edge_detect(x1, x2, 2500);*/
    return edge_detect_comix(x1, x2);
}

unsigned long udfi3(unsigned long x1, unsigned long x2, unsigned long x3)
{
/*    return edge_detect(x1, x2, x3);*/
/*    return smooth_gs(x1, x2, x3);*/
/*    return color_offset3(x1, x2, x3);*/
/*    return alien_gsi3(x1, x2, x3);*/
/*    return color_gsi3(x1, x2, x3);*/
/*    return alien_toon3(x1, x2, x3, 2, 3, 1);*/
/*    return alien_toon3(x1, x2, x3, 2, 0, 1);*/
    return color_invi3(x1, x2, x3);
}

unsigned long udfi6(unsigned long x1, unsigned long x2, unsigned long x3, unsigned long x4, unsigned long x5, unsigned long x6)
{
/*    return edge_detect(x1, x2, x3);*/
    unsigned long xt[6];
    xt[0] = x1;
    xt[1] = x2;
    xt[2] = x3;
    xt[3] = x4;
    xt[4] = x5;
    xt[5] = x6;
/*    return smooth_gs6(xt);*/
/*    return color_offset6(xt);*/
    return alien_gsi6(xt);
}
