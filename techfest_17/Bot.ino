/*
  #####~~~~ LETS FOLLOW ~~~~#####
  Bot can be set to any mode
  With Magnetometer just write A in front of Magnet(A) and remove A in front of noMagnet(A)
  Same System for time based or encoder based distance
  PLEASE WORK ON THIS VERSION ONLY
  Upoad HMC5883L library to Libs on Drive Please
  $$$COMPILE TIME MAGIC$$$
*/
#include <QTRSensors.h>
#include <DRV8833.h>

#include "Graph.h"
#include "Bot.h"

#define Debug(A) A
#define DEBUG(A) Serial.print(A)

#define AutoCalib(A) A

DRV8833 driver = DRV8833();
QTRSensorsRC qtrrc((unsigned char[]) {
  A0, A1, A2, A3, A4, A5
}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);


void leftturn()
{
  //DEBUG("left");DEBUG("\n");
  driver.motorBForward(255);
  driver.motorAReverse(255);
  delay(TURN_DELAY);
}

void rightturn()
{
  //DEBUG("Right");DEBUG("\n");
  driver.motorBReverse(255);
  driver.motorAForward(255);
  delay(TURN_DELAY);
}

void forward()
{
  //DEBUG("Forward");DEBUG("\n");
  driver.motorBForward(100);
  driver.motorAForward(100);
}

void uturn()
{
  //DEBUG("uturn"); DEBUG("\n");
  forward();
  delay(200);
  driver.motorBForward(255);
  driver.motorAReverse(255);
}

bool Bot :: Sensor(int x)
{
  if (x == 0)return digitalRead(2);
  if (x == 7)return digitalRead(3);
  if (x == FORWARD_SENSOR)return digitalRead(51);
  if (x == ENCODER) return digitalRead(ENCODER);
  return sensors[x - 1] < 400;
}

bool Bot:: get_button()
{
  return digitalRead(BUTTON);
}

void Bot:: read_sensors()
{
  // DEBUG("ReadSensor");DEBUG("\n")
  qtrc_pos = qtrrc.readLine(sensors);
  found[Right] = Sensor(0) && Sensor(1) ;
  found[Left] = Sensor(7) && Sensor(6);
  found[Forward] = Sensor(4) && Sensor(3) || Sensor(4) && Sensor(5) || Sensor(2) && Sensor(3);
}

bool Bot::has_forward()
{
  //DEBUG("Has Forward\n")
  long current_time=0;
  bool r;
  bool r_pre = 1;

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

    if (current_time >= GOAL_DIST)
    {
         at_goal = true;
         return true;
    }
  }
  //detachInterrupt(digitalPinToInterrupt(ENCODER));
  delay(150);
  read_sensors();
  return found[Forward];
}

bool Bot:: has_node()
{
  //DEBUG("Has node");
  read_sensors();
  ways[Left] = found[Left];
  ways[Right] = found[Right];
  ways[Forward] = found[Forward];
  if (Sensor(FORWARD_SENSOR))
  {
    ways[Left] = 0;
    ways[Right] = 0;
    ways[Forward] = 0;
    ways[Backg] = 0;
    at_block = true;
  }
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
  motorspeed = KP * error ;
  lasterror = error + KD * (error - lasterror);
  lms = normal_speed - motorspeed;
  rms = normal_speed + motorspeed;
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
  long current_time=0;
  bool r;
  bool r_pre = 1;
  //attachInterrupt(digitalPinToInterrupt(ENCODER), encoder, RISING);
  while (!has_node())
  {
    r = Sensor(ENCODER);
    if (r == 0 && r_pre == 1) current_time++;
    r_pre=r;
    pid(NORMAL_SPEED);
  }
  //detachInterrupt(digitalPinToInterrupt(ENCODER));
  //DEBUG(current_time);DEBUG("\n");
  if (current_time <= 20)
    current_time = 1;
  else if (current_time <= 50)
    current_time = 2;
  else
    current_time = 3;

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
        uturn();
        delay(200);
          if (ways[Left]){
            read_sensors();
            while (!found[Forward]) read_sensors();
            delay(200);}
          read_sensors();
          while (!found[Forward]) read_sensors();
          return;
      case Forward:
        return;
      case Right:
        rightturn();
        pro_turn = 4;
        break;
      case Left:
        leftturn();
        pro_turn = 3;
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

  pinMode(BUTTON,INPUT);
  pinMode(BUTTON_VCC, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(51, INPUT);
  pinMode(53, INPUT);
  digitalWrite(BUTTON_VCC, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
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
}

void loop()
{
  digitalWrite(13, HIGH);
  bot.follow_segment();
  digitalWrite(13, LOW);
  bot.turn(graph.decider());
 //  bot.turn(Back);
}


void query()
{

}
