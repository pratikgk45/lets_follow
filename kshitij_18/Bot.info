/*
  #####~~~~ LETS FOLLOW ~~~~#####
  USes HMC5883L library
  $$$COMPILE TIME MAGIC$$$
*/
#include <QTRSensors.h>
#include <DRV8833.h>

#include "Graph.h"
#include "Bot.h"

#define Debug(A) A
#define DEBUG(A) Serial.print(A)
#define AutoCalib(A) A
#define AutoQTR(A)
#define I !

DRV8833 driver = DRV8833();
QTRSensorsRC qtrrc((unsigned char[]) {
  A0, A1, A2, A3, A4, A5
}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);

// Random numbers required in some algorithms
unsigned long get_random()
{
  return millis();
}

// Turn Left
void leftturn()
{
  //DEBUG("left");DEBUG("\n");
  driver.motorBForward(70);
  driver.motorAReverse(70);
  delay(TURN_DELAY);
}

// Turn Right
void rightturn()
{
  //DEBUG("Right");DEBUG("\n");
  driver.motorBReverse(70);
  driver.motorAForward(70);
  delay(TURN_DELAY);
}

// Make a U Turn
void uturn()
{
  //DEBUG("uturn"); DEBUG("\n");
  driver.motorBForward(70);
  driver.motorAReverse(90);
}

// Glow an led on the bot. Can tur it on or off
void Bot:: glow (bool x)
{
  digitalWrite(13, x?HIGH:LOW);
}

// Read the sensor value on the bot corresponding to position x
bool Bot :: Sensor(int x)
{
  if (x == 0)return I digitalRead(2);
  if (x == 7)return I digitalRead(3);
  if (x == ENCODER)  return I digitalRead(ENCODER);
  return I (sensors[x - 1] < 400);
}

// Check input of a button on the bot
bool Bot:: get_button()
{
  return digitalRead(BUTTON);
}

// Read Sensor Values and adjust found accordingly
void Bot:: read_sensors()
{
  // DEBUG("ReadSensor");DEBUG("\n")
  qtrc_pos = qtrrc.readLine(sensors);
  AutoQTR
  (
    for (int i = 0; i < 8; i++)
    {
      DEBUG(Sensor(i)); DEBUG('\t');
    }
    ENDL;
  )
  found[Right] = Sensor(0) && Sensor(1) ;
  found[Left] = Sensor(7) && Sensor(6);
  found[Forward] = Sensor(4) && Sensor(3) || Sensor(4) && Sensor(5) || Sensor(2) && Sensor(3);
}

bool Bot::has_forward()
{
  //DEBUG("Has Forward\n")
  long current_time = 0;
  bool r;
  bool r_pre = 1;
  unsigned long goal_time=millis();
  //attachInterrupt(digitalPinToInterrupt(ENCODER), encoder, RISING);
  forward();
  while (found[Left] || found[Right])
  {
    read_sensors();
    ways[Left] = found[Left] || ways[Left];
    ways[Right] = found[Right] || ways[Right];
    r = Sensor(ENCODER);
    if (r == 0 && r_pre == 1) current_time++;
    r_pre = r;

    if (millis()-goal_time>2000) {
      at_goal = true;
      for (int w=0 ;w<4;w++){
        ways[w]=0;
      }
      return false;
    }
  }
  //detachInterrupt(digitalPinToInterrupt(ENCODER));
  delay(FORWARD_DELAY);
  read_sensors();
  bool x = 0;
  for (int h = 1; h < 6; h++)
  {
    x = x | Sensor(h);
  }
  return x;
}

bool Bot:: has_node()
{
  //DEBUG("Has node");
  read_sensors();
  ways[Left] = found[Left];
  ways[Right] = found[Right];
  ways[Forward] = found[Forward];

  if (found[Right] || found[Left])
  {
    forward();
    delay(5);
    read_sensors();
    ways[Left] = found[Left] || ways[Left];
    ways[Right] = found[Right] || ways[Right];
    delay(5);
    read_sensors();
    ways[Left] = found[Left] || ways[Left];
    ways[Forward] = has_forward();
    return true;
  }
  if (!Sensor(0) && !Sensor(1) && !Sensor(2) && !Sensor(3) && !Sensor(4) && !Sensor(5) && !Sensor(6) && !Sensor(7))
  {
    return true;
  }
  return false;
}

void Bot::pid(int normal_speed)
{
  int error, motorspeed, lms, rms;
  error = qtrc_pos - 2500;
  motorspeed = KP * error + KD * (error - lasterror);
  lasterror = error ;
  lms = normal_speed + motorspeed;
  rms = normal_speed - motorspeed;
  if (lms > MAX_SPEED)
    lms = MAX_SPEED;
  if (lms < 0)
    lms = 0;
  if (rms > MAX_SPEED)
    rms = MAX_SPEED;
  if (rms < 0)
    rms = 0;
  driver.motorBForward(lms);
  driver.motorAForward(rms);
}

void Bot::stop()
{
  driver.motorBStop();
  driver.motorAStop();
}

void Bot::follow_segment()
{
  DEBUG("Follow  at ");
  long current_time = 0;
  bool r;
  bool r_pre = 1;
  int unit1 = pot_units / 50;
  //attachInterrupt(digitalPinToInterrupt(ENCODER), encoder, RISING);
  while (!has_node())
  {
    //    DEBUG("_IN_\n/");
    r = Sensor(ENCODER);
    if (r == 0 && r_pre == 1) current_time++;
    r_pre = r;
    pid(NORMAL_SPEED);
  }
  for ( int k = 0; k < 4; k++)  DEBUG(ways[k]); ENDL;
  DEBUG("current_time : ");
  calibrate_unit = current_time;
  DEBUG(current_time); DEBUG("\n");
  if (current_time <= unit1)
    current_time = 1;
  else if (current_time <= unit1 * 2 + unit1 / 2)
    current_time = 2;
  else if (current_time <= unit1 * 3 + unit1)
    current_time = 3;
  else current_time = 4;

  switch (disha)
  {
    case North:
      pos[1] += current_time;
      break;
    case East:
      pos[0] += current_time;
      break;
    case South:
      pos[1] -= current_time;
      break;
    case West:
      pos[0] -= current_time;
      break;
  }
  DEBUG(pos[0]); DEBUG(" "); DEBUG(pos[1]); DEBUG("\n");
}


void Bot::turn(Direction d)
{
  disha = (d + disha) % 4;
  DEBUG("Turning "); DEBUGDIR(d); DEBUG(" to Disha "); DEBUGDIS(disha); DEBUG("\n");
  int pro_turn = 0;
  switch (d)
  {
    case Back:
     if (pot_units/50<=18 ){
    driver.motorAForward(70);
    driver.motorBForward(70);
    delay(100);
    
  }
      driver.motorBStop();
      driver.motorAStop();
      delay(200);
      
      uturn();
      delay(70);
      if (at_goal) {
        at_goal=false;

        while (Sensor(6)) read_sensors();
        delay(100);
                glow(false);

      } else {
        if (ways[Left]) {
          read_sensors();
          while (!Sensor(6)) read_sensors();
          delay(200);
        }
        read_sensors();
      }
      while (!Sensor(6)) read_sensors();
      return;
      
    case Forward:
      return;
    case Right:
      rightturn();
      pro_turn = 2;
      break;
    case Left:
      leftturn();
      if (ways[Forward])
      {
        pro_turn = 4;
      }
      else {
        pro_turn = 5;
      }
      break;
  }
  read_sensors();
  while (!Sensor(pro_turn)) read_sensors();
}

Bot bot;
#define GRAPH
#include "Graph.cpp"
Graph graph;


void setup()
{
  Debug(Serial.begin(9600));
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
  driver.motorBStop();
  driver.motorAStop();

  pinMode(BUTTON, INPUT);
  pinMode(BUTTON_VCC, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(3, INPUT);
  pinMode(51, INPUT);
  pinMode(53, INPUT);
  digitalWrite(BUTTON_VCC, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  if (analogRead(A7) < 300) 	                          graph.algorithm = RND;
  else if (analogRead(A7) >= 300 && analogRead(A7) < 700)  graph.algorithm = RHS;
  else                                                graph.algorithm = LHS;

  DEBUGALGO(graph.algorithm); ENDL;

  if (analogRead(A6) >= 8) {
    bot.pot_units = analogRead(A6);
    DEBUG(bot.pot_units / 50);
    digitalWrite(13, HIGH);
    delay(6000);
    NORMAL_SPEED = (analogRead(A6) / 10) + 50;
    digitalWrite(13, LOW);
  }
  AutoCalib
  (
    for (char i = 0; i < 70; i++)
{
  if (i < 20 || i >= 60)
    {
      driver.motorAReverse(55);
      driver.motorBForward(55);
    }
    else
    {
      driver.motorBReverse(55);
      driver.motorAForward(55);
    }
    qtrrc.calibrate();
    delay(10);
    driver.motorBStop();
    driver.motorAStop();
  }
  )
  DEBUG("Callibration Done\n");
  delay(1000);
}


void loop()
{
  if (analogRead(A6) >= 8) {
    bot.follow_segment();
    // DEBUG("Followed\n");

    bot.turn(graph.decider());
    driver.motorBStop();
    driver.motorAStop();

    delay(70);
    for (int i; i < 40; i++)
    {
      bot.pid(30);
      delay(2);
    }

  }
  else {
    bot.follow_segment();
    bot.stop();
    delay(100);
    while (calibrate_unit > (analogRead(A6) / 50)) {
      digitalWrite(13, HIGH);
    }
    digitalWrite(13, LOW);
    while (1) {
    }
  }
}

void forward()
{
  //DEBUG("Forward");DEBUG("\n");
  driver.motorBStop();
  driver.motorAStop();
  if (bot.pot_units/50<=16 )
  {
    driver.motorAForward(40);
    driver.motorBForward(40);
  }
}

void query()
{
}
