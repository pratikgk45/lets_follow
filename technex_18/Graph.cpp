#ifdef GRAPH
#include "Utility.cpp"

Direction Graph::resolve_rule()
{
    switch(algorithm)
    {
        case RND:   return random_rule();
        case RHS:   return RHS_rule();
        case LHS:   return LHS_rule();
    }
}

Direction Graph::LHS_rule()
{
    if (bot.ways[Left])     return Left;
    if (bot.ways[Forward])  return Forward;
    if (bot.ways[Right])    return Right;
                            return Back;
}

Direction Graph::RHS_rule()
{
    if (bot.ways[Right])    return Right;
    if (bot.ways[Forward])  return Forward;
    if (bot.ways[Left])return Left;
    else                    return Back;
}


void Graph:: show_nodes()
{
    DEBUG("Nodes ");DEBUG((int)node_n);ENDL;
    for (int i = 0; i < node_n; i++)
    {
        DEBUG(i);DEBUG(" ");
        DEBUG(nodes[i].x);DEBUG(" ");
        DEBUG(nodes[i].y);DEBUG(" ");
        DEBUG("\n");
    }
}


Direction Graph::random_rule()
{
    unsigned long rand=get_random();
    int n=0;
    Direction directions_available[3];
    if (bot.ways[Left])
        directions_available[n++]=Left;
    if (bot.ways[Forward])
        directions_available[n++]=Forward;
    if (bot.ways[Right])
        directions_available[n++]=Right;
    if (n==0)
        return Back;
    return directions_available[rand%n];
}

Direction Graph:: decide_turn()
{
  if (bot.found_wn) 
  {
    bot.found_wn=false;
    create_node();
  }
  if (abs(bot.pos[0])==(GRID_SIZE-1))
  {
    if ((GRID_SIZE%2==0 && bot.pos[1]==0 )||(GRID_SIZE%2 && bot.pos[1]==GRID_SIZE-1))
    {
      bot.stop();
      grid();
      while(true);
    }
  }
  if (bot.pos[1]==(GRID_SIZE-1)  )  return Right;
  if (bot.pos[1]==0 && ! first_time)return Left;
  first_time=false;
  return Forward;  
}

void Graph::create_node()
{
    nodes[node_n].x = bot.pos[0];
    nodes[node_n].y = bot.pos[1];
    node_n++;

}

Direction Graph::decider()
{
  switch (state)
  { 
    case Explore:   return decide_turn();
    case Headless:  return resolve_rule();
  }
}

void Graph:: grid()
{
  show_nodes();
  DEBUG("White Nodes ");
  DEBUG((int)node_n) ;ENDL;
  if (node_n==3)
  {
    bool istriangle = (nodes[1].x-nodes[0].x)*(nodes[2].y-nodes[0].y)
                - (nodes[2].x-nodes[0].x)*(nodes[1].y-nodes[0].y);
    if(istriangle)
    {
      bot.glow(37,true);
      DEBUG("TRIANGEL");ENDL;
    }
    else
    {
      DEBUG("LINE");ENDL;
    }
  } 
  if (node_n==4)
  {
    bool issquare = abs(nodes[1].y-nodes[0].y)==abs(nodes[2].x-nodes[1].x);
    if (issquare)
    {
      DEBUG("SQUARE");ENDL;
      bot.glow(13,true);
    }
    else
    {
      DEBUG("RECTANGLE");ENDL;
      bot.glow(39,true);
    }
  } 
}
#endif
