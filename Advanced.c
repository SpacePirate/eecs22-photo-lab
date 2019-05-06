/**************************************************************************************************/
/* Advanced.c: program file for PhotoLab Advanced DIP module                                      */
/* author: Deukkwon Yoon                                                                          */
/**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "FileIO.h"
#include "Advanced.h"
#include "Image.h"
#include "DIPs.h"

/* Posterize the image */
IMAGE *Posterize(IMAGE *image, unsigned int pbits)
{
    /*
    0x0F is a hexadecimal representation of decimal intger 15
    In binary, 00001111.
    c = c & 0x0F extracts lowest 4 bits from char c to 0
    c = c | 0x0F sets lowest 4 bits of char c to 1
    */
    
    int x, y;
    unsigned char r, g, b;
    int bits_to_0, bits_to_1;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);

    bits_to_0 = (1 << 8) - (1 << pbits); /* 1's nth bit 0's */
    bits_to_1 = (1 << (pbits - 1)) - 1;  /* 0's nth -1 bit 1's */

    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
        {
            /* Get RGB from the original image */
            r = GetPixelR(image, x, y);
            g = GetPixelG(image, x, y);
            b = GetPixelB(image, x, y);
        
            /* Assign values to the template */
            SetPixelR(temp, x, y, ((r & bits_to_0) | bits_to_1));
            SetPixelG(temp, x, y, ((g & bits_to_0) | bits_to_1));
            SetPixelB(temp, x, y, ((b & bits_to_0) | bits_to_1));
        }    
    }
    return temp;
}

/* Create light shape and write on image */
void MakeLight(int lightwidth, unsigned int w, int x, int y, int color,
               unsigned char *C)
{
    int mid = (lightwidth/2) + 1;
    int i;
    
    for( i = 0; i < lightwidth; i++ )
    {
        /* Negative slope diagonal */
        *(C + (x - mid + 1 + i) + ((y + mid - 1 - i) * w)) = color;
        /* Positive slope diagonal */
        *(C + (x - mid + 1 + i) + ((y - mid + 1 + i) * w)) = color;       
        /* Horizontal line */
        *(C + (x - mid + 1 + i) + (y * w)) = color;       
        /* Vertical line */
        *(C + x + ((y - mid + 1 + i) * w)) = color;
    }
}

/* Fill christmas lights to image */
IMAGE *FillLight(int number, int lightwidth, IMAGE *image)
{
    int x, y, i;
    int rand_x, rand_y;
    int rand_num, mid;
    unsigned char r, g, b;
    unsigned char c[] = {0, 0, 0};
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);
    
    /* initialize the random number generator with the current time */
    srand(time(NULL));
    
    /* Copy the image */
    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
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
    
    /* Assume user inputs valid light width. However, all non valid values will 
       be regarded to be the smallest dimension if value is too large and value 
       of 1 if the value is too small. */
    /* To make the light symmetric, length should always be an odd number */
    /* If value is too large*/
    if (lightwidth > w || lightwidth > h)
    {
        /* Use the smaller dimension as the maximum lightwidth */
        /* Determine even or odd */
        if (h > w)
            {lightwidth = (w % 2 != 0) ? w : w - 1;}
        else
            {lightwidth = (h % 2 != 0) ? h : h - 1;}
    }
    
    /* If value is too small */
    else if (lightwidth < 1) { lightwidth = 1; }
    
    /* Always make an odd number */
    else if (lightwidth%2 == 0)
    { lightwidth--; }
    
    /* Find the mid point (Axis of symmetry) */
    mid = (lightwidth/2) + 1;
    
    /* Create each star randomly on the image */
    for (i = 0; i < number; i ++)
    {
        /* Replace on of RGB value to 255 randomly */
        c[0] = c[1] = c[2] = 0; /* Reset color values */
        rand_num = rand() % 3;
        c[rand_num] = 255;
    
        /* Random number from lightwidth/2 to WIDTH - (lightwidth/2) */
        rand_x = (rand() % (w - (2*(mid - 1)))) + mid - 1;
        rand_y = (rand() % (h - (2*(mid - 1)))) + mid - 1;
        
        /* Create stars on image */
        MakeLight(lightwidth, w, rand_x, rand_y, c[0], temp->R);
        MakeLight(lightwidth, w, rand_x, rand_y, c[1], temp->G);
        MakeLight(lightwidth, w, rand_x, rand_y, c[2], temp->B);        
    }
    return temp;
}

/* Overlays an image onto the original image */
IMAGE *Overlay(char fname[SLEN], IMAGE *image, int x_offset, int y_offset)
{
    int x, y;
    int x_off, y_off;
    unsigned char r, g, b;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);
    
    /* Image object for the overlay image */
    IMAGE *over;
    int r_over, g_over, b_over;
    over = ReadImage(fname);
    
    /* Copy the image */
    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
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
    
    /* Overlay image on the copy */
    for( y = 0; y < over->Height; y++ )
    {
        for( x = 0; x < over->Width; x++ )
        {
            r_over = GetPixelR(over, x, y);
            g_over = GetPixelG(over, x, y);
            b_over = GetPixelB(over, x, y);
            
            /* Overlay pixels if conditions are met...*/
            /* Pixel is part of the image (not a background) */
            if (r_over < 250 && g_over < 250 && b_over < 250)
            {
                x_off = x + x_offset;
                y_off = y + y_offset;

                /* Overlay pixel is within bounds of the image */
                if (((x_off >= 0) && (x_off < w)) &&
                    ((y_off >= 0) && (y_off < h)))
                {
                    /* Assign values to the template */
                    SetPixelR(temp, x_off, y_off, r_over);
                    SetPixelG(temp, x_off, y_off, g_over);
                    SetPixelB(temp, x_off, y_off, b_over);
                }
            }
        }
    }
    DeleteImage(over);
    return temp;
}

/* Perform Cut Paste operations on the image*/
IMAGE *CutPaste(IMAGE *image, unsigned int startX, unsigned int startY, 
                unsigned int x_width, unsigned int y_width,
                unsigned int pasteNumber)
{
    int x, y, i;
    int x_val, y_val;
    unsigned char r, g, b;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w, h);
    
    /* Copy the image */
    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
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

    /* Get paste coordinates, user waits after each cut and paste */
    for (i = 0; i < pasteNumber; i++)
    {   
        while(1)
        {
            #ifndef DEBUG
            printf("Please input start x and y coordinate for Paste no. %d : ",
                    i + 1);
            scanf("%d", &x_val);
            scanf("%d", &y_val);
            #endif

            #ifdef DEBUG
            if (i == 0)
            {
                x_val = 341;
                y_val = 19;
            }
            else if (i == 1)
            {
                x_val = 315;
                y_val = 18;
            }
            else if (i == 2)
            {
                x_val = 288;
                y_val = 16;
            }
            else
            {
                x_val = 0;
                y_val = 0;
            }
            #endif
            
            if (x_val <= w && y_val <= h)
            {   
                for( y = 0; y < y_width; y++ )
                {
                    for( x = 0; x < x_width; x++ )
                    {
                        /* Check that pixels are within bounds of the image */
                        if ((x_val + x < w) && (y_val + y < h) &&
                            (startX + x < w) && (startY + y < h))
                        {
                            /* Get RGB from the original image */
                            r = GetPixelR(image, startX + x, startY + y);
                            g = GetPixelG(image, startX + x, startY + y);
                            b = GetPixelB(image, startX + x, startY + y);
        
                            /* Assign values to the template */
                            SetPixelR(temp, x_val + x, y_val + y, r);
                            SetPixelG(temp, x_val + x, y_val + y, g);
                            SetPixelB(temp, x_val + x, y_val + y, b);
                        }
                    }   
                }
                break;
            }
            else
            { printf("Invalid start coordinates. Try Again.\n"); }
        }
    }
    return temp;
}

/* Resize */
IMAGE *Resize(unsigned int percentage, IMAGE *image)
{
    unsigned int x, y;
    unsigned int x_new, y_new;
    unsigned char r, g, b;
    unsigned int avg_r, avg_g, avg_b;
    
    double p = (percentage / 100.00);
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    unsigned int w_new = w * p;
    unsigned int h_new = h * p;
    
    #ifndef DEBUG
    printf("New Size is: %d x %d\n", w_new, h_new);
    #endif
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w_new, h_new);

    assert(percentage <= 500 && percentage >= 1);
    
    /* Enlarge image or same size */
    if (percentage >= 100)
    {
        for( y = 0; y < h; y++ )
        {
            for( x = 0; x < w; x++ )
            {
                /* Get RGB from the original image */
                r = GetPixelR(image, x, y);
                g = GetPixelG(image, x, y);
                b = GetPixelB(image, x, y);
                
                /* Use same RGB values for newly created pixels */
                for( y_new = y*p; y_new < (y+1)*p; y_new++ )
                {
                    for( x_new = x*p; x_new < (x+1)*p; x_new++ )
                    {
                        /* Assign values to the template */
                        SetPixelR(temp, x_new, y_new, r);
                        SetPixelG(temp, x_new, y_new, g);
                        SetPixelB(temp, x_new, y_new, b);
                    }
                }
            }
        }
    }
    
    /* Shrink image */
    else
    {
        for( y_new = 0; y_new < h_new; y_new++ )
        {
            for( x_new = 0; x_new < w_new; x_new++ )
            {
                /* Get average of bigger area in to smaller area */
                int i = 0;
                avg_r = 0;
                avg_g = 0;
                avg_b = 0;

                /* Use same RGB values for newly created pixels */
                for( y = y_new/p; y < (y_new+1)/p; y++ )
                {
                    for( x = x_new/p; x < (x_new+1)/p; x++ )
                    {
                        /* Get RGB from the original image */
                        avg_r += GetPixelR(image, x, y);
                        avg_g += GetPixelG(image, x, y);
                        avg_b += GetPixelB(image, x, y);
                        i++;
                    }
                }
                /* Assign average values to the template */
                SetPixelR(temp, x_new, y_new, avg_r/i);
                SetPixelG(temp, x_new, y_new, avg_g/i);
                SetPixelB(temp, x_new, y_new, avg_b/i);
            }
        }
    }
    
    return temp;
}

/*Rotate 90 degrees clockwise*/
IMAGE *Rotate(IMAGE *image)
{
    int x, y;
    unsigned char r, g, b;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    unsigned int w_new = h;
    unsigned int h_new = w;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w_new, h_new);

    for( y = 0; y < h_new; y++ )
    {
        for( x = 0; x < w_new; x++ )
        {
            /* Get RGB from the original image */
            r = GetPixelR(image, y, w_new - x);
            g = GetPixelG(image, y, w_new - x);
            b = GetPixelB(image, y, w_new - x);
        
            /* Assign values to the template */
            SetPixelR(temp, x, y, r);
            SetPixelG(temp, x, y, g);
            SetPixelB(temp, x, y, b);
        }    
    }
    return temp;
}

/*Mandelbrot*/
IMAGE *Mandelbrot(unsigned int W, unsigned int H, unsigned int max_iteration)
{
    int x, y;
    double x0, y0, x1, y1, num;
    int i;
    unsigned char r, g, b;
    int color;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(W, H);
    
    /* RGB color palette for Mandelbrot image */
    const unsigned char palette[MAX_COLOR][3] = {
    /* r g b*/
    { 0, 0, 0 },       /* 0, black */
    { 127, 0, 0 },     /* 1, brown */
    { 255, 0, 0 },     /* 2, red */
    { 255, 127, 0 },   /* 3, orange */
    { 255, 255, 0 },   /* 4, yellow */
    { 127, 255, 0 },   /* 5, light green */
    { 0, 255, 0 },     /* 6, green */
    { 0, 255, 127 },   /* 7, blue green */
    { 0, 255, 255 },   /* 8, turquoise */
    { 127, 255, 255 }, /* 9, light blue */
    { 255, 255, 255 }, /* 10, white */
    { 255, 127, 255 }, /* 11, pink */
    { 255, 0, 255 },   /* 12, light pink */
    { 127, 0, 255 },   /* 13, purple */
    { 0, 0, 255 },     /* 14, blue */
    { 0, 0, 127 }      /* 15, dark blue */
    };
    
    int count = H*W;
    for( y = 0; y < H; y++ )
    {
        for( x = 0; x < W; x++ )
        {
            x0 = (((((double)y) / ((double)W)) * 3.5) - 2.5);
            y0 = (((((double)x) / ((double)H)) * 2.0) - 1.0);
            x1 = 0.0;
            y1 = 0.0;
            i = 0;
            while( (((x1 * x1) + (y1 * y1)) < (2 * 2)) && i < max_iteration)
            {
                num = (x1 * x1) - (y1 * y1) + x0;
                y1 = 2 * x1 * y1 + y0;
                x1 = num;           
                i++;
            }
            color = i % MAX_COLOR;
            #ifndef DEBUG
            printf("Color: %d\n", color);
            #endif
            r = palette[color][0];
            g = palette[color][1];
            b = palette[color][2];
            
            /* Assign values to the template */
            SetPixelR(temp, x, y, r);
            SetPixelG(temp, x, y, g);
            SetPixelB(temp, x, y, b);
            count--;
            #ifndef DEBUG
            printf("%d Iterations left....\n", count);
            #endif
        }
    }
    return temp;
}

/* BONUS : Add outer border */
IMAGE *AddOuterBorder(IMAGE *image, char color[SLEN], int border_width)
{
    int x, y;
    unsigned char r, g, b;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    unsigned int w_new = w + border_width;
    unsigned int h_new = h + border_width;
    
    /* Create template for pre-border image */
    IMAGE *temp;
    temp = CreateImage(w_new, h_new);

    /* Create template for post-border image */
    IMAGE *temp2;
    
    /* Make a copy of image */
    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
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
    
    temp2 = AddBorder(temp, color, border_width);
    DeleteImage(temp);
    
    return temp2;
}
