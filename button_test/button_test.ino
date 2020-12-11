

const uint8_t previous = 14;
const uint8_t play = 16;
const uint8_t next = 10;
uint8_t values;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(previous,INPUT);
  pinMode(play,INPUT);
  pinMode(next,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  bitWrite(values, 0, digitalRead(previous));
  bitWrite(values, 1, digitalRead(play));
  bitWrite(values, 2, digitalRead(next));
  Serial.print("Previous, Play, Next = ");
  Serial.println(values,BIN);
  delay(100);
  
}
