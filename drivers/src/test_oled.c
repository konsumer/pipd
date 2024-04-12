#include "oled.h"
#include <signal.h>

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

/* Extern volatile */
volatile unsigned char flag = 0;

void ALARMhandler(int sig)
{
    /* Set flag */
    flag = 5;
}

static const unsigned char logo16_glcd_bmp[] =
{
 0b00000000, 0b11000000,
 0b00000001, 0b11000000,
 0b00000001, 0b11000000,
 0b00000011, 0b11100000,
 0b11110011, 0b11100000,
 0b11111110, 0b11111000,
 0b01111110, 0b11111111,
 0b00110011, 0b10011111,
 0b00011111, 0b11111100,
 0b00001101, 0b01110000,
 0b00011011, 0b10100000,
 0b00111111, 0b11100000,
 0b00111111, 0b11110000,
 0b01111100, 0b11110000,
 0b01110000, 0b01110000,
 0b00000000, 0b00110000
};

void testdrawline(int fd) 
{
    short i = 0;
    for (i=0; i<SSD1306_LCDWIDTH; i+=4)
    {
        drawLine(0, 0, i, SSD1306_LCDHEIGHT-1, WHITE);
        Display(fd);
        usleep(1000);
    }
    for (i=0; i<SSD1306_LCDHEIGHT; i+=4)
    {
        drawLine(0, 0, SSD1306_LCDWIDTH-1, i, WHITE);
        Display(fd);
        usleep(1000);
    }
    usleep(250000);

    clearDisplay(fd);
    for (i=0; i<SSD1306_LCDWIDTH; i+=4)
    {
        drawLine(0, SSD1306_LCDHEIGHT-1, i, 0, WHITE);
        Display(fd);
        usleep(1000);
    }
    for (i=SSD1306_LCDHEIGHT-1; i>=0; i-=4)
    {
        drawLine(0, SSD1306_LCDHEIGHT-1, SSD1306_LCDWIDTH-1, i, WHITE);
        Display(fd);
        usleep(1000);
    }
    usleep(250000);

    clearDisplay(fd);
    for (i=SSD1306_LCDWIDTH-1; i>=0; i-=4)
    {
        drawLine(SSD1306_LCDWIDTH-1, SSD1306_LCDHEIGHT-1, i, 0, WHITE);
        Display(fd);
        usleep(1000);
    }
    for (i=SSD1306_LCDHEIGHT-1; i>=0; i-=4)
    {
        drawLine(SSD1306_LCDWIDTH-1, SSD1306_LCDHEIGHT-1, 0, i, WHITE);
        Display(fd);
        usleep(1000);
    }
    usleep(250000);

    clearDisplay(fd);
    for (i=0; i<SSD1306_LCDHEIGHT; i+=4)
    {
        drawLine(SSD1306_LCDWIDTH-1, 0, 0, i, WHITE);
        Display(fd);
        usleep(1000);
    }
    for (i=0; i<SSD1306_LCDWIDTH; i+=4) {
        drawLine(SSD1306_LCDWIDTH-1, 0, i, SSD1306_LCDHEIGHT-1, WHITE);
        Display(fd);
        usleep(1000);
    }
    usleep(250000);
}

void testdrawrect(int fd) 
{
    short i = 0;
    for (i=0; i<SSD1306_LCDHEIGHT/2; i+=2)
    {
        drawRect(i, i,SSD1306_LCDWIDTH-2*i, SSD1306_LCDHEIGHT-2*i, WHITE);
        Display(fd);
        usleep(1000);
    }
}

void testfillrect(int fd) 
{
    unsigned char color = 1;
    short i = 0;
    for (i=0; i<SSD1306_LCDHEIGHT/2; i+=3)
    {
        // alternate colors
        fillRect(i, i, SSD1306_LCDWIDTH-i*2, SSD1306_LCDHEIGHT-i*2, color%2);
        Display(fd);
        usleep(1000);
        color++;
    }
}

void testdrawcircle(int fd)
{
    short i = 0;
    for (i=0; i<SSD1306_LCDHEIGHT; i+=2)
    {
        drawCircle(SSD1306_LCDWIDTH/2,SSD1306_LCDHEIGHT/2, i, WHITE);
        Display(fd);
        usleep(1000);
    }
}

void testdrawroundrectint(int fd) {
  short i = 0;
  for (i = 0; i < SSD1306_LCDHEIGHT / 2 - 2; i += 2) {
    drawRoundRect(i, i, SSD1306_LCDWIDTH - 2 * i, SSD1306_LCDHEIGHT - 2 * i, SSD1306_LCDHEIGHT / 4, WHITE);
    Display(fd);
    usleep(1000);
  }
}

void testdrawroundrect(int fd) {
    short i = 0;
    for (i=0; i<SSD1306_LCDHEIGHT/2-2; i+=2) {
        drawRoundRect(i, i,SSD1306_LCDWIDTH-2*i, SSD1306_LCDHEIGHT-2*i, SSD1306_LCDHEIGHT/4, WHITE);
        Display(fd);
        usleep(1000);
    }
}

void testfillroundrect(int fd) {
    short color = WHITE,i = 0;
    for (i=0; i<SSD1306_LCDHEIGHT/2-2; i+=2)
    {
        fillRoundRect(i, i, SSD1306_LCDWIDTH-2*i, SSD1306_LCDHEIGHT-2*i, SSD1306_LCDHEIGHT/4, color);
        if (color == WHITE)
            color = BLACK;
        else
            color = WHITE;
        Display(fd);
        usleep(1000);
    }
}

void testdrawtriangle(int fd) 
{
    short i = 0;
    for (i=0; i<MIN(SSD1306_LCDWIDTH,SSD1306_LCDHEIGHT)/2; i+=5)
    {
        drawTriangle(SSD1306_LCDWIDTH/2, SSD1306_LCDHEIGHT/2-i,
                     SSD1306_LCDWIDTH/2-i,SSD1306_LCDHEIGHT /2+i,
                     SSD1306_LCDWIDTH/2+i, SSD1306_LCDHEIGHT/2+i, WHITE);
        Display(fd);
        usleep(1000);
    }
}

void testfilltriangle(int fd) 
{
    unsigned char color = WHITE;
    short i = 0;
    for (i=MIN(SSD1306_LCDWIDTH,SSD1306_LCDHEIGHT)/2; i>0; i-=5)
    {
        fillTriangle(SSD1306_LCDWIDTH/2, SSD1306_LCDHEIGHT/2-i,
                     SSD1306_LCDWIDTH/2-i, SSD1306_LCDHEIGHT/2+i,
                     SSD1306_LCDWIDTH/2+i, SSD1306_LCDHEIGHT/2+i, WHITE);
        if (color == WHITE)
            color = BLACK;
        else
            color = WHITE;
        Display(fd);
        usleep(1000);
    }
}

void testdrawchar(int fd)
{
    unsigned char i = 0;
    setTextSize(1);
    setTextColor(WHITE);
    setCursor(0,0);

    for (i=0; i < 168; i++)
    {
        if (i == '\n')
            continue;
        oled_write(i);
        if ((i > 0) && (i % 21 == 0))
            println();
    }
    Display(fd);
    usleep(1000);
}

/* Display "scroll" and scroll around */
void testscrolltext(int fd)
{
    setTextSize(2);
    setTextColor(WHITE);
    setCursor(10,0);
    print_str("scroll");
    println();
    Display(fd);
    usleep(1000);
    startscrollright(fd, 0x00, 0x0F);
    usleep(2000000);
    stopscroll(fd);
    usleep(1000000);
    startscrollleft(fd, 0x00, 0x0F);
    usleep(2000000);
    stopscroll(fd);
    usleep(1000000);
    startscrolldiagright(fd, 0x00, 0x07);
    usleep(2000000);
    startscrolldiagleft(fd, 0x00, 0x07);
    usleep(2000000);
    stopscroll(fd);
}

/* Display Texts */
void display_texts(int fd)
{
    setTextSize(1);
    setTextColor(WHITE);
    setCursor(10,0);
    print_str("HELLO FELLAS!");
    println();
    printFloat_ln(3.141592, 4); //Print 4 No's after the decimal Pt.
    printNumber_L_ln(-1234, DEC);
    printNumber_UC_ln(170, BIN);
    setTextSize(2);
    setTextColor(WHITE);
    print_str("0x");
    printNumber_UL_ln(0xDEADBEEF, HEX);
}

/* Display miniature bitmap */
void display_bitmap(int fd)
{
    drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
}

/* Invert Display and Normalize it */
void display_invert_normal(int fd)
{
    invertDisplay(fd, SSD1306_INVERT_DISPLAY);
    usleep(1000000);
    invertDisplay(fd, SSD1306_NORMALIZE_DISPLAY);
    usleep(1000000);
}

/* Draw a bitmap and 'animate' movement */
void testdrawbitmap(int fd, const unsigned char *bitmap, unsigned char w, unsigned char h)
{
    unsigned char icons[NUMFLAKES][3], f = 0;

    // initialize
    for (f=0; f< NUMFLAKES; f++)
    {
        icons[f][XPOS] = rand() % SSD1306_LCDWIDTH;
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = (rand() % 5) + 1;

        /* Looks kinna ugly to me - Un-Comment if you need it */
        //print_str("x: ");
        //printNumber_UC(icons[f][XPOS], DEC);
        //print_str("y: ");
        //printNumber_UC(icons[f][YPOS], DEC);
        //print_str("dy: ");
        //printNumber_UC(icons[f][DELTAY], DEC);
    }

    while (flag != 5)
    {
        // draw each icon
        for (f=0; f< NUMFLAKES; f++)
        {
            drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
        }
        Display(fd);
        usleep(200000);

        // then erase it + move it
        for (f=0; f< NUMFLAKES; f++)
        {
            drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);

            // move it
            icons[f][YPOS] += icons[f][DELTAY];

            // if its gone, reinit
            if (icons[f][YPOS] > SSD1306_LCDHEIGHT)
            {
                icons[f][XPOS] = rand() % SSD1306_LCDWIDTH;
                icons[f][YPOS] = 0;
                icons[f][DELTAY] = (rand() % 5) + 1;
            }
        }
    }
}

/* Draw bitmap and animate */
void testdrawbitmap_eg(int fd)
{
    setTextSize(1);
    setTextColor(WHITE);
    setCursor(10,0);
    testdrawbitmap(fd, logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}

int main(int argc, char *argv[]) {
  int fd = i2c_open(1);

  if (fd < 0) {
    perror("Could not open i2c bus.");
    return 1;
  }

  if (i2c_set_addr(fd, SSD1306_OLED_ADDR) < 0) {
    perror("Could not open the encoder.");
    return 1;
  }

  signal(SIGALRM, ALARMhandler);

  oled_setup(fd);

  // draw a single pixel
  drawPixel(0, 1, WHITE);
  Display(fd);
  usleep(1000000);
  clearDisplay(fd);

  // draw many lines
  testdrawline(fd);
  usleep(1000000);
  clearDisplay(fd);

  // draw rectangles
  testdrawrect(fd);
  usleep(1000000);
  clearDisplay(fd);

  // draw multiple rectangles
  testfillrect(fd);
  usleep(1000000);
  clearDisplay(fd);

  // draw mulitple circles
  testdrawcircle(fd);
  usleep(1000000);
  clearDisplay(fd);

  // draw a white circle, 10 pixel radius
  fillCircle(SSD1306_LCDWIDTH / 2, SSD1306_LCDHEIGHT / 2, 10, WHITE);
  Display(fd);
  usleep(1000000);
  clearDisplay(fd);

  // draw a white circle, 10 pixel radius
  testdrawroundrect(fd);
  usleep(1000000);
  clearDisplay(fd);

  // Fill the round rectangle
  testfillroundrect(fd);
  usleep(1000000);
  clearDisplay(fd);

  // Draw triangles
  testdrawtriangle(fd);
  usleep(1000000);
  clearDisplay(fd);

  // Fill triangles
  testfilltriangle(fd);
  usleep(1000000);
  clearDisplay(fd);

  // draw the first ~12 characters in the font
  testdrawchar(fd);
  usleep(1000000);
  clearDisplay(fd);

  // Display "scroll" and scroll around
  testscrolltext(fd);
  usleep(1000000);
  clearDisplay(fd);

  // Display Texts and Numbers
  display_texts(fd);
  Display(fd);
  usleep(1000000);
  clearDisplay(fd);

  // Display miniature bitmap
  display_bitmap(fd);
  Display(fd);
  usleep(1000000);

  // Display Inverted image and normalize it back
  display_invert_normal(fd);
  clearDisplay(fd);
  usleep(1000000);
  Display(fd);

  // Generate Signal after 20 Seconds
  alarm(20);

  // draw a bitmap icon and 'animate' movement
  testdrawbitmap_eg(fd);
  clearDisplay(fd);
  usleep(1000000);
  Display(fd);

  return 0;
}
