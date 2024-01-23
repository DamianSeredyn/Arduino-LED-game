
const int LedPinStart = 8;
const int LedPinEnd = 11;
const int buttonOut = 12;
const int buttonIn = 2;


unsigned long actualTime = 0;
unsigned long currentTime = 0;
unsigned long LedTime = 500;

unsigned int numberOfLeds = 5;
unsigned int index;
int LEDorder[5];

bool IdleMode = false;
bool GamePlaying = false;
bool showLed = false;
void setup() {
  Serial.begin(9600);  
  randomSeed(analogRead(0));
  for(int i = LedPinStart; i<=LedPinEnd;i++)
  {
    pinMode(i, OUTPUT);
  }
  for(int i = 3; i<=7;i++)
  {
    pinMode(i, INPUT);
  }
  pinMode(buttonOut, OUTPUT);
  pinMode(buttonIn, INPUT);
  digitalWrite(buttonOut,HIGH);
  attachInterrupt(digitalPinToInterrupt(buttonIn), StartGame, LOW ); 
}

void loop() {
  actualTime = millis();
  Serial.println(digitalRead(2));
  if(GamePlaying)
  {
    if(showLed)
    {
        if(actualTime - currentTime >= LedTime)
        {
          digitalWrite(LEDorder[index],LOW);
          if(numberOfLeds<index)
          {
            showLed = false;
          }
          else if(actualTime - currentTime >= LedTime+100)
          {
            index++;
            digitalWrite(LEDorder[index],HIGH);
            currentTime = actualTime;
          }
        }
    }
    else
    {

    }
  }
  
  if(IdleMode)
  {

  }
}


void InitGame()
{
    for(int i=0;i<numberOfLeds;i++)
    {
      randomSeed(analogRead(0));
      LEDorder[i] = random(LedPinStart,LedPinEnd+1);
      Serial.println(LEDorder[i]);
    }
    showLed = true;
    GamePlaying = true;
    index = 0;
    currentTime = actualTime;
    digitalWrite(LEDorder[index],HIGH);
}

void StartGame()
{
    if(!GamePlaying)
    InitGame();
}



