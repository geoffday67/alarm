#include "Output.h"

#define BUFFPIXEL 80

classOutput Output;

classOutput::classOutput() {}

classOutput::~classOutput() {}

void classOutput::begin() {
  pTFT = new TFT_eSPI();
  pTFT->begin();
  pTFT->setRotation(3);
  pTFT->setTextFont(7);

  clear();
}

void classOutput::clear() {
  pTFT->fillScreen(TFT_WHITE);    
}

void classOutput::showCentredText(int left, int top, int width, int height, const char *ptext, Colours colour) {
  pTFT->setTextColor((int) colour, TFT_WHITE);
  pTFT->setTextDatum(MC_DATUM);
  int x = left + (width / 2);
  int y = top + (height / 2);
  pTFT->drawString(ptext, x, y);
}

void classOutput::showText(int x, int y, const char *ptext, Colours foreground) {
  pTFT->setTextColor((int) foreground, TFT_WHITE);

  if (x == SCREEN_CENTRE) {
    x = pTFT->width() / 2;
    pTFT->setTextDatum(TC_DATUM);
  } else {
    pTFT->setTextDatum(TL_DATUM);    
  }

  pTFT->drawString(ptext, x, y);
}

void classOutput::drawRectangle(int x, int y, int width, int height, Colours colour) {
  pTFT->drawRect(x, y, width, height, (int) colour);
}

void classOutput::fillRectangle(int x, int y, int width, int height, Colours colour) {
  pTFT->fillRect(x, y, width, height, (int) colour);
}

void classOutput::drawImage(int x, int y, int width, int height, Colours colour, const unsigned char* pimage) {
  pTFT->fillRect(x, y, width, height, (int) Colours::White);
	pTFT->drawBitmap(x, y, pimage, width, height, (int) colour);
}

void classOutput::drawHorzLine(int x, int y, int length, Colours colour) {
  pTFT->drawFastHLine(x, y, length, (int) colour);
}

void classOutput::drawVertLine(int x, int y, int length, Colours colour) {
  pTFT->drawFastVLine(x, y, length, (int) colour);
}

void classOutput::drawBitmap(int x, int y, const char *pfilename) {
	bmpDraw (pfilename, x, y);
}

void classOutput::showIdleTime(time_t time) {}
void classOutput::showImage(int x, int y, const char *filename) {}
void classOutput::playSound() {}

void classOutput::startAlarm() {
  /*
  Send serial command through Serial1 - 'a' is start, 'b' is stop
  */
  Serial1.write('a');
}

void classOutput::stopAlarm() {
  Serial1.write('b');
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t classOutput::read16(File &f)
{
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t classOutput::read32(File &f)
{
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

void classOutput::bmpDraw(String filename, int16_t x, int16_t y) {
  // Flips the TFT internal SGRAM coords to draw bottom up BMP images faster, in this application it can be fixed
  boolean flip = 1;

  if ((x >= pTFT->width()) || (y >= pTFT->height())) return;

  fs::File bmpFile;
  int16_t  bmpWidth, bmpHeight;   // Image W+H in pixels
  uint32_t bmpImageoffset;        // Start address of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL];    // file read pixel buffer (8 bits each R+G+B per pixel)
  uint16_t tftbuffer[BUFFPIXEL];       // TFT pixel out buffer (16-bit per pixel)
  uint8_t  rgb_ptr = sizeof(sdbuffer); // read 24 bit RGB pixel data buffer pointer (8 bit so BUFF_SIZE must be less than 86)
  boolean  goodBmp = false;            // Flag set to true on valid header parse
  int16_t  w, h, row, col;             // to store width, height, row and column
  uint8_t rotation;      // to restore rotation
  uint8_t  tft_ptr = 0;  // TFT 16 bit 565 format pixel data buffer pointer

  // Check file exists and open it
  Serial.println(filename);
  if ( !(bmpFile = SPIFFS.open(filename, "r")) ) {
    Serial.println(F(" File not found")); // Can comment out if not needed
    return;
  }

  // Parse BMP header to get the information we need
  if (read16(bmpFile) == 0x4D42) { // BMP file start signature check
    read32(bmpFile);       // Dummy read to throw away and move on
    read32(bmpFile);       // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    read32(bmpFile);       // Dummy read to throw away and move on
    bmpWidth  = read32(bmpFile);  // Image width
    bmpHeight = read32(bmpFile);  // Image height

    // Only proceed if we pass a bitmap file check
    // Number of image planes -- must be '1', depth 24 and 0 (uncompressed format)
    if ((read16(bmpFile) == 1) && (read16(bmpFile) == 24) && (read32(bmpFile) == 0)) {
      goodBmp = true; // Supported BMP format
      // BMP rows are padded (if needed) to 4-byte boundary
      rowSize = (bmpWidth * 3 + 3) & ~3;
      // Crop area to be loaded
      w = bmpWidth;
      h = bmpHeight;

      // We might need to alter rotation to avoid tedious file pointer manipulation
      // Save the current value so we can restore it later
      rotation = pTFT->getRotation();
      // Use TFT SGRAM coord rotation if flip is set for 25% faster rendering (new rotations 4-7 supported by library)
      if (flip) pTFT->setRotation((rotation + (flip<<2)) % 8); // Value 0-3 mapped to 4-7

      // Calculate new y plot coordinate if we are flipping
      switch (rotation) {
        case 0:
          if (flip) y = pTFT->height() - y - h; break;
        case 1:
          y = pTFT->height() - y - h; break;
          break;
        case 2:
          if (flip) y = pTFT->height() - y - h; break;
          break;
        case 3:
          y = pTFT->height() - y - h; break;
          break;
      }

      // Set TFT address window to image bounds
      // Currently, image will not draw or will be corrputed if it does not fit
      // TODO -> efficient clipping, but I don't need it to be idiot proof ;-)
      pTFT->setAddrWindow(x, y, x + w - 1, y + h - 1);

      // Finally we are ready to send rows of pixels, writing like this avoids slow 32 bit multiply in 8 bit processors
      for (uint32_t pos = bmpImageoffset; pos < bmpImageoffset + h * rowSize ; pos += rowSize) {
        // Seek if we need to on boundaries and arrange to dump buffer and start again
        if (bmpFile.position() != pos) {
          bmpFile.seek(pos, fs::SeekSet);
          rgb_ptr = sizeof(sdbuffer);
          //Serial.println("Seeking in file >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
        }

        // Fill the pixel buffer and plot
        for (col = 0; col < w; col++) { // For each column...
          // Time to read more pixel data?
          if (rgb_ptr >= sizeof(sdbuffer)) {
            // Push tft buffer to the display
            if (tft_ptr) {
              // Here we are sending a uint16_t array to the function
              pTFT->pushColors(tftbuffer, tft_ptr);
              tft_ptr = 0; // tft_ptr and rgb_ptr are not always in sync...
            }
            // Finally reading bytes from SD Card
            bmpFile.read(sdbuffer, sizeof(sdbuffer));
            rgb_ptr = 0; // Set buffer index to start
          }
          // Convert pixel from BMP 8+8+8 format to TFT compatible 16 bit word
          // Blue 5 bits, green 6 bits and red 5 bits (16 bits total)
          // Is is a long line but it is faster than calling a library fn for this
          tftbuffer[tft_ptr] = (sdbuffer[rgb_ptr++] >> 3) ;
          tftbuffer[tft_ptr] |= ((sdbuffer[rgb_ptr++] & 0xFC) << 3);
          tftbuffer[tft_ptr] |= ((sdbuffer[rgb_ptr++] & 0xF8) << 8);
          tft_ptr++;
        } // Next row
      }   // All rows done

      // Write any partially full buffer to TFT
      if (tft_ptr) pTFT->pushColors(tftbuffer, tft_ptr);

    } // End of bitmap access
  }   // End of bitmap file check

  bmpFile.close();
  
  if(!goodBmp) {
    Serial.print(F("BMP format not recognised. File:"));
    Serial.println(filename);
  }
  else
    pTFT->setRotation(rotation); // Put back original rotation

}
