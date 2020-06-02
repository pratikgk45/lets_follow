#define Forward 0
#define Right 1
#define Back 2
#define Left 3
#define North 0
#define East 1
#define South 2
#define West 3

#define Explore 6
#define Path 1
#define Headless 2
#define Dijkstra 3
#define Wait 4
#define Finish 5
#define LHS 0
#define RHS 1
#define RND 2
int d;
#define DEBUGDIS(d)(DEBUG(d==North?"North":(d==East?"East":(d==South?"South":"West"))))
#define DEBUGDIR(d)(DEBUG(d==Forward?"Forward":(d==Right?"Right":(d==Back?"Back":"Left"))))
#define DEBUGSTATE(d)(DEBUG(d==Wait?"Wait":(d==Explore?"Explore":(d==Path?"Path":(d==Headless?"Headless":"Dijkstra")))))
#define DEBUGALGO(d)(DEBUG(d==LHS?"LHS":(d==RHS?"RHS":"RND")))
#define ENDL DEBUG("\n");

#define MAX_NODES 200
#define MAX_WHITE_NODE 5
#define MAX_PATH 1010
#define G_ERROR 1
#define GRID_SIZE 5

typedef char Direction ;
typedef char Disha ;

void query();

class Graph
{
  public:
    typedef char node;
    typedef char State;
    typedef char Algorithm;

    node node_n = 0;
    State state = Headless, next_state;
    bool finished=false;
    Algorithm algorithm =LHS;

    bool first_time=true;

    Direction LHS_rule();
    Direction RHS_rule();
    Direction random_rule();
    Direction resolve_rule();

    Direction decide_turn();
    Direction decider();

    void show_nodes();
    void create_node();
    void grid();
    
    struct Node
    {
      int x;
      int y;
    } nodes[MAX_WHITE_NODE];

};



