void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial.write("1");
  Serial.write(65);
  Serial.write("\r\n");
  delay(500);
}
