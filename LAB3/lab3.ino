int redled=11;// initialize pin 11
int greenled=9;// initialize pin 9
int inpin=7;// initialize pin 7
int val;// define val
2 | Page
int seconds = 1000;// defines seconds for delay
int x =1;// loop incrementer
void setup()
{
Serial.begin(9600);// gives us the ability to print stuff in the serial monitor
pinMode(redled,OUTPUT);// set red led pin as “output”
pinMode(greenled, OUTPUT);// set green led pin as "output"
pinMode(inpin,INPUT);// set button pin as “input”
}
void loop()
{
val=digitalRead(inpin);// read the level value of pin 7 and assign if to val
if(val==LOW)// check if the button is pressed, if yes, turn on the LED
{ // turns off the leds after the button is pressed
digitalWrite(greenled,LOW);
digitalWrite(redled, LOW);
}
else
{ // goes through the incremental procress of increasing the duration of the led being
lit
digitalWrite(redled,HIGH);// turns on the red led for one second
digitalWrite(greenled, LOW);
delay(x * seconds);// delay the time of the red led being turned off
digitalWrite(greenled,HIGH);// turns on the green led for one second
digitalWrite(redled, LOW);
delay(x * seconds);// delay the time of the green led being turned off
digitalWrite(greenled,LOW);// turns off both leds
digitalWrite(redled, LOW);
x++;// increases the increment
Serial.print(x-1);// posts the value of the duration of the led being lit
}
if(x==6){ // resets the increment back to 1 after the button has been pressed for 5
times
x=1;
}
}
