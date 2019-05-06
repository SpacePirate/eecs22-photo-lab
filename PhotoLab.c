/**************************************************************************************************/
/* PhotoLab2.c: EECS 22 assignment 3                                                              */
/* author: Deukkwon Yoon                                                                          */
/* created: 2014-10-26                                                                            */
/* last modified: 2014-11-11                                                                      */
/* ver. 2                                                                                         */
/**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "FileIO.h"
#include "Image.h"
#include "DIPs.h"
#include "Advanced.h"
#include "Constants.h"

/* print a menu */
void PrintMenu();

/* Test all functions */
int AutoTest();

int main()
{
    /* Define pointer image. Pointer to IMAGE type */
    IMAGE *image;
    
    /* Define image template. */
    IMAGE *temp;
    

    int FileNotRead = 1; 
    int choice;
    
    /* Used to load and save an image */
    char fname[SLEN];
    
    /* Parameters used for color filter */
    char color[SLEN];
    int target_r, target_g, target_b;
    int replace_r, replace_g, replace_b;
    int threshold;

    /* Used to put border on an image */
    int border_width;    
    
    /* Used to posterize an image */
    int pbits;
    
    /* Used to draw lights on an image */
    int lightnumber, lightwidth;
    
    /* Used to load an overlay image */
    char over_img[SLEN];
    int x_off, y_off;
    
    /* Used to cut and paste image */
    unsigned int startX, startY;
    unsigned int x_width, y_width;
    unsigned int pasteNumber;

    /* Used to resize image */
    unsigned int percentage;
    
    /* Used to create Mandelbrot image */
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_iter;
    
    #ifndef DEBUG
    while (choice != 19)
    {

        /* Print menu and receive user input */
        PrintMenu();
        scanf("%d", &choice);
    
        /* Process user request */
        switch (choice) {
            /* Don't use user interface during the debug mode*/

            /* 1: Load a PPM image */
            case 1:
            {
                printf("Please input the file name to load: ");
                scanf("%79s", fname);
                image = ReadImage(fname);
                if (image != NULL) { FileNotRead = 0; }
                break;
            }
            
            /* 2: Save an image in PPM and JPEG format */ 
            case 2:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    printf("Please input the file name to save: ");
                    scanf("%79s", fname);
                    SaveImage(image, fname);
                }
                break;
            }
            
            /* 3: Change a color image to Black & White */
            case 3:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                { 
                    temp = BlackNWhite(image); 
                    printf("\"Black & White\" operation is done!\n");
                    SaveImage(temp, "bw");
                }
                break;
            }
            
            /* 4: Flip an image vertically */
            case 4:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    temp = VFlip(image);
                    printf("\"VFlip\" operation is done!\n");
                    SaveImage(temp, "vflip");
                }
                break;
            }
            
            /* 5: Mirror an image horizontally */
            case 5:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    temp = HMirror(image);
                    printf("\"HMirror\" operation is done!\n");
                    SaveImage(temp, "hmirror");
                }
                break;
            }
            
            /* 6: Color filter an image */
            case 6:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                { 
                /* Program expects correct inputs from the user */
                /* Color values must range from 0 to 255 */
                printf("Enter Red component for the target color: ");
                scanf("%d", &target_r);
                printf("Enter Green component for the target color: ");
                scanf("%d", &target_g);
                printf("Enter Blue component for the target color: ");
                scanf("%d", &target_b);
                printf("Enter threshold for the color difference: ");
                scanf("%d", &threshold);
                printf("Enter value for Red component in the target color: ");
                scanf("%d", &replace_r);
                printf("Enter value for Green component in the target color: ");
                scanf("%d", &replace_g);
                printf("Enter value for Blue component in the target color: ");
                scanf("%d", &replace_b);
                temp = ColorFilter(image, target_r, target_g, target_b,
                                   threshold, replace_r, replace_g, replace_b);
                printf("\"Color Filter\" operation is done!\n");
                SaveImage(temp, "colorfilter");
                }
                break;
            }
            
            /* 7: Sketch the edge of an image */
            case 7:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                   temp = Edge(image);
                   printf("\"Edge\" operation is done!\n");
                   SaveImage(temp, "edge");
                }
                break;
            }
            
            /* 8: Shuffle an image */
            case 8:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    temp = Shuffle(image);
                    printf("\"Shuffle\" operation is done!\n");
                    SaveImage(temp, "shuffle");
                }
                break;
            }
            
            /* 9: Add Border to an image */
            case 9:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    printf("Enter boarder width: ");
                    scanf("%d", &border_width);
                    printf("Available border colors : black, white, red, "
                           "green, blue, yellow, cyan, pink, orange\n"
                           "Select border color from the options: ");
                    scanf("%79s", color);
                    
                    temp = AddBorder(image, color, border_width);
                    printf("\"Border\" operation is done!\n");
                    SaveImage(temp, "border");
                }
                break;
            }
            
            /* 10: Posterize the image */
            case 10:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                { 
                    printf("Enter the number of posterization bits (1 to 8): ");
                    scanf("%d", &pbits);
                    if (pbits > 0 && pbits <= 8)
                    {
                        temp = Posterize(image, pbits);   
                        printf("\"Posterize\" operation is done!");
                        SaveImage(temp, "poster");
                    }
                    else
                    { printf("Invalid bit value. Try Again.\n");}
                }
                break;
            }
            
            /* 11: Fill Lights to an image */
            case 11:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                { 
                    printf("Please input number of lights: ");
                    scanf("%d", &lightnumber);
                    printf("Please input the width of each light: ");
                    scanf("%d", &lightwidth);
                    temp = FillLight(lightnumber, lightwidth, image);
                    printf("\"FillLight\" operation is done!\n");
                    SaveImage(temp, "light");
                }
                break;
            }
            
            /* 12: Overlay an image */
            case 12:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    printf("Please input the file name for the second image: ");
                    scanf("%79s", over_img);
    
                    printf("Please input x coordinate of the overlay image: ");
                    scanf("%d", &x_off);

                    printf("Please input y coordinate of the overlay image: ");
                    scanf("%d", &y_off);

                    temp = Overlay(over_img, image, x_off, y_off);
                    printf("\"Image Overlay\" operation is done!\n");
                    
                    char str1[SLEN];
                    char str2[SLEN];
                    strcpy(str1, "overlay_");
                    strcpy(str2, over_img);
                    strcat(str1, str2);
                    SaveImage(temp, str1);
                }
                break;
            }
            
            /* 13: Cut and Paste operation on image */
            case 13:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    printf("Give the start X coordinate for cutting: ");
                    scanf("%d", &startX);
    
                    printf("Give the start Y coordinate for cutting: ");
                    scanf("%d", &startY);

                    printf("Give the width for cutting: ");
                    scanf("%d", &x_width);
                    
                    printf("Give the height for cutting: ");
                    scanf("%d", &y_width);                   
                    
                    printf("Give the number of paste locations: ");
                    scanf("%d", &pasteNumber);
                    
                    temp = CutPaste(image, startX, startY,
                                    x_width, y_width, pasteNumber);
                    printf("\"Cut Paste\" operation is done!\n");
                    SaveImage(temp, "cutPaste");
                }
                break;
            }
            
            /* 14: Resize image */
            case 14:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    printf("Please input the resizing percentage "
                           "(integer between 1~500): ");
                    scanf("%d", &percentage);
                    
                    temp = Resize(percentage, image);
                    printf("\"Resize  the image\" operation is done!\n");
                    if (percentage >= 100)
                    { SaveImage(temp, "bigresize");}
                    else
                    { SaveImage(temp, "smallresize");}
                }
                break;
            }
            
            /*Rotate 90 degrees clockwise*/
            case 15:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    temp = Rotate(image);
                    printf("\"Rotate\" operation is done!\n");
                    SaveImage(temp, "rotate");
                }
                break;
            }
            
            /* Create Mandelbrot image */
            case 16:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    printf("Please input the width of the mandelbrot image: ");
                    scanf("%d", &m_width);
                    printf("Please input the height of the mandelbrot image: ");
                    scanf("%d", &m_height);
                    printf("Please input the max iteration for the mandelbrot "
                           "calculation: ");
                    scanf("%d", &m_iter);

                    temp = Mandelbrot(m_width, m_height, m_iter);
                    printf("\"Mandelbrot\" operation is done!\n");
                    SaveImage(temp, "mandelbrot");
                }
                break;
            }
            
            /* Add outer boarder */
            case 17:
            {
                if (FileNotRead != 0)
                { printf("No Image Read Yet! Please Read image first.\n"); }
                else
                {
                    printf("Enter boarder width: ");
                    scanf("%d", &border_width);
                    printf("Available border colors : black, white, red, "
                           "green, blue, yellow, cyan, pink, orange\n"
                           "Select border color from the options: ");
                    scanf("%79s", color);
                    
                    temp = AddOuterBorder(image, color, border_width);
                    printf("\"Outer Border\" operation is done!\n");
                    SaveImage(temp, "outerborder");
                }
                break;
            }
            
            /* 18: Test all functions */
            case 18:
            {
                FileNotRead = AutoTest();
                break;
            }
            
            /* 19: Exit */
            case 19:
            {
                printf("Exit.\n"); 
                break;
            }
            
            /* Any other user input will not be accepted. */
            default:
            {
                printf("Invalid selection!\n");
                break;
            }
        }
   }
    #endif

    #ifdef DEBUG
    AutoTest();
    #endif
    
    return 0;
}

void PrintMenu()
{
    printf("--------------------------------\n"
           "1: Load a PPM image\n"
           "2: Save an image in PPM and JPEG format\n"
           "3: Change a color image to Black & White\n"
           "4: Flip an image vertically\n"
           "5: Mirror an image horizontally\n"
           "6: Color filter an image\n"
           "7: Sketch the edge of an image\n"
           "8: Shuffle an image\n"
           "9: Add Border to an image\n"
           "10: Posterize the image\n"
           "11: Fill Lights to an image\n"
           "12: Overlay an image\n"
           "13: Cut and Paste operation on image\n"
           "14: Resize the image\n"
           "15: Rotate 90 degrees clockwise\n"
           "16: Generate the Mandelbrot image\n"
           "17: Add border outside the image\n"
           "18: Test all functions\n"
           "19: Exit\n"
           "please make your choice:");
}

/* auto test*/
int AutoTest()
{
    IMAGE *image;
    IMAGE *temp;
    char fname[SLEN] = "RingMall";
    char img1[SLEN] = "Peter";
    char img2[SLEN] = "Spider";

    /* read image from a file */
    image = ReadImage(fname);
    
    /* apply aging filter */
    temp = Aging(image);
    SaveImage(temp, "aging");
    printf("Aging tested!\n\n");

    /* change color image to black & white */
    temp = BlackNWhite(image);
    SaveImage(temp, "bw");
    printf("Black & White tested!\n\n");
    
    /* flip image vertically */
    temp = VFlip(image);
    SaveImage(temp, "vflip");
    printf("VFlip tested!\n\n");

    /* mirror image horizontally */
    temp = HMirror(image);
    SaveImage(temp, "hmirror");
    printf("HMirror tested!\n\n");

    /* color filter */
    temp = ColorFilter(image, 180, 180, 70, 50, 0, 255, 0);
    SaveImage(temp, "colorfilter");
    printf("ColorFilter tested!\n\n");

    /* edge detection */
    temp = Edge(image);
    SaveImage(temp, "edge");
    printf("Edge tested!\n\n");
    
    /* shuffles the image */
    temp = Shuffle(image);
    SaveImage(temp, "shuffle");
    printf("Shuffle tested!\n\n");
    
    /* add border */
    temp = AddBorder(image, "pink", 20);
    SaveImage(temp, "border");
    printf("AddBorder tested!\n\n");
    
    /* posterize image */
    temp = Posterize(image, 7);
    SaveImage(temp, "poster");
    printf("Posterize tested!\n\n");
    
    /* draw lights on image */
    temp = FillLight(200, 20, image);
    SaveImage(temp, "light");
    printf("FillLight tested!\n\n");
    
    /* overlay an image */
    temp = Overlay(img1, image, 350, 300);
    SaveImage(temp, "overlay_peter");
    temp = Overlay(img2, image, 90, -10);
    SaveImage(temp, "overlay_spider");
    printf("Overlay tested!\n\n");
             
    /* Perform Cut Paste operations on the image */
    temp = CutPaste(image, 370, 20, 27, 57, 3);
    SaveImage(temp, "cutPaste");
    printf("CutPaste tested!\n\n");
    
    /* Resize image */
    temp = Resize(175, image);
    SaveImage(temp, "bigresize");
    temp = Resize(60, image);
    SaveImage(temp, "smallresize");
    printf("Resize tested!\n\n");
    
    /* Rotate image 90 degrees clockwise */
    temp = Rotate(image);
    SaveImage(temp, "rotate");
    printf("Rotate tested!\n\n");
    
    /* Create Mandelbrot image */
    temp = Mandelbrot(720, 538, 2000);
    SaveImage(temp, "mandelbrot");
    printf("Mandelbrot tested!\n\n");
    
    /* Add outside border to image */
    temp = AddOuterBorder(image, "orange", 10);
    SaveImage(temp, "outerborder");
    printf("AddOuterBorder tested!\n\n");

    DeleteImage(image);
    printf("All functions tested!\n\n");
    
    return 0;
}
