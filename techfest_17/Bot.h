#define EMITTER_PIN   4
#define FORWARD_SENSOR  100
#define BUTTON_VCC 20
#define BUTTON 31
#define ENCODER 21
#define NUM_SENSORS   6
#define inputA1 5
#define inputA2 6
#define inputB1 10
#define inputB2 11

#define KP  0.2
#define KD  0
#define NORMAL_SPEED 200
#define MAX_SPEED    255
#define TURN_DELAY   300
#define TIMEOUT      2500
#define SCALING_FACTOR    100000
#define GOAL_DIST  3


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
    void  read_sensors();
    void  pid(int speed);

  public:
    void stop();
    bool get_button();
    long  pos[2] = {0, 0};
    bool  ways[4];
    bool  at_goal = false;
    bool  at_block=false;
    Disha  disha = North;
    void  follow_segment();
    void  turn(Direction);
};

