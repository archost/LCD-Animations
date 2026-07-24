#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // адрес, столбцов, строк

uint8_t customChar[] = {
    1,
    6,
    8,
    16,
    8,
    6,
    1,
    0};

bool display[18][96] = {0};

bool pixelArt[7][11] = {
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0}};

uint8_t pointer[2] = {0, 0};

void fillDisplay()
{
  int rows = sizeof(pixelArt) / sizeof(pixelArt[0]);
  int cols = sizeof(pixelArt[0]) / sizeof(pixelArt[0][0]);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      display[i + pointer[0]][j + pointer[1]] = pixelArt[i][j];
    }
  }
}

void setup()
{
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, customChar);

  lcd.setCursor(1, 0);
  lcd.write(0);
}

void loop()
{
}