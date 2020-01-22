/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/* 
 * Please fill in the following student struct 
 */
team_t team = {
    "e2310647",              /* Student ID */

    "Yasar Cahit Yildirim",     /* full name */
    "cahit.yildirim@metu.edu.tr",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};

#define MOVE_ONE_STEP_SOBEL dst[jxdi] =                       \
                                (-src[jxdi - dim - 1]     ) + \
                                ( src[jxdi - dim + 1]     ) + \
                                (-src[jxdi       - 1] << 1) + \
                                ( src[jxdi       + 1] << 1) + \
                                (-src[jxdi + dim - 1]     ) + \
                                ( src[jxdi + dim + 1]     );  \
                                jxdi++

char v1_sobel_descr[] = "Dot product: V1";
void v1_sobel(int dim, int *src, int *dst)
{
    int jxdi = 0;
    int dimdim = dim * dim;
    int lim = dim - 3;
    
    while(jxdi < dim)
    {
        dst[jxdi++] = 0;
        dst[--dimdim] = 0;
        dst[jxdi++] = 0;
        dst[--dimdim] = 0;
    }
    
    while(jxdi < dimdim)
    {
        dst[jxdi++] = 0;
        MOVE_ONE_STEP_SOBEL;

        lim += dim;
        while(jxdi < lim)
        {
            MOVE_ONE_STEP_SOBEL;
            MOVE_ONE_STEP_SOBEL;
        }
        
        MOVE_ONE_STEP_SOBEL;
        dst[jxdi++] = 0;
    }
}

char v2_sobel_descr[] = "Dot product: V2";
void v2_sobel(int dim,int *src, int *dst)
{
    int *dstl = dst;
    int *dimdim = dst + dim*dim;
    int *lim = dst + dim-1;
    int *src1 = src     ;
    int *srcl = src +dim;
    int *src2 = srcl+dim;

    while(dstl < dst+dim)
    {
        *(dstl      ) = 0;
        *(dimdim - 1) = 0;
        *(dstl   + 1) = 0;
        *(dimdim - 2) = 0;
        *(dstl   + 2) = 0;
        *(dimdim - 3) = 0;
        *(dstl   + 3) = 0;
        *(dimdim - 4) = 0;

        dstl += 4;
        dimdim -= 4;
    }

    while(dstl < dimdim)
    {
        *(dstl) = 0;
        dstl++;

        srcl++;
        src1++;
        src2++;
        
        lim += dim;
        while(dstl < lim)
        {
            *(dstl) =
                (-*(src1 - 1)     ) +
                ( *(src1 + 1)     ) +
                (-*(srcl - 1) << 1) +
                ( *(srcl + 1) << 1) +
                (-*(src2 - 1)     ) +
                ( *(src2 + 1)     );
            *(dstl + 1) =
                (-*(src1    )     ) +
                ( *(src1 + 2)     ) +
                (-*(srcl    ) << 1) +
                ( *(srcl + 2) << 1) +
                (-*(src2    )     ) +
                ( *(src2 + 2)     );
            dstl+=2;
            srcl+=2;
            src1+=2;
            src2+=2;
        }
        
        *(dstl) = 0;
        dstl++;

        srcl++;
        src1++;
        src2++;
    }
}

char v3_sobel_descr[] = "Dot product: V3";
void v3_sobel(int dim, int *src, int *dst)
{
    int *dstl = dst;
    int *dimdim = dst + dim*dim;
    int *lim = dst + dim-1;
    int *src1 = src     ;
    int *srcl = src +dim;
    int *src2 = srcl+dim;
    
    int *src3 = src2+dim;

    while(dstl < dst+dim)
    {
        *(dstl      ) = 0;
        *(dimdim - 1) = 0;
        *(dstl   + 1) = 0;
        *(dimdim - 2) = 0;
        *(dstl   + 2) = 0;
        *(dimdim - 3) = 0;
        *(dstl   + 3) = 0;
        *(dimdim - 4) = 0;

        dstl += 4;
        dimdim -= 4;
    }

    while(dstl < dimdim)
    {
        *(dstl) = 0;
        *(dstl+dim) = 0;
        dstl++;

        srcl++;
        src1++;
        src2++;
        src3++;
        
        lim += dim;
        while(dstl < lim)
        {
            *(dstl) =
                (-*(src1 - 1)     ) +
                ( *(src1 + 1)     ) +
                (-*(srcl - 1) << 1) +
                ( *(srcl + 1) << 1) +
                (-*(src2 - 1)     ) +
                ( *(src2 + 1)     );
            *(dstl + dim) =
                (-*(srcl - 1)     ) +
                ( *(srcl + 1)     ) +
                (-*(src2 - 1) << 1) +
                ( *(src2 + 1) << 1) +
                (-*(src3 - 1)     ) +
                ( *(src3 + 1)     );
            *(dstl + 1) =
                (-*(src1    )     ) +
                ( *(src1 + 2)     ) +
                (-*(srcl    ) << 1) +
                ( *(srcl + 2) << 1) +
                (-*(src2    )     ) +
                ( *(src2 + 2)     );
            *(dstl + dim + 1) =
                (-*(srcl    )     ) +
                ( *(srcl + 2)     ) +
                (-*(src2    ) << 1) +
                ( *(src2 + 2) << 1) +
                (-*(src3    )     ) +
                ( *(src3 + 2)     );
            dstl+=2;
            srcl+=2;
            src1+=2;
            src2+=2;
            src3+=2;
        }
        
        *(dstl) = 0;
        *(dstl+dim) = 0;
        dstl++;

        srcl++;
        src1++;
        src2++;
        src3++;

        dstl+=dim;
        srcl+=dim;
        src1+=dim;
        src2+=dim;
        src3+=dim;
    }
}

char v4_sobel_descr[] = "Dot product: V4";
void v4_sobel(int dim,int *src, int *dst)
{
    int *dstl = dst;
    int *dimdim = dst + dim*dim;
    int *lim = dst + dim-2;
    int *src1 = src     ;
    int *srcl = src +dim;
    int *src2 = srcl+dim;

    while(dstl < dst+dim)
    {
        *(dstl      ) = 0;
        *(dimdim - 1) = 0;
        *(dstl   + 1) = 0;
        *(dimdim - 2) = 0;
        *(dstl   + 2) = 0;
        *(dimdim - 3) = 0;
        *(dstl   + 3) = 0;
        *(dimdim - 4) = 0;

        dstl += 4;
        dimdim -= 4;
    }

    while(dstl < dimdim)
    {
        *(dstl) = 0;
        *(dstl + 1) =
            (-*(src1    )     ) +
            ( *(src1 + 2)     ) +
            (-*(srcl    ) << 1) +
            ( *(srcl + 2) << 1) +
            (-*(src2    )     ) +
            ( *(src2 + 2)     );

        dstl+=2;

        srcl+=2;
        src1+=2;
        src2+=2;
        
        lim += dim;
        while(dstl < lim)
        {
            *(dstl) =
                (-*(src1 - 1)     ) +
                ( *(src1 + 1)     ) +
                (-*(srcl - 1) << 1) +
                ( *(srcl + 1) << 1) +
                (-*(src2 - 1)     ) +
                ( *(src2 + 1)     );
            *(dstl + 1) =
                (-*(src1    )     ) +
                ( *(src1 + 2)     ) +
                (-*(srcl    ) << 1) +
                ( *(srcl + 2) << 1) +
                (-*(src2    )     ) +
                ( *(src2 + 2)     );
            *(dstl + 2) =
                (-*(src1 + 1)     ) +
                ( *(src1 + 3)     ) +
                (-*(srcl + 1) << 1) +
                ( *(srcl + 3) << 1) +
                (-*(src2 + 1)     ) +
                ( *(src2 + 3)     );
            *(dstl + 3) =
                (-*(src1 + 2)     ) +
                ( *(src1 + 4)     ) +
                (-*(srcl + 2) << 1) +
                ( *(srcl + 4) << 1) +
                (-*(src2 + 2)     ) +
                ( *(src2 + 4)     );
            
            dstl+=4;
            srcl+=4;
            src1+=4;
            src2+=4;
        }
        
        *(dstl) =
            (-*(src1 - 1)     ) +
            ( *(src1 + 1)     ) +
            (-*(srcl - 1) << 1) +
            ( *(srcl + 1) << 1) +
            (-*(src2 - 1)     ) +
            ( *(src2 + 1)     );
        *(dstl + 1) = 0;
        dstl+=2;

        srcl+=2;
        src1+=2;
        src2+=2;
    }
}


char sobel_descr[] = "Dot product: Current working version";
void sobel(int dim,int *src,int *dst) 
{
    v4_sobel(dim, src, dst);
}

void register_sobel_functions() {
    //add_sobel_function(&sobel, sobel_descr);   

    //add_sobel_function(&v1_sobel, v1_sobel_descr);
    //add_sobel_function(&v2_sobel, v2_sobel_descr);
    //add_sobel_function(&v3_sobel, v3_sobel_descr);
    //add_sobel_function(&v4_sobel, v4_sobel_descr);
}




char v1_mirror_descr[] = "Mirror: V1";
void v1_mirror(int dim,int *src,int *dst)
{
    int jxdi = 0;
    int jxdd1i;
    int lim = 0;

    while(jxdi < dim*dim)
    {
        lim   += dim;
        jxdd1i = jxdi + dim - 1;
        while(jxdi < lim)
        {
            dst[jxdi++] = src[jxdd1i--];
            dst[jxdi++] = src[jxdd1i--];
        }
    }
}

char v2_mirror_descr[] = "Mirror: V2";
void v2_mirror(int dim,int *src,int *dst)
{
    int jxdi = 0;
    int jxdd1i;
    int lim = 0;

    while(jxdi < dim*dim)
    {
        lim   += dim;
        jxdd1i = jxdi + dim;
        while(jxdi < lim)
        {
            dst[jxdi++] = src[--jxdd1i];
            dst[jxdi++] = src[--jxdd1i];
        }
    }
}

char v3_mirror_descr[] = "Mirror: V3";
void v3_mirror(int dim,int *src,int *dst)
{
    int *lim = dst;
    int *srcl;
    int *dstl = dst;
    int *fin = dst + dim*dim;

    while(dstl < fin)
    {
        lim += dim;
        srcl = src + dim;
        while(dstl < lim)
        {
            *(dstl    ) = *(srcl - 1);
            *(dstl + 1) = *(srcl - 2);
            *(dstl + 2) = *(srcl - 3);
            *(dstl + 3) = *(srcl - 4);
            *(dstl + 4) = *(srcl - 5);
            *(dstl + 5) = *(srcl - 6);
            *(dstl + 6) = *(srcl - 7);
            *(dstl + 7) = *(srcl - 8);
            *(dstl + 8) = *(srcl - 9);
            *(dstl + 9) = *(srcl - 10);
            *(dstl + 10) = *(srcl - 11);
            *(dstl + 11) = *(srcl - 12);
            *(dstl + 12) = *(srcl - 13);
            *(dstl + 13) = *(srcl - 14);
            *(dstl + 14) = *(srcl - 15);
            *(dstl + 15) = *(srcl - 16);
            *(dstl + 16) = *(srcl - 17);
            *(dstl + 17) = *(srcl - 18);
            *(dstl + 18) = *(srcl - 19);
            *(dstl + 19) = *(srcl - 20);
            *(dstl + 20) = *(srcl - 21);
            *(dstl + 21) = *(srcl - 22);
            *(dstl + 22) = *(srcl - 23);
            *(dstl + 23) = *(srcl - 24);
            *(dstl + 24) = *(srcl - 25);
            *(dstl + 25) = *(srcl - 26);
            *(dstl + 26) = *(srcl - 27);
            *(dstl + 27) = *(srcl - 28);
            *(dstl + 28) = *(srcl - 29);
            *(dstl + 29) = *(srcl - 30);
            *(dstl + 30) = *(srcl - 31);
            *(dstl + 31) = *(srcl - 32);

            dstl += 32;
            srcl -= 32;
        }
        src += dim;
    }
}

char mirror_descr[] = "Mirror: Current working version";
void mirror(int dim,int *src,int *dst) 
{
    v3_mirror(dim, src, dst);
}

void register_mirror_functions() {
    //add_mirror_function(&mirror, mirror_descr);   

    //add_mirror_function(&v1_mirror, v1_mirror_descr);
    //add_mirror_function(&v2_mirror, v2_mirror_descr);
    //add_mirror_function(&v3_mirror, v3_mirror_descr);
}


