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
    if (bot.ways[Left])     return Left;
                            return Back;
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

void Graph::backtrack()
{
  DEBUG("Backtrack\n");
  state = Path; 
  Disha d = bot.disha;
  while (path_n--)
  {
    node parent = 4;
    for (char j = 0; j < 4; j++)
    {
      if (nodes[path[path_n]].neighbour[j] == -1)
      {
        path_trace[path_trace_n++] = (4 + j - d) % 4;
        next_state = Explore;
        goto SUCCESS;
      }
      if (nodes[path[path_n]].neighbour[j] == path[path_n - 1])
      {
        parent = j;
      }
    }

    if (parent != 4)
    {
      path_trace[path_trace_n++] = (4 + parent - d) % 4;
      d = parent;
    }
  } 

FAILURE:
  next_state=Dijkstra;
SUCCESS:
  current_node = path[path_n++];
  std::reverse(path_trace,path_trace + path_trace_n);
  
}

void Graph::create_node()
{
  DEBUG("Created Node "); DEBUG((int)node_n); DEBUG(" at "); DEBUG(bot.pos[0]); DEBUG(" "); DEBUG( bot.pos[1]); DEBUG("\n");
  for (int i = 0; i < 4; i++)
  {
    nodes[node_n].neighbour[i] = bot.ways[(4 + i - bot.disha) % 4] - 2;
  }
  nodes[node_n].x = bot.pos[0];
  nodes[node_n].y = bot.pos[1];
  nodes[current_node].neighbour[bot.disha] = node_n;
  nodes[node_n].neighbour[(bot.disha + 2) % 4] = current_node;
  current_node = node_n++;
}

bool Graph::is_visited()
{
    for (char i = 0; i < node_n; i++)
    {
      if (abs(bot.pos[0] - nodes[i].x) < G_ERROR && abs(bot.pos[1] - nodes[i].y) < G_ERROR)
      {
        nodes[i].neighbour[(bot.disha + 2) % 4] = current_node;
        nodes[current_node].neighbour[bot.disha] = i;
        bot.pos[0] = nodes[i].x;
        bot.pos[1] = nodes[i].y;
        current_node = i;
        path[path_n++] = current_node;
        DEBUG("Visited ");DEBUG(current_node);ENDL;
        return true;
      }
    }
  create_node();
  path[path_n++] = current_node;
  return false;
}

Direction Graph::decide_turn()
{
  Direction current_direction = resolve_rule();
  if (is_visited() )
  {
    backtrack();
    return follow_path();
  }
  if (bot.at_goal && goal==-1)
  {
    DEBUG("Created Goal\n");
    goal = node_n-1;  
    bot.glow(HIGH);
    backtrack();
    show_path();
    show_nodes();
    show_trace();
    return follow_path();
  }
  if (current_direction == Back )
  {
    backtrack();
    return follow_path();
  }
  return current_direction;
}
void Graph:: dijkstra()
{
  int dist[MAX_NODES];
  bool visited[MAX_NODES];

  for (int i = 0; i < node_n; i++)
  {
    dist[i] = INT_MAX;
    visited[i] = false;
  }

  dist[0] = 0;
  for (int i = 0; i < node_n; i++)
  {
    node u = closest_node(dist, visited, node_n);
    visited[u] = true;
    for (char j = 0; j < 4; j++)
    {
      node v = nodes[u].neighbour[j];
      if (v == -2 || v == -1)
      {
        continue;
      }
      int v_dist = sqrt(sq(nodes[u].x - nodes[v].x) + sq(nodes[u].y - nodes[v].y));
      if (!visited[v] && (dist[u] + v_dist < dist[v]))
      {
        dist[v] = dist[u] + v_dist;
        nodes[v].parent = j;
      }
    }
  }
  DEBUG("OVER\n");

  node c = goal;
  Disha d ;
  while (true)
  {
    DEBUG((int)c);DEBUG(" ");
    d = nodes[c].parent;
    if (d==-1)break;
    c = nodes[c].neighbour[(2 + d) % 4];
    path_trace[path_trace_n++] = (4 + d - nodes[c].parent) % 4;
  }

  path_trace_n--;
  state=Path;
  ENDL;
}

void Graph:: wait()
{

    bot.stop();
    bot.glow(true);
    while(bot.get_button());
    bot.glow(false);
    delay(500);
}


Direction Graph::follow_path()
{

  if (path_trace_n == 1)
  {
    state = next_state;
    DEBUG("Shifting to state ");DEBUGSTATE(state);ENDL;
  }
  return path_trace[--path_trace_n];
}


Direction Graph::decider()
{
  DEBUG("State = ");DEBUGSTATE(state);ENDL;
  switch (state)
  {
    case Explore:   return decide_turn();
    case Path:      return follow_path();
    case Headless:  return resolve_rule();
    case Wait:      wait();return Forward;
    case Dijkstra:  
      wait();
      dijkstra(); 
      next_state=Finish;
      return Forward;
    case Finish:
      bot.glow(HIGH);
      bot.stop();
      while(true);
  }
}


#endif
