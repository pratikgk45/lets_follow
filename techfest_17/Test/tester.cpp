#ifndef ARDUINO
#define GRAPH
#include <iostream>
# include <cmath>
#include <cstdlib>
#include "Graph.h"
#include "Bot.h"
#define Debug
#define DEBUG(x) (cout<<(x))
#include <string>
using namespace std;

void query();

int abs(int i) {
  if (i > 1)return i;
  return -i;
}
float sq(float x) {
  return x * x;
}


void Bot::follow_segment()
{
  int dist;
  cout << "> Distance ? ";
  cout.flush();
  cin >> dist;


  pos[0] += (disha % 2) * (2 - disha) * dist;
  pos[1] += (1 - disha % 2) * (1 - disha) * dist;
  string s;
  cout << "Pos " << pos[0] << " " << pos[1] << endl << "> Give Ways ?";
  cout.flush();
  cin >> s;
  for (int i = 0; i < 4; i++)
  {
    ways[i] = (s[i] == '1');
  }
}


void Bot::turn(Direction d) {
  disha = (disha + d) % 4;
  cout << "Turn: " << (d == 0 ? "Forward" : (d == 1 ? "Right" : (d == 2 ? "Back" : "Left"))) << endl;
  cout << "Disha " << (disha == 0 ? "North" : (disha == 1 ? "East" : (disha == 2 ? "South" : "West"))) << endl;
}
Bot bot;
#include "Graph.cpp"
Graph graph;

int main()
{
  while (true)
  {
    bot.follow_segment();
    query();
    bot.turn(graph.decider());
  }
}

void query()
{
  char c;
  do
  {
    cout << "> Going Further ?";
    cin >> c;
    switch (c)
    {
      case 'b':
        DEBUGDIS(bot.disha); cout << " " << bot.pos[0] << " " << bot.pos[1] << endl;
        break;
      case 'p':
        graph.show_nodes();
        break;
      case 'P':
        graph.show_path();
        break;
      case 'g':
        bot.at_goal = true;
        break;
      case 'G':
        cout << (int)graph.goal << endl;
        break;
      case 'R':
        graph.state = Dijkstra;
        break;
      case 's':
        DEBUG("State "); DEBUGSTATE(graph.state);
        DEBUG(" Next "); DEBUGSTATE(graph.next_state);
        DEBUG("\n");
        break;
      case 't':
        graph.show_trace();
        break;
      case 'q':
        return ;
    }
  } while (c != 'y');
}

#endif
