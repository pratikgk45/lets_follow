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

#define KP  0.025
#define KD  0.1
int NORMAL_SPEED=75;
int calibrate_unit=0;
#define MAX_SPEED    255
#define TURN_DELAY   200
#define FORWARD_DELAY 0
#define TIMEOUT      2500
#define SCALING_FACTOR    100000
#define GOAL_DIST  4


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

  void  read_sensors();
    void stop(); 
    bool get_button();
    unsigned long get_random();
    void glow(bool); 
    long  pos[2] = {0, 0};
    bool  ways[4];
    bool  at_goal = false;
    bool at_block=false;
    Disha   disha = North;
    void  follow_segment();
    void  turn(Direction);
};

