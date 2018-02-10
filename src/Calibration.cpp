#include <Adafruit_STMPE610.h>
#include "Output.h"
#include "Persistent.h"
#include "Calibration.h"

CalibrationData calibration;

void calibrateX(Adafruit_STMPE610& touch) {
  uint16_t raw_x, raw_y;
  uint8_t raw_z;
  int count, raw_1, raw_2;
  long total;

  int screen_1 = 40;
  int screen_2 = 280;

  // Left
  Output.clear();
  Output.drawHorzLine(screen_1 - 10, 120, 20, Colours::Red);
  Output.drawVertLine(screen_1, 110, 20, Colours::Red);
  while (touch.bufferEmpty()) {
    delay(50);
  }
  count = 0;
  total = 0L; 
  while (!touch.bufferEmpty()) {
    touch.readData(&raw_x, &raw_y, &raw_z);
    if (count < 10) {
      total += raw_y;
      count++;
    }
    delay(50);
  }
  raw_1 = (int) (total / count);

  // Right
  Output.clear();
  Output.drawHorzLine(screen_2 - 10, 120, 20, Colours::Red);
  Output.drawVertLine(screen_2, 110, 20, Colours::Red);
  while (touch.bufferEmpty()) {
    delay(50);
  }
  count = 0;
  total = 0L;
  while (!touch.bufferEmpty()) {
    touch.readData(&raw_x, &raw_y, &raw_z);
    if (count < 10) {
      total += raw_y;
      count++;
    }
    delay(50);
  }
  raw_2 = (int) (total / count);

  // Change raw range to start at zero
  int raw_offset = raw_1;
  raw_2 -= raw_offset;

  // Raw range is now 0 to raw_2, do the same for screen
  int screen_offset = screen_1;
  screen_2 -= screen_offset;

  // Find the scale factor to convert raw range to screen range
  calibration.scale_x = ((float) raw_2 / (float) screen_2);

  // Calculate the final offset
  calibration.offset_x = (raw_offset / calibration.scale_x) - screen_offset;

  Serial.printf("offset_x = %f, scale_x = %f\n", calibration.offset_x, calibration.scale_x);
}

void calibrateY(Adafruit_STMPE610& touch) {
  uint16_t raw_x, raw_y;
  uint8_t raw_z;
  int count, raw_1, raw_2;
  long total;

  int screen_1 = 40;
  int screen_2 = 200;

  // Top
  Output.clear();
  Output.drawHorzLine(150, screen_1, 20, Colours::Red);
  Output.drawVertLine(160, screen_1 - 10, 20, Colours::Red);
  while (touch.bufferEmpty()) {
    delay(50);
  }
  count = 0;
  total = 0L; 
  while (!touch.bufferEmpty()) {
    touch.readData(&raw_x, &raw_y, &raw_z);
    if (count < 10) {
      total += (4096 - raw_x);
      count++;
    }
    delay(50);
  }
  raw_1 = (int) (total / count);
  Serial.printf("Top raw = %d\n", raw_1);

  // Bottom
  Output.clear();
  Output.drawHorzLine(150, screen_2, 20, Colours::Red);
  Output.drawVertLine(160, screen_2 - 10, 20, Colours::Red);
  while (touch.bufferEmpty()) {
    delay(50);
  }
  count = 0;
  total = 0L;
  while (!touch.bufferEmpty()) {
    touch.readData(&raw_x, &raw_y, &raw_z);
    if (count < 10) {
      total += (4096 - raw_x);
      count++;
    }
    delay(50);
  }
  raw_2 = (int) (total / count);
  Serial.printf("Bottom raw = %d\n", raw_2);

  // Change raw range to start at zero
  int raw_offset = raw_1;
  raw_2 -= raw_offset;

  // Raw range is now 0 to raw_2, do the same for screen
  int screen_offset = screen_1;
  screen_2 -= screen_offset;

  // Find the scale factor to convert raw range to screen range
  calibration.scale_y = ((float) raw_2 / (float) screen_2);

  // Calculate the final offset
  calibration.offset_y = (raw_offset / calibration.scale_y) - screen_offset;

  Serial.printf("offset_y = %f, scale_y = %f\n", calibration.offset_y, calibration.scale_y);
}

void calibrate(Adafruit_STMPE610& touch) {
    calibrateX(touch);
    calibrateY(touch);
    Persistent.storeCalibration(&calibration);
}

void loadCalibration() {
    Persistent.fetchCalibration(&calibration);    
    Serial.printf("offset_x = %f, scale_x = %f\n", calibration.offset_x, calibration.scale_x);
    Serial.printf("offset_y = %f, scale_y = %f\n", calibration.offset_y, calibration.scale_y);
}

void transform(int raw_x, int raw_y, int* pscreen_x, int* pscreen_y) {
    *pscreen_x = (raw_y / calibration.scale_x) - calibration.offset_x;
    *pscreen_y = ((4096 - raw_x) / calibration.scale_y) - calibration.offset_y;
}
