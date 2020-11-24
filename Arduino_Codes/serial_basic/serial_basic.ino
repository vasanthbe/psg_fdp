
unsigned long previousMillis = 0; 
unsigned long interval = 500;
char count = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:
    unsigned long currentMillis = millis();
    
    if((currentMillis - previousMillis >= interval))
    {
        count ++;
        char sdata[50];
        int slen = 0;
        previousMillis = currentMillis;        
        
        sdata[slen++] = '<';
        sdata[slen++] = '#';
        sdata[slen++] = 'c';
        sdata[slen++] = ':';
        sdata[slen++] = count;
        sdata[slen++] = '>';
        sdata[slen++] = '\r';
        sdata[slen++] = '\n';
        Serial.write(sdata,slen);
    }
}
