#define DIR_UP 0
#define DIR_DOWN 3
#define DIR_LEFT 6
#define DIR_RIGHT 9

struct Player
{
  int x, y, healthCurrent, healthTotal, gold, experience, experienceForNextLevel;
  byte currentMap, direction, level, attack, attackAddition, defense, defenseAddition, speed, speedAddition, weapon, armor, other;
  unsigned char name[6];
  byte items[ITEMAMOUNT];
  bool tags[1];
};

Player player;

void resetPlayer()
{
  player =
  {
    144, 128, // position
    20, 20, // health
    100, // gold
    0, 50, // experience
    0, // currentMap
    DIR_DOWN, // direction
    1, // level
    5, 0, // attack
    5, 0, // defence
    5, 0, // speed
    NONE, NONE, NONE, // equipped items
    {NONE, NONE, NONE, NONE, NONE, NONE}, // name
    {1, 0, 0, 0, 0, 0}, // items
    {true}
  };
}

byte playerFrame = 0;
byte playerSteps = 0;

int camX = 0;
int camY = 0;

const unsigned char animSeq[] = { 0, 1, 2, 1 };

const char playerMovement[][4] =
{
  {UP_BUTTON, DIR_UP, 0, -1},
  {DOWN_BUTTON, DIR_DOWN, 0, 1},
  {LEFT_BUTTON, DIR_LEFT, -1, 0},
  {RIGHT_BUTTON, DIR_RIGHT, 1, 0},
};

const char playerCollision[][2] =
{
  {2, 8},{13, 8},{2, 15},{13, 15}
};

PROGMEM const unsigned char playerBitmaps[][32] = 
{  
  {0x00,0x00,0x00,0x90,0x78,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0x78,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x30,0x39,0x03,0x09,0x5b,0x5b,0x01,0x04,0x01,0x00,0x00,0x00}, 
  {0x00,0x00,0x00,0xc8,0x3c,0xfe,0xfe,0xfe,0xfe,0xfe,0xfc,0x3c,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x44,0x5d,0x1c,0x1d,0x5d,0x44,0x18,0x00,0x00,0x00,0x00}, 
  {0x00,0x00,0x00,0x90,0x78,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0x78,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x01,0x5b,0x59,0x0b,0x03,0x39,0x30,0x01,0x00,0x00,0x00}, 
  {0x00,0x00,0x00,0x90,0x78,0xb8,0xdc,0xbc,0xcc,0xdc,0xbc,0x78,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x03,0x54,0x57,0x07,0x34,0x3b,0x00,0x01,0x00,0x00,0x00},  
  {0x00,0x00,0x00,0xc8,0x3c,0xdc,0x6e,0xde,0xe6,0x6e,0xde,0x3c,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x45,0x5a,0x1b,0x1b,0x5a,0x45,0x18,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x90,0x78,0xb8,0xdc,0xbc,0xcc,0xdc,0xbc,0x78,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x3b,0x34,0x07,0x57,0x54,0x03,0x04,0x01,0x00,0x00,0x00}, 
  {0x00,0x00,0x00,0x30,0xb8,0xdc,0xbc,0x7c,0x7c,0xfc,0xf8,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1b,0x18,0x07,0x16,0x1b,0x50,0x49,0x00,0x00,0x00,0x00,0x00}, 
  {0x00,0x00,0x00,0x18,0xdc,0x6e,0xde,0x3e,0xbe,0x7e,0xfc,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x43,0x5b,0x19,0x24,0x00,0x00,0x00,0x00,0x00,0x00}, 
  {0x00,0x00,0x00,0x30,0xb8,0xdc,0xbc,0x7c,0x7c,0xfc,0xf8,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x43,0x54,0x17,0x16,0x0b,0x10,0x0d,0x0c,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0xd0,0xf8,0xfc,0x7c,0x7c,0xbc,0xdc,0xb8,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x0d,0x10,0x0b,0x16,0x17,0x54,0x43,0x00,0x00,0x00,0x00}, 
  {0x00,0x00,0x00,0x00,0x68,0xfc,0x7e,0xbe,0x3e,0xde,0x6e,0xdc,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x24,0x19,0x5b,0x43,0x02,0x01,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0xd0,0xf8,0xfc,0x7c,0x7c,0xbc,0xdc,0xb8,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x49,0x50,0x1b,0x16,0x07,0x18,0x1b,0x00,0x00,0x00,0x00},
};

PROGMEM const unsigned char playerMasks[][32] = 
{
  {0x00,0x00,0x90,0xf8,0xfc,0xfc,0xfe,0xfe,0xfe,0xfe,0xfe,0xfc,0xf8,0x90,0x00,0x00,0x00,0x00,0x01,0x33,0x7f,0x7f,0x7f,0x7f,0xff,0xff,0x7f,0x0f,0x07,0x01,0x00,0x00},
  {0x00,0x00,0xc8,0xfc,0xfe,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfc,0xc8,0x00,0x00,0x00,0x00,0x00,0x19,0x7f,0xff,0xff,0x7f,0x7f,0xff,0xff,0x7f,0x19,0x00,0x00,0x00},
  {0x00,0x00,0x90,0xf8,0xfc,0xfc,0xfe,0xfe,0xfe,0xfe,0xfe,0xfc,0xf8,0x90,0x00,0x00,0x00,0x00,0x01,0x07,0x0f,0x7f,0xff,0xff,0x7f,0x7f,0x7f,0x7f,0x33,0x01,0x00,0x00},
  {0x00,0x00,0x90,0xf8,0xfc,0xfc,0xfe,0xfe,0xfe,0xfe,0xfe,0xfc,0xf8,0x90,0x00,0x00,0x00,0x00,0x01,0x07,0x0f,0x7f,0xff,0xff,0x7f,0x7f,0x7f,0x3f,0x03,0x01,0x00,0x00},
  {0x00,0x00,0xc8,0xfc,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,0xfe,0xfc,0xc8,0x00,0x00,0x00,0x00,0x00,0x19,0x7f,0xff,0xff,0x7f,0x7f,0xff,0xff,0x7f,0x19,0x00,0x00,0x00},
  {0x00,0x00,0x90,0xf8,0xfc,0xfc,0xfe,0xfe,0xfe,0xfe,0xfe,0xfc,0xf8,0x90,0x00,0x00,0x00,0x00,0x01,0x03,0x3f,0x7f,0x7f,0x7f,0xff,0xff,0x7f,0x0f,0x07,0x01,0x00,0x00},
  {0x00,0x00,0x30,0xf8,0xfc,0xfe,0xfe,0xfe,0xfe,0xfe,0xfc,0xf8,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x1b,0x7f,0xff,0xff,0x7f,0x7f,0xff,0xff,0x79,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x18,0xfc,0xfe,0xff,0xff,0xff,0xff,0xff,0xfe,0xfc,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x5f,0xff,0xff,0xff,0x7f,0x3d,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x30,0xf8,0xfc,0xfe,0xfe,0xfe,0xfe,0xfe,0xfc,0xf8,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x5b,0xff,0xff,0x7f,0x7f,0x7f,0xff,0xff,0x5f,0x0c,0x00,0x00,0x00},
  {0x00,0x00,0x00,0xd0,0xf8,0xfc,0xfe,0xfe,0xfe,0xfe,0xfe,0xfc,0xf8,0x30,0x00,0x00,0x00,0x00,0x00,0x0c,0x5f,0xff,0xff,0x7f,0x7f,0x7f,0xff,0xff,0x5b,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x68,0xfc,0xfe,0xff,0xff,0xff,0xff,0xff,0xfe,0xfc,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3d,0x7f,0xff,0xff,0xff,0x5f,0x03,0x01,0x00,0x00,0x00},
  {0x00,0x00,0x00,0xd0,0xf8,0xfc,0xfe,0xfe,0xfe,0xfe,0xfe,0xfc,0xf8,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0xff,0xff,0x7f,0x7f,0xff,0xff,0x7f,0x1b,0x00,0x00,0x00}
};