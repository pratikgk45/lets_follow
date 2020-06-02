/*
  #####~~~~ LETS FOLLOW ~~~~#####
  PLEASE WORK ON THIS VERSION ONLY
  Upoad HMC5883L QTRSensors library to Libs on Drive Please
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

unsigned long get_random()
{
  return millis();
}

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

void uturn()
{
  //DEBUG("uturn"); DEBUG("\n");
  driver.motorBForward(255);
  driver.motorAReverse(255);
}


void forward()
{
    driver.motorAForward(150);
    driver.motorBForward(150);
}

void Bot:: stop()
{
  DEBUG("STOP");ENDL;
  driver.motorBStop();
  driver.motorAStop();
}

void Bot:: glow (int y ,bool x)
{
  digitalWrite(y, x?HIGH:LOW);
}

bool Bot :: Sensor(int x)
{
  if (x == 0)return I digitalRead(2);
  if (x == 7)return I digitalRead(3);
  if (x == ENCODER)  return I digitalRead(ENCODER);
  if (x==FORWARD_SENSOR)return I digitalRead(FORWARD_SENSOR);
  return I (sensors[x - 1] < 200);
}

bool Bot:: get_button()
{
  return digitalRead(BUTTON);
}

void Bot:: read_sensors()
{
  // DEBUG("ReadSensor");DEBUG("\n")
  qtrc_pos = qtrrc.readLine(sensors);
  AutoQTR
  (
    for (int i = 0; i < 8; i++)
{
  DEBUG(sensors[i]); DEBUG('\t');
  }
  ENDL;
  )
  found[Right] = Sensor(0) && Sensor(1) ;
  found[Left] = Sensor(7) && Sensor(6);
  found[Forward] = Sensor(4) && Sensor(3) || Sensor(4) && Sensor(5) || Sensor(2) && Sensor(3);
}

bool Bot::has_forward()
{ 
  bool r;
  bool r_pre = 1;
  unsigned long current_time = 0,goal_time=millis();
  while (found[Left] || found[Right])
  {
    read_sensors();
    current_time=millis()-goal_time;
    if (!Sensor(4) && !Sensor(3) && current_time<25)
    {
      glow(13,true);
      found_wn=true;
    }
    if(current_time>GOAL_TIME)
    {
      glow(13,true);
      stop();
      while(true);  
    }
    ways[Left] = found[Left] || ways[Left];
    ways[Right] = found[Right] || ways[Right];
  }
  delay(FORWARD_DELAY);
  read_sensors();
  glow(13,false);
  bool x = 0;
  for (int h = 1; h < 6; h++)
  {
    x = x | Sensor(h);
  }
  return x;
}

bool Bot:: has_node()
{
  read_sensors();
  ways[Left] = found[Left];
  ways[Right] = found[Right];
  ways[Forward] = found[Forward];
  read_sensors();
  
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


void Bot::follow_segment()
{
  DEBUG("Follow  at "); 
  while (!has_node())
  {
    pid(NORMAL_SPEED);
  }
  for ( int k = 0; k < 4; k++)  DEBUG(ways[k]); ENDL;
  

  switch (disha)
  {
    case North:
      pos[1] ++;
      break;
    case East:
      pos[0] ++;
      break;
    case South:
      pos[1] --;
      break;
    case West:
      pos[0] --;
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
        read_sensors();
        while (!found[Forward])read_sensors();
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
  for (int i=0 ;i<10;i++)
  {
    read_sensors();
    pid(200);
    delay(2);
  }
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
  pinMode(37, OUTPUT);
  pinMode(39, OUTPUT);
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

  if (analogRead(A6) <= 500)
  {
    graph.state = Headless;
  }
  else
  {
    graph.state = Explore;
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
  bot.follow_segment();
  bot.turn(graph.decider());
}

void query()
{

}
