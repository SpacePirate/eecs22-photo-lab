/**************************************************************************************************/
/* DIPs.c: program file for PhotoLab DIP module                                                   */
/* author: Deukkwon Yoon                                                                          */
/**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Image.h"
#include "DIPs.h"
#include "FileIO.h"
#include "Constants.h"

/* change color image to black & white */
IMAGE *BlackNWhite(IMAGE *image)
{
    int x, y;
    unsigned char r, g, b;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);
    
    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
        {
            /* Get RGB from the original image */
            r = GetPixelR(image, x, y);
            g = GetPixelG(image, x, y);
            b = GetPixelB(image, x, y);
        
            /* Assign values to the template */
            SetPixelR(temp, x, y,(r + g + b)/3);
            SetPixelG(temp, x, y,(r + g + b)/3);
            SetPixelB(temp, x, y,(r + g + b)/3);
        }
    }
    return temp;
}

/* flip image vertically */
IMAGE *VFlip(IMAGE *image)
{
    int x, y;
    unsigned char r, g, b;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);
    
    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
        {
            /* Get RGB from the original image */
            r = GetPixelR(image, x, h - y);
            g = GetPixelG(image, x, h - y);
            b = GetPixelB(image, x, h - y);
        
            /* Assign values to the template */
            SetPixelR(temp, x, y, r);
            SetPixelG(temp, x, y, g);
            SetPixelB(temp, x, y, b);
        }
    }
    return temp;
}         

/* mirror image horizontally */
IMAGE *HMirror(IMAGE *image)
{
    int x, y;
    unsigned char r, g, b;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    int mid = ((w*5)+5)/10;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);

    for( y = 0; y < h; y++ )
    {
        /* map right side onto left side */
        for( x = 0; x < mid; x++ )
        {
            /* Get RGB from the original image */
            r = GetPixelR(image, w - x, y);
            g = GetPixelG(image, w - x, y);
            b = GetPixelB(image, w - x, y);
        
            /* Assign values to the template */
            SetPixelR(temp, x, y, r);
            SetPixelG(temp, x, y, g);
            SetPixelB(temp, x, y, b);
        }
        /* map right side onto right side*/
        for( x = mid; x < w; x++)
        {
            /* Get RGB from the original image */
            r = GetPixelR(image, x, y);
            g = GetPixelG(image, x, y);
            b = GetPixelB(image, x, y);
        
            /* Assign values to the template */
            SetPixelR(temp, x, y, r);
            SetPixelG(temp, x, y, g);
            SetPixelB(temp, x, y, b);
        }
    }
    return temp;
}   
          
/* replace color */
unsigned char ReColor(unsigned char color, int target,
                      int threshold, int replace)
{
    if (target - threshold < color && color < target + threshold)
    {
        color = replace;
    }

    return color;
}
            
/* color filter */
IMAGE *ColorFilter(IMAGE *image, int target_r, int target_g, int target_b,
                   int threshold, int replace_r, int replace_g, int replace_b)
{
    int x, y;
    unsigned char r, g, b;
    unsigned char rx, gx, bx;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);
    
    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
        {
            /* Get RGB from the original image */
            r = GetPixelR(image, x, y);
            g = GetPixelG(image, x, y);
            b = GetPixelB(image, x, y);

            /* Get adjusted color values */
            rx = ReColor(r, target_r, threshold, replace_r);
            gx = ReColor(g, target_g, threshold, replace_g);
            bx = ReColor(b, target_b, threshold, replace_b);
            
            if (rx != r && gx != g && bx != b)
            {
                /* Assign values to the template */
                SetPixelR(temp, x, y, rx);
                SetPixelG(temp, x, y, gx);
                SetPixelB(temp, x, y, bx);
            }
            else
            {
                /* Assign values to the template */
                SetPixelR(temp, x, y, r);
                SetPixelG(temp, x, y, g);
                SetPixelB(temp, x, y, b);
            }
        }    
    }
    return temp;
}

int EdgeVal(unsigned char *M, int x, int y, int w)
{
    unsigned int A, B, C, D, E, F, G, H, I;
    int val;
    /* Pixel Orientation */
    /* X X X X X */
    /* X A B C X */
    /* X D E F X */
    /* X G H I X */
    /* X X X X X */
    
    /* sum neighboring pixels, assume border is not included */
    A = *(M + (x - 1) + ((y - 1) * w));
    B = *(M + x + ((y - 1) * w));
    C = *(M + (x + 1) + ((y - 1) * w));
    D = *(M + (x - 1) + (y * w));
    E = *(M + x + (y * w));
    F = *(M + (x + 1) + (y * w));
    G = *(M + (x - 1) + ((y + 1) * w));
    H = *(M + x + ((y + 1) * w));
    I = *(M + (x + 1) + ((y + 1) * w));
    val = 8*E - A - B - C - D - F - G - H - I;
    
    return val;
}

/* edge detection */
IMAGE *Edge(IMAGE *image)
{
    int x, y;
    unsigned char r, g, b;
    int re, ge, be;
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);
    
    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
        {
            /* Disregard borders for edge filter */
            if (y < 1 || x < 1 || y > h - 1 || x > w - 1)
            {
                /* Get RGB from the original image */
                r = GetPixelR(image, x, y);
                g = GetPixelG(image, x, y);
                b = GetPixelB(image, x, y);
            
                /* Assign values to the template */
                SetPixelR(temp, x, y, r);
                SetPixelG(temp, x, y, g);
                SetPixelB(temp, x, y, b);
            }
            /* find pixel value */
            else
            {
                re = EdgeVal(image->R, x, y, w);
                ge = EdgeVal(image->G, x, y, w);
                be = EdgeVal(image->B, x, y, w);
               
                if (re > 255)
                    { re = 255; }
                else if (re < 0)
                    { re = 0; }
                
                if (ge > 255)
                    { ge = 255; }
                else if (ge < 0)
                    { ge = 0; }
                
                if (be > 255)
                    { be = 255; }
                else if (be < 0)
                    { be = 0; }
                
                SetPixelR(temp, x, y, re);
                SetPixelG(temp, x, y, ge);
                SetPixelB(temp, x, y, be);
            }
        }
    }

    return temp;
}

/* shuffles the image */
IMAGE *Shuffle(IMAGE *image)
{
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    int i, j, k, x;
    int unique, moved;
    int b_num = SHUFF_HEIGHT_DIV * SHUFF_WIDTH_DIV;
    int swapped[b_num];
    int h1, w1, h2, w2;
    unsigned char r, g, b;
    int BH = h/SHUFF_HEIGHT_DIV;
    int BW = w/SHUFF_WIDTH_DIV;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);
    
    /* divide image into 16 blocks represented by their start indices scale */
    /* each block is BW x BH */
    int blocks[][2] = {{0, 0},{1, 0},{2, 0},{3, 0},
                       {0, 1},{1, 1},{2, 1},{3, 1},
                       {0, 2},{1, 2},{2, 2},{3, 2},
                       {0, 3},{1, 3},{2, 3},{3, 3}};

    /* initialize the random number generator with the current time */
    srand(time(NULL));
    
    /* simulate swapping every b_num blocks */
    for (i = 0; i < b_num; i++)
    {
        do
        {
            /* assume that the number is unique*/
            unique = 1;
            
            /* assume that the block is not moved */
            moved = 0;
            
            /* randomly get a unique blocks to be swapped*/
            /* random number range from 0 to 15 */
            x = rand() % 16;
            
            /* check that block has moved */
            if (x != i) {moved = 1;}
            
            /* check that number is unique */
            for (j = 0; j < i; j++)
            {
                if (swapped[j] == x)
                {
                    /* value is not unique stop for loop */
                    /* run do-while loop again until */
                    /* unique value is found */
                    unique = 0;
                    break;
                }
            }
        } while (moved == 0 || unique == 0);
        
        swapped[i] = x;
    }
    
    /* blocks are now shuffled, map the values to shuffled blocks */
    for (i = 0; i < b_num; i++)
    {
    
        /* find shuffled block's start and end indices*/
        h1 = blocks[i][1] * BH;
        w1 = blocks[i][0] * BW;
        h2 = blocks[swapped[i]][1] * BH;
        w2 = blocks[swapped[i]][0] * BW;
        
        for (j = 0; j < BH; j++)
        {
            for (k = 0; k < BW; k++)
            {
                /* Get RGB from the original image */
                r = GetPixelR(image, w1 + k, h1 + j);
                g = GetPixelG(image, w1 + k, h1 + j);
                b = GetPixelB(image, w1 + k, h1 + j);
            
                /* map shuffled blocks */
                /* Assign values to the template */
                SetPixelR(temp, w2 + k, h2 + j, r);
                SetPixelG(temp, w2 + k, h2 + j, g);
                SetPixelB(temp, w2 + k, h2 + j, b);
            }
        }
    }
    return temp;
}

/* add border */
IMAGE *AddBorder(IMAGE *image, char color[SLEN], int border_width)
{
    int x, y;
    unsigned char r, g, b;
    unsigned char rval, gval, bval;
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);
    
    /* Define border colors */
    if (strcmp(color, "white") == 0)
    {
        rval = 255;
        gval = 255;
        bval = 255;
    }
    else if (strcmp(color, "red") == 0)
    {
        rval = 255;
        gval = 0;
        bval = 0;
    }
    else if (strcmp(color, "green") == 0)
    {
        rval = 0;
        gval = 255;
        bval = 0;
    }
    else if (strcmp(color, "blue") == 0)
    {
        rval = 0;
        gval = 0;
        bval = 255;
    }
    else if (strcmp(color, "yellow") == 0)
    {
        rval = 255;
        gval = 255;
        bval = 0;
    }
    else if (strcmp(color, "cyan") == 0)
    {
        rval = 0;
        gval = 255;
        bval = 255;
    }
    else if (strcmp(color, "pink") == 0)
    {
        rval = 255;
        gval = 0;
        bval = 128;
    }
    else if (strcmp(color, "orange") == 0)
    {
        rval = 255;
        gval = 128;
        bval = 0;
    }
    /* Although it is assumed that the user will always input correct values, */
    /* other values will be regarded as 'black' */
    else
    {
        rval = 0;
        gval = 0;
        bval = 0;
    }   

    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
        {
            /* replace borders */
            if (y < border_width || x < border_width ||
                y >= h - border_width ||  x >= w - border_width)
            {
                /* Assign values to the template */
                SetPixelR(temp, x, y, rval);
                SetPixelG(temp, x, y, gval);
                SetPixelB(temp, x, y, bval);
                
            }
            else
            {
                /* Get RGB from the original image */
                r = GetPixelR(image, x, y);
                g = GetPixelG(image, x, y);
                b = GetPixelB(image, x, y);
        
                /* Assign values to the template */
                SetPixelR(temp, x, y, r);
                SetPixelG(temp, x, y, g);
                SetPixelB(temp, x, y, b);             
            }
        }
    }
    return temp;
}

/* age the image to make it look like old picture */
IMAGE *Aging(IMAGE *image)
{
    int x, y;
    unsigned char r, g, b;
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);

    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
        {
            /* Get RGB from the original image */
            r = GetPixelR(image, x, y);
            g = GetPixelG(image, x, y);
            b = GetPixelB(image, x, y);
        
            /* Assign values to the template */
            SetPixelR(temp, x, y, (unsigned char) (b * 1.6));
            SetPixelG(temp, x, y, (unsigned char) (b * 1.6));
            SetPixelB(temp, x, y, (r + g + b)/5);    
        }
    }
    return temp;
}