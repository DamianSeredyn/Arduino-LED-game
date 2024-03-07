
const int LedPinStart = 8;
const int LedPinEnd = 11;
const int buzzer = 12;
const int buttonIn = 2;

const int loseDiode = 7;
const int winDiode = 13;

unsigned long actualTime = 0;
unsigned long currentTime = 0;
unsigned long buttonTime = 0;
unsigned long EndLEDTime = 0;
unsigned long LedTime = 300;
unsigned long lightime = 0;

unsigned long currentPressed = 0;

unsigned int numberOfLeds = 5;
unsigned int playerProgress = 0;
unsigned int index;
int LEDorder[5];

bool IdleMode = false;
bool GamePlaying = false;
bool GameFinished = false;
bool showLed = false;
void setup() {
  Serial.begin(9600);  
  randomSeed(analogRead(0));
  for(int i = LedPinStart; i<=13;i++)
  {
    pinMode(i, OUTPUT);
  }
  for(int i = 3; i<=7;i++)
  {
    pinMode(i, INPUT);
  }
  pinMode(buzzer, OUTPUT);
  pinMode(buttonIn, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonIn), StartGame, LOW ); 
}

void loop() {
  actualTime = millis();
  if(GamePlaying)
  {
    if(showLed)
    {
        if(actualTime - currentTime >= LedTime)
        {
          digitalWrite(LEDorder[index],LOW);
          if(numberOfLeds-1<=index)
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
      if(actualTime - buttonTime >= 25)
      {
        if(actualTime - lightime >= 10)
        {
          digitalWrite(buzzer,LOW);
        }
        if(actualTime - lightime >= 250)
        {
          digitalWrite(currentPressed,LOW);
          buttonTime = actualTime;
        }
        if(!digitalRead(currentPressed-5) && currentPressed!=0)
          return;
        
        if(!digitalRead(currentPressed) && digitalRead(currentPressed-5))
        {
         currentPressed =0;
        }
          for(int i = 3; i<=6;i++)
          {
            if(!digitalRead(i) && currentPressed != i+5)
            {

              digitalWrite(i+5,HIGH);
              digitalWrite(buzzer,HIGH);
              currentPressed = i+5;
              lightime = actualTime;
              if(playerProgress == numberOfLeds-1 || LEDorder[playerProgress] != currentPressed)
              {
                GameFinished = true;
                GamePlaying = false;
                for(int i = LedPinStart; i<=LedPinEnd;i++)
                {
                  digitalWrite(i, HIGH);
                }
                EndLEDTime = actualTime;
                if(playerProgress == numberOfLeds-1)
                  digitalWrite(winDiode, HIGH);
                else
                  digitalWrite(loseDiode, HIGH);
              } 
              playerProgress++;           
            }
            }
      }
    }
  }
  if(GameFinished)
  {
    if(actualTime - EndLEDTime >= 10)
    {
        digitalWrite(buzzer,LOW);
    }
    if(actualTime - EndLEDTime >= 20)
    {
        digitalWrite(buzzer,HIGH);
    }
    if(actualTime - EndLEDTime >= 500)
    {
      GameFinished = false;
      for(int i = LedPinStart; i<=LedPinEnd;i++)
      {
        digitalWrite(i, LOW);
      }
      digitalWrite(winDiode, LOW);
      digitalWrite(loseDiode, LOW);
    }     
    
  }
  if(IdleMode)
  {

  }
}


void InitGame()
{
    randomSeed(analogRead(A1));
    for(int i=0;i<numberOfLeds;i++)
    {
      LEDorder[i] = random(LedPinStart,LedPinEnd+1);
      while(LEDorder[i] == LEDorder[i-1])
      {
          LEDorder[i] = random(LedPinStart,LedPinEnd+1);
      }
    }
    showLed = true;
    GamePlaying = true;
    index = 0;
    currentTime = actualTime;
    playerProgress = 0;
    digitalWrite(LEDorder[index],HIGH);
}

void StartGame()
{
    if(!GamePlaying && !GameFinished)
      InitGame();
}
