int pawPin = A1;
int pawValue;

int flowerPin = A2;
int flowerValue;

//The baseline: what each sensor reads when nothing is touching it
//setup() fills these in
int flowerBaseline = 0;

int baselineReadings = 30; //how many readings to average

//defining offset from baseline
int mediumOffset = 300;

//defining pressure value for paw pressure sensor- max value is 4095

int pawThreshold = 50;
int pawHighMax = 4095;

//defining values for flower pressure sensor
int flowerMin;
int flowerMax = 4095;


//LED variables - 5 LEDs located 13~15 on feather
int led1Pin = 13;
int led1Value;

int led2Pin = 12;
int led2Value;

int led3Pin = 27;
int led3Value;

int led4Pin = 33;
int led4Value;

int led5Pin = 15;
int led5Value;

unsigned long pawStartTime=0;
//method for setting led brightness
void setAllLEDs(int brightness){
  analogWrite(led1Pin, brightness);
  analogWrite(led2Pin, brightness);
  analogWrite(led3Pin, brightness);
  analogWrite(led4Pin, brightness);
  analogWrite(led5Pin, brightness);
}
//method for updating 
void updateLEDs(){
  analogWrite(led1Pin, led1Value);
  analogWrite(led2Pin, led2Value);;
  analogWrite(led3Pin, led3Value);
  analogWrite(led4Pin, led4Value);
  analogWrite(led5Pin, led5Value);
}

//The brightness for each step (0 - 255)
int ledOff = 0;
int ledLowBrightness = 50;
int ledMediumBrightness = 150;
int ledHighBrightness = 255;


void setup()
{
  Serial.begin(9600);

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);
  pinMode(led5Pin, OUTPUT);

  delay(1000); //a moment to let go of the sensors

  for (int i = 0; i < baselineReadings; i++)
  {
    flowerBaseline = flowerBaseline + analogRead(flowerPin);
    delay(10);
  }

  //divide by the number of readings to get the average
  flowerBaseline = flowerBaseline / baselineReadings;
  flowerMin = flowerBaseline + mediumOffset;

}

void loop()
{
  pawValue = analogRead(pawPin);
  flowerValue = analogRead(flowerPin);

  if (pawValue <= pawThreshold)
  {
    // Start timer when paw first goes below threshold
    if (pawStartTime == 0)
    {
      pawStartTime = millis();
    }
    // Only start breathing after 5 seconds
    if (millis() - pawStartTime >= 5000)
    {
      // Inhale for 4 sec
      for (int brightness = 0; brightness <=255; brightness +=5){
        setAllLEDs(brightness);
        delay(85);
      }
      // Hold for 4 sec
      setAllLEDs(255);
      delay(4000);
      // Exhale for 4 sec
      for (int brightness = 255; brightness >=0; brightness -=5) {
        setAllLEDs(brightness);
        delay(85);
      }
      // Hold for 4 sec
      setAllLEDs(0);
      delay(4000);

      pawStartTime = 0;
    }
  }
  else if (flowerValue >= flowerMin)
  //if the dog is not picked up but someone is sitting on flower, loading circle led sequence
  {
    //Position 1 of circle sequence
    led1Value = ledHighBrightness;
    led2Value = ledMediumBrightness;
    led3Value = ledLowBrightness;
    led4Value = ledOff;
    led5Value = ledLowBrightness;
    updateLEDs();
    delay(250);

    //Position 2
    led1Value = ledLowBrightness;
    led2Value = ledHighBrightness;
    led3Value = ledMediumBrightness;
    led4Value = ledLowBrightness;
    led5Value = ledOff;
    updateLEDs();
    delay(100);

    //Position 3
    led1Value = ledOff;
    led2Value = ledLowBrightness;
    led3Value = ledHighBrightness;
    led4Value = ledMediumBrightness;
    led5Value = ledLowBrightness;
    updateLEDs();
    delay(100);

    //Position 4
    led1Value = ledLowBrightness;
    led2Value = ledOff;
    led3Value = ledLowBrightness;
    led4Value = ledHighBrightness;
    led5Value = ledMediumBrightness;
    updateLEDs();
    delay(100);

    //Position 5
    led1Value = ledMediumBrightness;
    led2Value = ledLowBrightness;
    led3Value = ledOff;
    led4Value = ledLowBrightness;
    led5Value = ledHighBrightness;
    updateLEDs();
    delay(100);
  }
  else
  {
    // Reset timer if dog is put down
    pawStartTime = 0;

    led1Value=ledOff;
    led2Value=ledOff;
    led3Value=ledOff;
    led4Value=ledOff;
    led5Value=ledOff;
    updateLEDs();
  }
  //printing for testing purposes
  Serial.print("Paw Value: ");
  Serial.println(pawValue);

  Serial.print("flower Value: ");
  Serial.println(flowerValue);

  delay(200);
}
