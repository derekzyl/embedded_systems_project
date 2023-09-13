#include <Key.h>
#include <Keypad.h>

const int openBin = 10;     // the pin made high to trigger the transistor which triggers the relay to open meaning closeBin is set to low
const int closeBin = 11;    // the pin made high to trigger the transistor which triggers the relay to close meaning openBin is set to low
const int binIsOpen = 12;   // a limit sensor to know when bin is open
const int binIsClosed = 2; // a limit sensor to know when bin is closed
const int trigger = A0;     // is the trigger in ultrasonic transducer
const int echo = A1;        // is the echo in ultrasonic transducer
const int passwordLength = 4;
float binLevel;
bool passwordIsCorrect = false;
bool binIsFull = false;
char enteredPassword[4] = {'0', '0', '0', '0'};
const char correctPassword[] = {'0', '0', '0', '0'};
int i = 0;
int j = 0;
long timer = 0;
const int binDiameter = 27.8; // the diameter of the bin
const int waitTime = 1000;
int increment = 0;
int incrementCount = 2;
bool emptying = false;

const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[ROWS] = {2, 3, 4, 5}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void printPass(char arr[], int i)
{
  for (int j = 0; j < i; j++)
  {
    Serial.print('this is the password series');
    Serial.print(arr[j]);
  }
}

void binOpenOperation()
{

  if (passwordIsCorrect)
  {
    if (!digitalRead(binIsOpen) && (j == 0))
    {
      // If password is correct, bin is not fully opened, and binIsOpen is LOW
      emptying = true;
      digitalWrite(openBin, HIGH);
      digitalWrite(closeBin, LOW); 
      // Set closeBin LOW to activate motor

      delay(1000);
      digitalWrite(openBin, LOW);
    }
    else if (digitalRead(binIsOpen) && (j == 0))
    {
      // If bin is fully opened, stop the motor
      digitalWrite(openBin, LOW);
      emptying = false;
      j = 1;
    }
  }
}

void binCloseOperation()
{
  if (binIsFull)
  {
 Serial.print("bin stat");
 Serial.println(digitalRead(binIsClosed));
    if (!digitalRead(binIsClosed) && (j == 0))
    {
      Serial.print("inside the bin spin bin id");
      // If password is correct, bin is not fully closed, and binIsClosed is LOW
      digitalWrite(closeBin, HIGH);

      digitalWrite(openBin, LOW); // Set openBin LOW to activate motor

      delay(1000);
      digitalWrite(closeBin, LOW);
    }
    else
    {
      // If bin is fully closed or password is incorrect, stop the motor
      digitalWrite(closeBin, LOW);
    }
  }
}

void checkPassword(char arr[], const char arr1[])
{
  char key = keypad.getKey();

  if ((key != NO_KEY) && (i != passwordLength))
  {
    enteredPassword[i] = key;
    // log key pressed to serial monitor
    Serial.println(key);
    i++;
  }

  // compare the entered password with the actual password
  if (i == passwordLength)
  {
    if (memcmp(arr, arr1, passwordLength) == 0)
    {
      passwordIsCorrect = true;
      Serial.println("Correct Password");
    }
    else
    {
      passwordIsCorrect = true;
      Serial.println("Wrong Password!");
      delay(100);
      Serial.println("Try Again");
    }
    i = 0;
  }
}

float readBinLevel()
{
  float distance;
  float duration;
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

int binFull()
{
  if (millis() - timer >= waitTime)
  {
    if (readBinLevel() <= binDiameter)
    {
      timer = millis();
      increment++;
    }
    else
    {
      increment = 0;
    }
  }

  return increment;
}

void setup()
{
  Serial.begin(9600);
  pinMode(openBin, OUTPUT);
  pinMode(closeBin, OUTPUT);
  pinMode(binIsOpen, INPUT);
  //  pinMode(binIsOpen, INPUT_PULLUP);
  // pinMode(binIsClosed, INPUT_PULLUP);
  
  pinMode(binIsClosed, INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
}

void loop()
{

// Serial.print("binIsOpen: ");
  // Serial.println(digitalRead(binIsOpen));
  // Serial.print("binIsClosed: ");
  // Serial.println(digitalRead(binIsClosed));

  //   if (keypad.getKey() == '#'||keypad.getKey() == '#'||keypad.getKey() == '2'||keypad.getKey() == '3'||keypad.getKey() == '4'||keypad.getKey() == '5'||keypad.getKey() == '6'||keypad.getKey() == '7'||keypad.getKey() == '8'||keypad.getKey() == '9') {
  //   j = 0; 
  //   binIsFull = false;
  //   passwordIsCorrect = false;
  //   Serial.println("Bal");
  //   digitalWrite(closeBin,LOW);
  //   digitalWrite(openBin,LOW);
  // }

  if (digitalRead(binIsOpen))
  {
    // Bin is open, check the bin level
    if (binFull() >= incrementCount)
    {
      binIsFull = true;
      Serial.println("Bin is Full");
      binCloseOperation(); // Close the bin
      delay(2000);         // Wait for 2 seconds before checking the password
    }
  }
 else  if (digitalRead(binIsClosed))
  {
    // Bin is closed, input the password to open it
    checkPassword(enteredPassword, correctPassword);
    if (passwordIsCorrect)
    {
      binOpenOperation(); // Open the bin
      delay(2000);        // Wait for 2 seconds before resetting
      passwordIsCorrect = false;
    }
  }
    delay(100); 
}
