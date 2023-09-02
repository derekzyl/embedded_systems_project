#include <Arduino.h>
#line 1 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"


#include <Key.h>
#include <Keypad.h>
//const int led = 13;
const int openBin = 10;
const int closeBin = 11;
const int binIsOpen = 12;
const int binIsClosed = 13;
const int trigger = A0;
const int echo = A1;
const int passwordLength = 4;
float binLevel;
bool passwordIsCorrect = false;
bool binIsFull = false;
char enteredPassword[4] = {'0','0','0','0'};
const char correctPassword[] = {'2','3','4','5'};
int i = 0;
int j = 0;
long timer = 0;
const int binDiameter = 200;
const int waitTime = 1000;
int increment = 0;
int incrementCount = 2;
bool emptying = false;


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9,8,7,6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5,4,3,2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#line 41 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"
void printPass(char arr[], int i);
#line 50 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"
void binOpenOperation();
#line 68 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"
void binCloseOperation();
#line 85 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"
void checkPassword(char arr[],const char arr1[]);
#line 115 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"
float readBinLevel();
#line 131 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"
int binFull();
#line 159 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"
void setup();
#line 170 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"
void loop();
#line 41 "C:\\Users\\derek\\Documents\\embebbed\\Bin\\Bin.ino"
void printPass(char arr[], int i){
 
  for (int j=0; j<i; j++){
    
    Serial.print(arr[j]);
    
  }
}

void binOpenOperation(){
  if ( passwordIsCorrect ) {
    if ( (digitalRead(binIsOpen) != true) && ( j== 0) ){
      // If password is correct and bin is not fully opened
      emptying = true;
      digitalWrite(openBin,HIGH);
      digitalWrite(closeBin,LOW);
      //Serial.println(digitalRead(openBin));
      //Serial.println("Bin Op");
    }else if (( digitalRead(binIsOpen) == true ) && ( j== 0) ){
      // If bin is fully opened
      digitalWrite(openBin,LOW);
      emptying = false;
      j = 1;
    }
  }
}

void binCloseOperation(){
  if ( binIsFull ){
    if ( (digitalRead(binIsClosed) != true) && ( j == 0 ) ){
      // If password is correct and bin is not fully closed
      digitalWrite(closeBin,HIGH);
      //digitalWrite(openBin,LOW);
      //Serial.println("Bin Cls");
     
    }else{
      // If bin is fully closed
      digitalWrite(closeBin,LOW);
      //Serial.println("not");
    }
  }
  
}

void checkPassword(char arr[],const char arr1[]){
    char key = keypad.getKey();

  if ((key != NO_KEY)&& (i != passwordLength)){
    enteredPassword[i] = key;
    // log key pressed to serial monitor
    Serial.println(key);
    
    i++;
    //Serial.println(i);
  }
  
	// compare the entered password with the actual password
  if ( i == passwordLength ){
   if ( (arr[0] == arr1[0]) &&
       (arr[1] == arr1[1]) &&
       (arr[2] == arr1[2]) &&
       (arr[3] == arr1[3]))
  	{
    	passwordIsCorrect = true;
    	Serial.println("Correct Password");
       
  } else {
    	passwordIsCorrect = false;
    	Serial.println("Wrong Password!");
    	delay(100);
    Serial.println("Try Again");}
    i = 0;
}
}
  float readBinLevel(){
    float distance;
    float duration;
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echo, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    return distance;
  
  }
int binFull(){
    if (millis() - timer >= waitTime){
    if ( readBinLevel() <= binDiameter){
      timer = millis();
      increment++;
      //Serial.println(increment);
    }else { increment = 0; }
     
  }

  return increment;
}
//void blinkWithoutDelay(){
  
  //if ((millis() - timer >= 1000) && (digitalRead(A5) == LOW)){
    //digitalWrite(A5, HIGH);
    //timer = millis();
    //Serial.println(timer);
  //}else if ((millis() - timer >= 1000) && (digitalRead(A5) == HIGH)){
    //digitalWrite(A5, LOW);
    //timer = millis();
     //Serial.println("This111111111111111111111");
  //}
  //return millis();
//}

  

void setup(){
  Serial.begin(9600);
  pinMode(openBin, OUTPUT);
  pinMode(closeBin, OUTPUT);
  pinMode(binIsOpen,INPUT);
  pinMode(binIsClosed, INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(A5, OUTPUT);
}

void loop(){
  
  
  //checkPassword(enteredPassword,correctPassword);
  //binOpenOperation();
  //binCloseOperation();
  //Serial.println(binFull() >= incrementCount);
  //binFull();
  if (keypad.getKey() == '#') {
    j = 0; 
    binIsFull = false;
    passwordIsCorrect = false;
    Serial.println("Bal");
  }
  if ( (binFull() >= incrementCount) && (binIsFull == false) ) {
    binIsFull = true;
    Serial.println("Balablu");
  } //else { binIsFull = false; }
  
  if ( binIsFull && (emptying == false) && ( j == 0 ) ){
  	binCloseOperation();
  	checkPassword(enteredPassword,correctPassword);
  }
  if ( passwordIsCorrect ){
    binOpenOperation();
  }
  

}

