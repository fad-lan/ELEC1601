#include <Servo.h>

Servo myservo1;
Servo myservo2;
const int irLedMid = 6, irReceiverMid = 7; // Select these to match the IR LED/receiver pair that you are using
const int irLedLeft = 10, irReceiverLeft = 11;
const int irLedRight = 2, irReceiverRight = 3;
const int redLedRight = A0;
const int redLedLeft = A2;
const int redLedMid = A1; // Select this to match the red LED next to the IR receiver you are using
int irValRight;
int irValLeft;
int irValMid;

void setup()
{
    myservo1.attach(12);
    myservo2.attach(13);
    pinMode(irReceiverMid, INPUT);
    pinMode(irLedMid, OUTPUT);
    pinMode(redLedMid, OUTPUT);
    pinMode(irReceiverLeft, INPUT);
    pinMode(irLedLeft, OUTPUT);
    pinMode(redLedLeft, OUTPUT);
    pinMode(irReceiverRight, INPUT);
    pinMode(irLedRight, OUTPUT);
    pinMode(redLedRight, OUTPUT); // Red LED pin is an output
    Serial.begin(9600);
    delay(1000);
}

void loop()
{
    irValMid = irDistanceMid(irLedMid, irReceiverMid);
    irValLeft = irDistanceLeft(irLedLeft, irReceiverLeft);
    irValRight = irDistanceRight(irLedRight, irReceiverRight);
    if (irValMid < 2 && irValLeft < 2 && irValRight < 2)
    {
        myservo1.writeMicroseconds(1500);
        myservo2.writeMicroseconds(1500);
        delay(1000);
        TurnLeft();
        delay(200);
        TurnLeft();
        digitalWrite(redLedMid, HIGH);
        digitalWrite(redLedLeft, HIGH);
        digitalWrite(redLedRight, HIGH);
    }
    if (irValMid > 3) // Optional - display detection by setting red LED high
    {
        if (irValRight < 1)
        {
            digitalWrite(redLedMid, LOW);
            digitalWrite(redLedLeft, HIGH);
            digitalWrite(redLedRight, LOW);
            LeftForward();
        }
        else if (irValLeft < 1)
        {
            digitalWrite(redLedMid, LOW);
            digitalWrite(redLedLeft, LOW);
            digitalWrite(redLedRight, HIGH);
            RightForward();
        }
        else
        {
            MoveForward();
            digitalWrite(redLedMid, LOW);
            digitalWrite(redLedLeft, LOW);
            digitalWrite(redLedRight, LOW);
        }
    }
    else if (irValLeft > 1) // Optional - display detection by setting red LED high
    {
        MoveForward();
        delay(100);
        TurnLeft();
        digitalWrite(redLedLeft, LOW);
        digitalWrite(redLedRight, LOW);
        digitalWrite(redLedMid, HIGH);
    }
    else if (irValRight > 1) // Optional - display detection by setting red LED high
    {
        MoveForward();
        delay(100);
        TurnRight();
        digitalWrite(redLedRight, LOW);
        digitalWrite(redLedLeft, LOW);
        digitalWrite(redLedMid, HIGH);
    }
    // 0.5 second delay - just long enough to see the LED blink
}

void MoveReverse()
{
    myservo1.writeMicroseconds(1590);
    myservo2.writeMicroseconds(1400);
    delay(3000);
    myservo1.writeMicroseconds(1500);
    myservo2.writeMicroseconds(1500);
    delay(1000);
}

void MoveForward()
{
    myservo1.writeMicroseconds(1410);
    myservo2.writeMicroseconds(1590);
}
void RightForward()
{
    myservo1.writeMicroseconds(1450);
    myservo2.writeMicroseconds(1660);
}

void LeftForward()
{
    myservo1.writeMicroseconds(1340);
    myservo2.writeMicroseconds(1540);
}

void TurnRight()
{
    myservo1.writeMicroseconds(1500);
    myservo2.writeMicroseconds(1500);
    myservo1.writeMicroseconds(1600);
    myservo2.writeMicroseconds(1580);
    delay(580);
}

void TurnLeft()
{
    myservo1.writeMicroseconds(1500);
    myservo2.writeMicroseconds(1500);
    myservo1.writeMicroseconds(1400);
    myservo2.writeMicroseconds(1380);
    delay(510);
}

int irDetectMid(long frequency)
{
    tone(irLedMid, frequency);           // Turn on the IR LED square wave
    delay(1);                            // Wait 1 ms
    int ir = digitalRead(irReceiverMid); // IR receiver -> ir variable
    noTone(irLedMid);                    // Turn off the IR LED
    delay(1);                            // Down time before recheck
    return ir;                           // Return 0 detect, 1 no detect
}

int irDetectLeft(long frequency)
{
    tone(irLedLeft, frequency);           // Turn on the IR LED square wave
    delay(1);                             // Wait 1 ms
    int ir = digitalRead(irReceiverLeft); // IR receiver -> ir variable
    noTone(irLedLeft);                    // Turn off the IR LED
    delay(1);                             // Down time before recheck
    return ir;                            // Return 0 detect, 1 no detect
}

int irDetectRight(long frequency)
{
    tone(irLedRight, frequency);           // Turn on the IR LED square wave
    delay(1);                              // Wait 1 ms
    int ir = digitalRead(irReceiverRight); // IR receiver -> ir variable
    noTone(irLedRight);                    // Turn off the IR LED
    delay(1);                              // Down time before recheck
    return ir;                             // Return 0 detect, 1 no detect
}

int irDistanceMid(int irLedMid, int irReceiverMid)
{
    int distance = 0;
    for (long f = 39000; f <= 42000; f += 1000)
    {
        distance += irDetectMid(f);
    }
    return distance + 1;
}
int irDistanceLeft(int irLedLeft, int irReceiverLeft)
{
    int distance = 0;
    for (long f = 38000; f <= 42000; f += 1000)
    {
        distance += irDetectLeft(f);
    }
    return distance;
}
int irDistanceRight(int irLedRight, int irReceiverRight)
{
    int distance = 0;
    for (long f = 37000; f <= 42000; f += 1000)
    {
        distance += irDetectRight(f);
    }
    return distance - 1;
}
