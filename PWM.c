// Analog 
int RawVoltagePin0 = 0;
int RawVoltagePin1 = 0;
float VoltagePin0 = 0;
float VoltagePin1 = 0;

// Analog Pins for Raw Voltage and Current
int inputValue00 = A0;
int inputValue01 = A1;

// Info for Input
int incomingByte = 0;
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;
float dataNumber = 0;   

// PWM
int PWMPin = 9;
float targetVoltage = 10;


void setup() {
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
  Serial.println("Defaulting to 8 Volts");
  Serial.println("Type 2 to increase by 0.5 :: Type 1 to decrease by 0.5 :: Type 0 to display voltages");
  analogWrite(PWMPin, 127);
}

void loop() {
  checkForInput();
  showNewNumber();
}

void Write(float targetVoltage){
  int outValue = 127;
  
  if(targetVoltage == 8)
    outValue = 0;
  else if(targetVoltage == 8.5)
    outValue = 31;
  else if(targetVoltage == 9)
    outValue = 63;
  else if(targetVoltage == 9.5)
    outValue = 95;
  else if(targetVoltage == 10)
    outValue = 127;
  else if(targetVoltage == 10.5)
    outValue = 159;
  else if(targetVoltage == 11)
    outValue = 191;
  else if(targetVoltage == 11.5)
    outValue = 223;
  else
    outValue = 255;

  analogWrite(PWMPin, outValue);
}

void computeAverageValues()
{
    float VoltageCountPin0,VoltageCountPin1 = 0.0;
    
    for(int i = 0 ; i < 100 ; i ++)
    {
      // Read Voltage on Wiper
      RawVoltagePin0 = analogRead(inputValue00);
      RawVoltagePin1 = analogRead(inputValue01);
      
      VoltagePin0 = (RawVoltagePin0 * 5.0 )/ 1024.0;
      VoltagePin1 = (RawVoltagePin1 * 5.0 )/ 1024.0;
      
      VoltageCountPin0 = VoltageCountPin0+VoltagePin0;
      VoltageCountPin1 = VoltageCountPin1+VoltagePin1;
    }
    VoltageCountPin0 = VoltageCountPin0/100;
    VoltageCountPin1 = VoltageCountPin1/100;

    //VoltageControl(VoltageCountPin0, VoltageCountPin1);
    Print(VoltageCountPin0, VoltageCountPin1);
}

void VoltageControl(float param1, float param2)
{
  // compare expected to incoming
  
}

void Print(float param1, float param2)
{
    Serial.print("\t Voltage0 = ");
    Serial.println(param1,3);
    Serial.print("\t Voltage1 = ");
    Serial.println(param2,3);
    delay(1000);
}

void checkForInput() 
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  
  if (Serial.available() > 0) 
  {
    rc = Serial.read();

    if (rc != endMarker) 
    {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) 
      {
        ndx = numChars - 1;
      }
    }
    else 
    {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewNumber() {
  if (newData == true) 
  {
    dataNumber = 0;
    dataNumber = atoi(receivedChars);
    Serial.print("Selected: ");
    Serial.println(dataNumber);
    newData = false;
    handleInput(dataNumber);
    computeAverageValues();
  }
}

void handleInput(int myDataNumber){
  if(myDataNumber == 0) {
    computeAverageValues();
  }
  else if(myDataNumber == 1){
    decrementTarget();
  }
  else if(myDataNumber ==2){
    incrementTarget();
  }
}


void decrementTarget(){
  if(targetVoltage <= 8.0)
    targetVoltage = 8.0;
  else
    targetVoltage = targetVoltage - 0.5;

  Serial.print("Current Target: ");
  Serial.println(targetVoltage);
  
  Write(targetVoltage);
}


void incrementTarget(){
  if(targetVoltage >= 12.0)
    targetVoltage = 12.0;
  else
    targetVoltage = targetVoltage + 0.5;

  Serial.print("Current Target: ");
  Serial.println(targetVoltage);
  
  Write(targetVoltage);
}
