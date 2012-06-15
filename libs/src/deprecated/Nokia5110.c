#include "../include/Nokia5110.h"

/* write and ascii character */
void
LcdCharacter(char character)
{
  LcdWrite(LCD_D, 0x00);
  int index;
  for (index = 0; index < 5; index++) {
    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_D, 0x00);
}

/* clear the entire LCD */
void
LcdClear(void)
{
  int index;
  for (index = 0; index < LCD_X * LCD_Y / 8; index++) {
    LcdWrite(LCD_D, 0x00);
  }
}

/* init the SPI port and initialize the LCD */
void
LcdInitialize(void)
{
    mInitLCDPins();
    SpiChnOpen(SPI_CHANNEL3, SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON, SYS_FREQ/100000); /* will open SPI3 and set the bit rate to .1MHz */

    mRESET_Low();
    mRESET_High();

    mSCE_High();

    LcdWrite(LCD_C, 0x21 );  /* LCD Extended Commands.                */
    LcdWrite(LCD_C, (char)185 );   /* Set LCD Vop (Contrast).               */
    LcdWrite(LCD_C, 0x04 );  /* Set Temp coefficent.                  */
    LcdWrite(LCD_C, 0x14 );  /* LCD bias mode 1:48.                   */
    LcdWrite(LCD_C, 0x0C );  /* LCD in normal mode. 0x0d for inverse  */
    LcdWrite(LCD_C, 0x20);
    LcdWrite(LCD_C, 0x0C);
}

/* send a string of characters to the LCD */
void
LcdString(const char *characters)
{
  while (*characters) {
    LcdCharacter(*characters++);
  }
}

/* write an individal command to the LCD */
void
LcdWrite(char dc, char data)
{
  if(dc) {
    mDC_High();
  }
  else {
    mDC_Low();
  }

  mSCE_Low();

  SpiChnPutC(SPI_CHANNEL3, (unsigned int)data);

  /* wait a little for the LCD to respond to the command */
  int i;

  for (i=0; i<1000; i++) {
    Nop();
  }

/*  delay_us(15); */

  mSCE_High();
}

/* position the cursor 
 * x - range: 0 to 84
 * y - range: 0 to 5
 */
void
gotoXY(int x, int y)
{
  LcdWrite( 0, (char)(0x80 | x));  /* Column.   */
  LcdWrite( 0, (char)(0x40 | y));  /* Row.      */
}

/* write to a specific pixel
 * x: 0 to 84, y: 0 to 48
 */
void
setPixel(int x, int y)
{
  if (x > 84 || y > 48)
    return;

  /* The LCD has 6 rows, with 8 pixels per  row.
   * 'y_mod' is the row that the pixel is in.
   * 'y_pix' is the pixel in that row we want to enable/disable
   */
  int y_mod = (int)(y >> 3);	/* >>3 divides by 8     */
  int y_pix = (y-(y_mod << 3)); /* <<3 multiplies by 8  */
  int val = 1 << y_pix;

  /* Write the updated pixel out to the LCD */
  gotoXY(x,y_mod);
  LcdWrite (1,(char)val);
}