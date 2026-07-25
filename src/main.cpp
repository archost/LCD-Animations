#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

LiquidCrystal_I2C lcd(0x27, 16, 2); // адрес, столбцов, строк

struct CellData
{
  bool flag;
  uint8_t data[8];
};

static CellData lcdScreen[2][16];

bool display[17][95] = {0};

bool pixelArt[7][11] = {
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0}};

uint8_t pointer[2] = {0, 0};

template <size_t artRows, size_t artCols, size_t dispRows, size_t dispCols>
void fillDisplay(bool (&pixelArt)[artRows][artCols], bool (&display)[dispRows][dispCols], uint8_t pointer[2]);

template <size_t dispRows, size_t dispCols>
void clearDisplay(bool (&display)[dispRows][dispCols]);

void fillLcdDisplay();
void paint();
void test();

void setup()
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  lcd.init();
  lcd.backlight();
}

void loop()
{
  static unsigned long tmr = 0;
  if (millis() - tmr > 250)
  {
    tmr = millis();
    pointer[1]++;
    clearDisplay(display);
    fillDisplay(pixelArt, display, pointer);
    fillLcdDisplay();
    paint();
  }
}

template <size_t artRows, size_t artCols, size_t dispRows, size_t dispCols>
void fillDisplay(bool (&pixelArt)[artRows][artCols], bool (&display)[dispRows][dispCols], uint8_t pointer[2])
{
  for (int i = 0; i < artRows; i++)
  {
    for (int j = 0; j < artCols; j++)
    {
      display[i + pointer[0]][j + pointer[1]] = pixelArt[i][j];
    }
  }
}

template <size_t dispRows, size_t dispCols>
void clearDisplay(bool (&display)[dispRows][dispCols])
{
  for (int i = 0; i < dispRows; i++)
  {
    for (int j = 0; j < dispCols; j++)
    {
      display[i][j] = 0;
    }
  }
}

void fillLcdDisplay()
{
  for (int lcdRow = 0; lcdRow < 2; lcdRow++)
  {
    for (int lcdCol = 0; lcdCol < 16; lcdCol++)
    {
      uint8_t temp[8] = {0};
      bool flag = false;
      for (int j = 0; j < 8; j++)
      {
        uint8_t resultByte = 0;
        for (int i = 0; i < 5; i++)
        {
          if (display[lcdRow * 9 + j][lcdCol * 6 + i] == true)
          {
            flag = true;

            int shiftAmount = 4 - i;
            resultByte = resultByte | (1 << shiftAmount);
          }
        }
        temp[j] = resultByte;
      }
      for (int k = 0; k < 8; k++)
      {
        lcdScreen[lcdRow][lcdCol].data[k] = temp[k];
      }
      lcdScreen[lcdRow][lcdCol].flag = flag;
    }
  }
}

void paint()
{
  uint8_t customCharCount = 0;
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      if (lcdScreen[i][j].flag)
      {
        lcd.createChar(customCharCount, lcdScreen[i][j].data);
        lcd.setCursor(j, i);
        lcd.write(customCharCount);
        customCharCount++;
      }
      else
      {
        lcd.setCursor(j, i);
        lcd.write(' ');
      }
    }
  }
}
