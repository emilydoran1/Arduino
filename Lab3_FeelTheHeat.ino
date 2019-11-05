const int sensorPin = A0;
const float lowerLimit = 22.27;
const float upperLimit = 26.52;
const float limitDifference = upperLimit - lowerLimit;
const float lightVal = (255 / limitDifference);
float tempDifference;

const int greenLEDPin = 9;
const int redLEDPin = 10;
const int blueLEDPin = 11;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

int temperatures[10];
int normalizedSensor = 0;
int currentIndex = 0;
int total = 0;

void setup() {
   Serial.begin(9600);
   for(int pinNumber = 2; pinNumber < 5; pinNumber++){
     pinMode(pinNumber, OUTPUT);
     digitalWrite(pinNumber, LOW);
   }
   
   pinMode(greenLEDPin, OUTPUT);
   pinMode(redLEDPin, OUTPUT);
   pinMode(blueLEDPin, OUTPUT);
   
   Serial.print("Limit Difference: ");
   Serial.println(limitDifference);
   Serial.print("Light Val: ");
   Serial.println(lightVal);
   Serial.println();
}

void loop() {
   int sensorVal = analogRead(sensorPin);
   Serial.print("Sensor Value: ");
   Serial.print(sensorVal);
   
   total = total - temperatures[currentIndex];
   temperatures[currentIndex] = sensorVal;
   total += sensorVal;
   
   currentIndex++;
   
   if (currentIndex >= 10) {
    currentIndex = 0;
   }
   normalizedSensor = total/10;
   Serial.print(", Normalized Sensor Value: ");
   Serial.print(normalizedSensor);
   
   float voltage = (normalizedSensor/1024.0) * 5.0;
   Serial.print(", Volts: ");
   Serial.print(voltage);
   Serial.print(", Degrees C: ");
   
   float temperature = (voltage - .5) * 100;
   Serial.print(temperature);
   
   analogWrite(redLEDPin, redValue);
   analogWrite(greenLEDPin, greenValue);
   analogWrite(blueLEDPin, blueValue);
   
   tempDifference = temperature - lowerLimit;
   Serial.print(", Over Threshold: ");
   Serial.println(tempDifference);
   
   blueValue = 255 - (tempDifference * lightVal);
   redValue = 255 - blueValue;
   
   if(temperature > upperLimit){
     blueValue = 0;
     redValue = 255;
   }
   else if(temperature < lowerLimit){
     blueValue = 255;
     redValue = 0;
   }
   
   analogWrite(redLEDPin, redValue);
   analogWrite(greenLEDPin, greenValue);
   analogWrite(blueLEDPin, blueValue);

}
