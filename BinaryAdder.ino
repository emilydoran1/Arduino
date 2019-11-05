// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//set the switch states
int switchState1 = 0;
int switchState2 = 0;
byte prevState1 = HIGH;
byte prevState2 = HIGH;
boolean toggle1 = false;
boolean toggle2 = false;
int cursor = 0;

//save the values from the lcd
int byte1[8];
int byte2[8];

//variable to store sum in
int sumDecimal = 0;

//store the bytes in decimal
int byte1Decimal;
int byte2Decimal;

//declare the contrast variable
int contrast = 20;

//sets current screen to first screen
int currentScreen = 1;

void setup() {
  Serial.begin(9600);
  
  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);

  //initialize the contrast of LCD
  pinMode(6, INPUT); 
  analogWrite(6, contrast); 
  
  //Switch1
  pinMode(10, INPUT);
  //Switch2
  pinMode(8, INPUT);
}

void loop(){
  switchState1 = digitalRead(10);
  switchState2 = digitalRead(8);
  
  //if switch1 AND switch2 are pressed on first screen
  if(currentScreen == 1){
    // Print a message to the LCD.
    lcd.setCursor(0, 0);
    lcd.print("Press 2 Buttons");
    lcd.setCursor(0, 1);
    lcd.print("to Continue!");
    if(switchState1 == HIGH && switchState2 == HIGH){
      currentScreen++;
      firstByte();
    }
  }
  
  //2nd screen -- enter the first byte
  else if(currentScreen == 2 && cursor < 8){
    
    //toggle bit between 0 and 1
    if(switchState2 != prevState2){
      if(switchState2 == HIGH){
        toggle1 = !toggle1;
        lcd.setCursor(cursor, 1);
        lcd.print(toggle1);
        byte1[cursor] = toggle1;
      }
      //set previous switch State to current
      prevState2 = switchState2;
    } 
    
    //go to the next bit
    else if(switchState1 != prevState1){
      if(switchState1 == HIGH){
        cursor++;
        if(byte1[cursor-1] == 1){
          toggle1 = !toggle1;
          prevState1 = HIGH;
        }
        //if you get to end of byte, increase screen
        if(cursor == 8){
          prevState1 = HIGH'
          currentScreen = 3;
          secondByte();
          cursor = 0;
        }
      }
      prevState1 = switchState1;
    } 
  }
  
  //3rd screen-- enter the second byte
  else if(currentScreen == 3 && cursor < 8){
    
    //toggle bit between 0 and 1
    if(switchState2 != prevState2){
      if(switchState2 == HIGH){
        toggle2 = !toggle2;
        lcd.setCursor(cursor, 1);
        lcd.print(toggle2);
        byte2[cursor] = toggle2;
      }
      //set previous switch State to current
      prevState2 = switchState2;
    } 
    
    //go to the next bit
    else if(switchState1 != prevState1){
      // Serial.println("test");
      if(switchState1 == HIGH){
        cursor++;
        if(byte2[cursor-1] == 1){
          toggle2 = !toggle2;
        }
        //if you get to end of byte, increase screen
        if(cursor == 8){
          currentScreen = 4;
          sum();
        }
      }
      prevState1 = switchState1;
    } 
  }
  
  //4th screen -- show sum
  else if(currentScreen == 4){
    if(switchState1 == HIGH && switchState2 == HIGH){
      delay(1000);
      currentScreen = 5;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Use buttons to");
      lcd.setCursor(0,1);
      lcd.print("Adjust Contrast");
      prevState1 = switchState1;
      prevState2 = switchState2;
    }
    
  }
  
  //5th screen -- control contrast
  else if(currentScreen == 5){
    
    //turn brightness down
    if(switchState2 != prevState2){
      if(switchState2 == HIGH){
        contrast -= 5;
        if(contrast <= 0){
          analogWrite(6, 0); 
          contrast = 0;
        }
        else{
          analogWrite(6, contrast); 
        }
      }
      //set previous switch State to current
      prevState2 = switchState2;
    } 
    
    //turn brightness up
    else if(switchState1 != prevState1){
      if(switchState1 == HIGH){
        contrast += 5;
        if(contrast >=128){
          analogWrite(6, 128); 
          contrast = 128;
        }
        else{
          analogWrite(6, contrast); 
        }
      }
      //set previous switch State to current
      prevState1 = switchState1;
    } 
    
    //move back to first screen
    if(switchState1 == HIGH && switchState2 == HIGH){
      reset();
    }
  }
}

//function to set the first byte of numbers
void firstByte(){
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Byte 1:");
  lcd.setCursor(0, 1);
  lcd.print("00000000");
}

//function to set the second byte of numbers
void secondByte(){
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Byte 2:");
  lcd.setCursor(0, 1);
  lcd.print("00000000");
}

//get the sum of the two bytes and print to lcd
void sum(){
  sumDecimal = 0;
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Sum:");
  lcd.setCursor(0, 1);
  
  convertByte1Decimal(byte1);
  convertByte2Decimal(byte2);
  
  sumDecimal = byte1Decimal+byte2Decimal;
  
  //if the sum needs additional bit
  if(sumDecimal >= 255){
    int sumBinary[9];
    int i = 8;
    while(sumDecimal > 0){
      sumBinary[i] = sumDecimal % 2;
      sumDecimal = sumDecimal/2;
      i--;
    }
    
    //print to lcd
    for(int i = 0; i < 9; i++){
      lcd.print(sumBinary[i]);
    }
  }
  
  //if the sum is 0 print all zeros
  else if(sumDecimal == 0){
    int sumBinary[8] = {0,0,0,0,0,0,0,0};
    
    //print to lcd
    for(int i = 0; i < 8; i++){
      lcd.print(sumBinary[i]);
    }
  }
  //sum does not need additional bit
  else{
    int sumBinary[8];
    int i = 0;
    while(i < 8){
      if(sumDecimal <= 0){
        sumBinary[i] = 0;
      }
      else{
        sumBinary[i] = sumDecimal % 2;
        // Serial.println(sumBinary[i]);
        sumDecimal = sumDecimal/2;
      }
      i++;
    }
    
    //print to lcd
    for(int i = 7; i >= 0; i--){
      lcd.print(sumBinary[i]);
    }
  }

}

//convert first byte from binary to decimal
void convertByte1Decimal(int myByte[]){
  int weight = 1;
  for(int j = 7; j >=0; j--){
    byte1Decimal += weight* myByte[j];
    weight *= 2;
  }
  // Serial.println(byte1Decimal);
}

//convert second byte from binary to decimal
void convertByte2Decimal(int myByte[]){
  int weight = 1;
  for(int j = 7; j >=0; j--){
    byte2Decimal += weight* myByte[j];
    weight *= 2;
  }
  // Serial.println(byte2Decimal);
}

//reset all of the values and delay before getting back to start sc
void reset(){
  cursor = 0;
  byte1Decimal = 0;
  byte2Decimal = 0;
  sumDecimal = 0;
  prevState1 = !HIGH;
  prevState2 = !HIGH;
  lcd.clear();
  currentScreen = 1;
  delay(1000);
}
