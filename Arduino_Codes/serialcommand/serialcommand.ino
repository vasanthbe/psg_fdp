/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
String command = "";
bool IsStream = false;
unsigned int count = 0;

unsigned long previousMillis = 0; 
unsigned long interval = 100;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(13,OUTPUT);
}

void loop() {
  int cval = 0;
  int stloc = 0;
  int endloc = 0;
  String sval = "";
  String data = "";
  
  unsigned long currentMillis = millis();
  // print the string when a newline arrives:
  if (stringComplete) {
    stloc = inputString.indexOf('<');
    endloc = inputString.indexOf('>',stloc + 1);

    if((stloc >=0) and (endloc >stloc))
    {
      data = inputString.substring(stloc + 1,endloc);
      stloc = 0;
      endloc = data.indexOf(',');
      command = data.substring(stloc,endloc);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  if(command)
  {
    char cmd = command[0];
    switch(cmd)
    {
      case 'l':
        stloc = 0;
        endloc = data.indexOf(':');
        command = data.substring(stloc,endloc);

        stloc = endloc + 1;
        endloc = data.length();
        sval = data.substring(stloc,endloc);
        cval = sval.toInt();

        if(cval == 1)
          digitalWrite(13,HIGH);
        else
          digitalWrite(13,LOW);
        break;
      case 's':
        stloc = 0;
        endloc = data.indexOf(':');
        command = data.substring(stloc,endloc);

        stloc = endloc + 1;
        endloc = data.length();
        sval = data.substring(stloc,endloc);
        cval = sval.toInt();

        if(cval == 1)
        {
          IsStream = true;
        }
        else
          IsStream = false;
        break;
       case 'e':
        stloc = 0;
        endloc = data.indexOf(':');
        command = data.substring(stloc,endloc);

        stloc = endloc + 1;
        endloc = data.length();
        sval = data.substring(stloc,endloc);
        cval = sval.toInt();     
        break;
       case 't':
        stloc = 0;
        endloc = data.indexOf(':');
        command = data.substring(stloc,endloc);

        stloc = endloc + 1;
        endloc = data.length();
        sval = data.substring(stloc,endloc);
        cval = sval.toInt();
        if((cval > 10) and (cval < 10000))
          interval = cval;          
        break;
    }
    command = "";
  }
    if(currentMillis - previousMillis >= interval)
    {
    count++;  
    }
    if((IsStream) and (currentMillis - previousMillis >= interval))
    {
        char sdata[50];
        int slen = 0;
        previousMillis = currentMillis;        
        
        sdata[slen++] = '<';
        sdata[slen++] = '#';
        sdata[slen++] = 'c';
        sdata[slen++] = ':';
        sdata[slen++] = (char)((count >> 8)& 0xFF);
        sdata[slen++] = (char)((count) & 0xFF) ;
        sdata[slen++] = ',';
        sdata[slen++] = '#';
        sdata[slen++] = 'a';
        sdata[slen++] = (char)1;
        sdata[slen++] = ':';
        cval = analogRead(0);
        sdata[slen++] = (char)((cval >> 8) & 0xFF);
        sdata[slen++] = (char)((cval) & 0xFF) ;
        sdata[slen++] = ',';
        sdata[slen++] = '#';
        sdata[slen++] = 'a';
        sdata[slen++] = (char)2;
        sdata[slen++] = ':';
        cval = analogRead(1);
        sdata[slen++] = (char)((cval >> 8) & 0xFF);
        sdata[slen++] = (char)((cval) & 0xFF) ;
        sdata[slen++] = '>';
        sdata[slen++] = '\r';
        sdata[slen++] = '\n';
        Serial.write(sdata,slen);
    }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
