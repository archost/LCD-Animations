#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // адрес, столбцов, строк

struct CellData
{
  bool flag;
  uint8_t data[8];
};

static CellData lcdScreen[2][16];

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

template <size_t Rows, size_t Cols>
void printArr(bool (&arr)[Rows][Cols]);

template <size_t artRows, size_t artCols, size_t dispRows, size_t dispCols>
void fillDisplay(bool (&pixelArt)[artRows][artCols], bool (&display)[dispRows][dispCols], uint8_t pointer[2]);

void fillLcdDisplay();
void paint();

void setup()
{
  lcd.init();
  lcd.backlight();
  fillDisplay(pixelArt, display, pointer);
  fillLcdDisplay();
  paint();
}

void loop()
{
}

template <size_t Rows, size_t Cols>
void printArr(bool (&arr)[Rows][Cols])
{
  for (size_t i = 0; i < Rows; ++i)
  {
    for (size_t j = 0; j < Cols; ++j)
    {
      std::cout << arr[i][j];
    }
    std::cout << "\n";
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
  int customCharCount = 0;
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      CellData currentCell = lcdScreen[i][j];
      if (currentCell.flag)
      {
        lcd.setCursor(j, i);
        lcd.createChar(customCharCount, currentCell.data);
        lcd.write(customCharCount);
        customCharCount++;
      }
    }
  }
}
