const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

float sensorValue = 0;        // value read from the pot
float outputValue = 0;      // value output to the PWM (analog out)
float tempValue = 0;
float aveValue = 0;
float viscosity = 0.20;     // average weight
float mapValue = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  //mapValue = map(sensorValue, 508, 427, 1.35,5.4);
  // print the results to the serial monitor:
  Serial.print("sensor = " );
  Serial.print(sensorValue);
  
  // convert to inches
  if (sensorValue == 508) {
    outputValue = 1;
    Serial.println("\t output is less than 1.35 inches");
  }  
  else {
    aveValue = 0;
    for (int i = 0; i < 100; i++) {
      sensorValue = analogRead(analogInPin);
      //tempValue = -0.0519*sensorValue + 27.48;
      tempValue = -0.00009*sensorValue*sensorValue + 0.0268*sensorValue + 10.325;
      aveValue += tempValue;
      delay(2);
    }
    aveValue = aveValue/100;
    outputValue = (1-viscosity)*outputValue + viscosity*aveValue;
    Serial.print("\t output = ");
    printFloat(outputValue,2); 
    Serial.println(" inches");
  }
  
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);
}

// printFloat prints out the float 'value' rounded to 'places' places after the decimal point
void printFloat(float value, int places) {
  // this is used to cast digits 
  int digit;
  float tens = 0.1;
  int tenscount = 0;
  int i;
  float tempfloat = value;

    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
  // if this rounding step isn't here, the value  54.321 prints as 54.3209

  // calculate rounding term d:   0.5/pow(10,places)  
  float d = 0.5;
  if (value < 0)
    d *= -1.0;
  // divide by ten for each decimal place
  for (i = 0; i < places; i++)
    d/= 10.0;    
  // this small addition, combined with truncation will round our values properly 
  tempfloat +=  d;

  // first get value tens to be the large power of ten less than value
  // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

  if (value < 0)
    tempfloat *= -1.0;
  while ((tens * 10.0) <= tempfloat) {
    tens *= 10.0;
    tenscount += 1;
  }


  // write out the negative if needed
  if (value < 0)
    Serial.print('-');

  if (tenscount == 0)
    Serial.print(0, DEC);

  for (i=0; i< tenscount; i++) {
    digit = (int) (tempfloat/tens);
    Serial.print(digit, DEC);
    tempfloat = tempfloat - ((float)digit * tens);
    tens /= 10.0;
  }

  // if no places after decimal, stop now and return
  if (places <= 0)
    return;

  // otherwise, write the point and continue on
  Serial.print('.');  

  // now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
  for (i = 0; i < places; i++) {
    tempfloat *= 10.0; 
    digit = (int) tempfloat;
    Serial.print(digit,DEC);  
    // once written, subtract off that digit
    tempfloat = tempfloat - (float) digit; 
  }
}


