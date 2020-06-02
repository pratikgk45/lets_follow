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

#define MAX_NODES 150
#define MAX_PATH 80
#define G_ERROR 1

typedef char Direction ;
typedef char Disha ;

void query();

class Graph
{
  public:
    typedef char node;
    typedef char State;
    typedef char Algorithm;

    node path[MAX_PATH] = {0};
    Direction path_trace[MAX_PATH];
    node goal = -1;
    node node_n = 1;
    int path_n = 1;
    int path_trace_n = 0;
    node current_node = 0;
    State state = Explore, next_state;
    bool finished=false;
    Algorithm algorithm =LHS;

    Direction LHS_rule();
    Direction RHS_rule();
    Direction random_rule();
    Direction decide_turn();
    Direction follow_path();
    Direction decider();
    Direction resolve_rule();

    void show_path();
    void show_nodes();
    void show_trace();

    void wait();
    void backtrack();
    void create_node();
    bool is_visited();
    void dijkstra();

    struct Node
    {
      node parent = -1;
      node neighbour[4] = { -2, -2, -2, -2};
      int x;
      int y;
    } nodes[MAX_NODES];

};



