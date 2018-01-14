#include "Output.h"

#define BUFFPIXEL 80
#define TFT_DC 0
#define TFT_CS 15

Output::Output() {
	pTFT= new  Adafruit_ILI9341(TFT_CS, TFT_DC);
  	pTFT->begin();
  	pTFT->setRotation(3);
  clear();
}

Output::~Output() {}

void Output::clear() {
  pTFT->fillScreen(ILI9341_WHITE);    
}

void Output::showText(int x, int y, const char *ptext, Colours colour) {
    pTFT->setCursor(x, y);
    pTFT->setTextColor((int) colour, ILI9341_WHITE);
    pTFT->setTextSize(3);
    pTFT->print(ptext);
}

void Output::drawRectangle(int x, int y, int width, int height, Colours colour) {
    pTFT->drawRect(x, y, width, height, (int) colour);
}

void Output::drawImage(int x, int y, int width, int height, Colours colour, const unsigned char* pimage) {
	pTFT->drawBitmap(x, y, pimage, width, height, (int) colour);
}

void Output::drawBitmap(int x, int y, const char *pfilename) {
	bmpDraw (pfilename, x, y);
}

void Output::showIdleTime(time_t time) {}
void Output::showImage(int x, int y, const char *filename) {}
void Output::playSound() {}
void Output::soundAlarm() {}

uint16_t Output::read16(File &f)
{
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t Output::read32(File &f)
{
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

void Output::bmpDraw(String filename, int16_t x, int16_t y)
{
  File bmpFile;
  int bmpWidth, bmpHeight;            // W+H in pixels
  uint8_t bmpDepth;                   // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;            // Start of image data in file
  uint32_t rowSize;                   // Not always = bmpWidth; may have padding
  uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel buffer (R+G+B per pixel)
  uint8_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean goodBmp = false;            // Set to true on valid header parse
  boolean flip = true;                // BMP is stored bottom-to-top
  int w, h, row, col, x2, y2, bx1, by1;
  uint8_t r, g, b;
  uint32_t pos = 0;

  if ((x >= pTFT->width()) || (y >= pTFT->height()))
    return;

  // Open requested file on SD card
  if (!(bmpFile = SPIFFS.open(filename, "r")))
  {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42)
  { // BMP signature
    Serial.print(F("File size: "));
    Serial.println(read32(bmpFile));
    (void)read32(bmpFile);            // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    // Read DIB header
    read32(bmpFile);
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1)
    {                             // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      if ((bmpDepth == 24) && (read32(bmpFile) == 0))
      { // 0 = uncompressed
        goodBmp = true; // Supported BMP format -- proceed!

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0)
        {
          bmpHeight = -bmpHeight;
          flip = false;
        }

        // Crop area to be loaded
        x2 = x + bmpWidth - 1; // Lower-right corner
        y2 = y + bmpHeight - 1;
        if ((x2 >= 0) && (y2 >= 0))
        {               // On screen?
          w = bmpWidth; // Width/height of section to load/display
          h = bmpHeight;
          bx1 = by1 = 0; // UL coordinate in BMP file
          if (x < 0)
          { // Clip left
            bx1 = -x;
            x = 0;
            w = x2 + 1;
          }
          if (y < 0)
          { // Clip top
            by1 = -y;
            y = 0;
            h = y2 + 1;
          }
          if (x2 >= pTFT->width())
            w = pTFT->width() - x; // Clip right
          if (y2 >= pTFT->height())
            h = pTFT->height() - y; // Clip bottom

          // Set TFT address window to clipped image bounds
          pTFT->startWrite(); // Requires start/end transaction now
          pTFT->setAddrWindow(x, y, w, h);

          for (row = 0; row < h; row++)
          { // For each scanline...

            // Seek to start of scan line.  It might seem labor-
            // intensive to be doing this on every line, but this
            // method covers a lot of gritty details like cropping
            // and scanline padding.  Also, the seek only takes
            // place if the file position actually needs to change
            // (avoids a lot of cluster math in SD library).
            if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
              pos = bmpImageoffset + (bmpHeight - 1 - (row + by1)) * rowSize;
            else // Bitmap is stored top-to-bottom
              pos = bmpImageoffset + (row + by1) * rowSize;
            pos += bx1 * 3; // Factor in starting column (bx1)
            if (bmpFile.position() != pos)
            {                 // Need seek?
              pTFT->endWrite(); // End TFT transaction
              bmpFile.seek(pos, SeekSet);
              buffidx = sizeof(sdbuffer); // Force buffer reload
              pTFT->startWrite();           // Start new TFT transaction
            }
            for (col = 0; col < w; col++)
            { // For each pixel...
              // Time to read more pixel data?
              if (buffidx >= sizeof(sdbuffer))
              {                 // Indeed
                pTFT->endWrite(); // End TFT transaction
                bmpFile.read(sdbuffer, sizeof(sdbuffer));
                buffidx = 0;      // Set index to beginning
                pTFT->startWrite(); // Start new TFT transaction
              }
              // Convert pixel from BMP to TFT format, push to display
              b = sdbuffer[buffidx++];
              g = sdbuffer[buffidx++];
              r = sdbuffer[buffidx++];
              pTFT->writePixel(pTFT->color565(r, g, b));
            }             // end pixel
          }               // end scanline
          pTFT->endWrite(); // End last TFT transaction
        }                 // end onscreen
      } // end goodBmp
    }
  }

  bmpFile.close();
  if (!goodBmp)
    Serial.println(F("BMP format not recognized."));
}
