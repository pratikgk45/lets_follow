#define EMITTER_PIN   4
#define FORWARD_SENSOR  51
#define BUTTON_VCC 20
#define BUTTON 31
#define ENCODER 21
#define NUM_SENSORS   6
#define inputA1 5
#define inputA2 6
#define inputB1 10
#define inputB2 11 
#define LED_PIN 13

#define KP  0.2
#define KD  00
int NORMAL_SPEED=200;
int calibrate_unit=0;

#define MAX_SPEED    255
#define TURN_DELAY   350
#define FORWARD_DELAY 100
#define TIMEOUT      2500
#define SCALING_FACTOR    100000
#define GOAL_DIST  4
#define GOAL_TIME  500



class Bot
{
  protected:
    unsigned int sensors[NUM_SENSORS];
    unsigned int qtrc_pos = 0;
    int   lasterror = 0;
    
    bool  found[4];
    

    bool  Sensor(int x);
    bool  has_forward();
    bool  has_node();
     

  public: 
      void  pid(int speed);
      int   pot_units=1024;
      int junc=0;
      int junc_in_y=1;
      bool found_wn=false;
      int white_node_count=0;
      int loc_white[4][2];


      
      long  pos[2] = {0, -1};
      bool  ways[4];
      bool  at_goal = false;
      Disha   disha = North;

      void glow(int ,bool);
      void stop();
      void  follow_segment();
      void  turn(Direction);
      void  read_sensors();
      bool get_button();
      unsigned long get_random();
      


};

