#define Forward 0
#define Right 1
#define Back 2
#define Left 3
#define North 0
#define East 1
#define South 2
#define West 3
#define Explore 0
#define Path 1
#define Panic 2
#define Dijkstra 3
#define Wait 4
typedef char State;
typedef char Direction ;
typedef char Disha ;
typedef char node;
#define DEBUGDIS(d)(DEBUG(d==North?"North":(d==East?"East":(d==South?"South":"West"))))
#define DEBUGDIR(d)(DEBUG(d==Forward?"Forward":(d==Right?"Right":(d==Back?"Back":"Left"))))
#define DEBUGSTATE(d)(DEBUG(d==Wait?"Wait":(d==Explore?"Explore":(d==Path?"Path":(d==Panic?"Panic":"Dijkstra")))))
#define ENDL DEBUG("\n");
#define MAX_NODES 100
#define MAX_PATH 80
#define INT_MAX 100
#define G_ERROR 1

class Graph
{
  public:
    node path[MAX_PATH] = {0};
    Direction path_trace[MAX_PATH];

    node goal = -1;
    node node_n = 1;

    int path_n = 1;
    int path_trace_n = 0;
    node current_node = 0;

    State state = Explore, next_state;


    Direction LHS_rule();
    Direction decide_turn();
    Direction follow_path();
    Direction wait();
    Direction decider();

    void show_path();
    void show_nodes();
    void show_trace();

    void backtrack();
    void create_node();
    bool is_visited();
    void dijkstra();
    void reset();

    struct Node
    {
      node parent = -1;
      node neighbour[4] = { -2, -2, -2, -2};
      int x;
      int y;
    } nodes[MAX_NODES];

};



