#ifndef ARDUINO
#define GRAPH
#endif
#ifdef GRAPH

void query();

void Graph::reset()
{
    bot.disha = North;
    bot.pos[0] = 0;
    bot.pos[1] = 0;
    path_n = 1;
    path_trace_n = 0;
    current_node = 0;
    state = Path;
    next_state = Wait;
}

void Graph:: show_nodes()
{
    DEBUG("Nodes ");
    DEBUG((int)node_n);
    ENDL;
    for (int i = 0; i < node_n; i++)
    {
        DEBUG(i);
        DEBUG(" ");
        DEBUG(nodes[i].x);
        DEBUG(" ");
        DEBUG(nodes[i].y);
        DEBUG(" ");
        DEBUG((int)nodes[i].parent);
        DEBUG(" | ");
        for (int j = 0; j < 4; j++)
        {
            DEBUG((int)nodes[i].neighbour[j]);
            DEBUG(" ,");
        }
        DEBUG("\n");
    }
}

void Graph::show_path()
{
    DEBUG("Path ");
    DEBUG((int)path_n);
    DEBUG("\n");
    for (int i = 0; i < path_n; i++)
    {
        DEBUG((int)path[i]);
        DEBUG("  ");
    }
    ENDL;
}

void Graph::show_trace()
{

    DEBUG("Trace ");
    DEBUG((int)path_trace_n);
    DEBUG("\n");
    for (int i = 0; i < path_trace_n; i++)
    {
        char d = path_trace[i];
        DEBUGDIR(d);
        DEBUG(" ");
    }
    ENDL;
}

void Graph::backtrack()
{
    DEBUG("Backtrack\n");
    state = Path;
    next_state = Wait;
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
                goto OUTSIDE_WHILE;
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
OUTSIDE_WHILE:
    current_node = path[path_n++];
    std::reverse(path_trace,path_trace+ path_trace_n);
}

void Graph::create_node()
{
    DEBUG("Created Node ");
    DEBUG((int)node_n);
    DEBUG(" at ");
    DEBUG(bot.pos[0]);
    DEBUG(" ");
    DEBUG( bot.pos[1]);
    DEBUG("\n");
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
    if (bot.at_block)
    {
        bot.at_block=false;
    }
    else
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
                return true;
            }
        }
    }
    create_node();
    path[path_n++] = current_node;
    return false;
}

Direction Graph::decide_turn()
{
    Direction current_direction = LHS_rule();
    if (is_visited() )
    {
        backtrack();
        return follow_path();
    }
    if (bot.at_goal)
    {
        bot.at_goal = false;
        goal = node_n-1;
        state=Dijkstra;
        next_state=Wait;
        DEBUG("Created Goal\n");
        return wait();
    }
    if (current_direction == Back )
    {
        backtrack();
        return follow_path();
    }
    return current_direction;
}

Direction Graph::LHS_rule()
{
    if (bot.ways[Left])
        return Left;
    if (bot.ways[Forward])
        return Forward;
    if (bot.ways[Right])
        return Right;
    return Back;
}

int closest_node(int dist[], bool visited[], int node_n)
{
    int m = INT_MAX, m_index;
    for (int v = 0; v < node_n; v++)
    {
        if (!visited[v] && dist[v] <= m)
        {
            m = dist[v];
            m_index = v;
        }
    }
    return m_index;
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
        DEBUG((int)c);
        DEBUG(" ");
        d = nodes[c].parent;
        if (d==-1)break;
        c = nodes[c].neighbour[(2 + d) % 4];
        path_trace[path_trace_n++] = (4 + d - nodes[c].parent) % 4;
    }

    path_trace_n--;
    ENDL;
    state=Path;
}

Direction Graph:: wait()
{
    bot.stop();
    while (bot.get_button());
//    while (Serial.read()==-1);
    DEBUG("Wait End : Reset\n");
    return Forward;
}


Direction Graph::follow_path()
{


    if (path_trace_n == 1)
    {
        state = next_state;
        DEBUG("Shifting to state ");
        DEBUGSTATE(state);
        ENDL;
    }
    else if (path_trace_n==0)
    {
        state = Wait;
        next_state= Wait;
        return Forward;
    }
    return path_trace[--path_trace_n];
}


Direction Graph::decider()
{
    DEBUG("State = ");
    DEBUGSTATE(state);
    ENDL;
    switch (state)
    {
    case Explore:
        return decide_turn();
    case Wait:
        return wait();
    case Path:
        return follow_path();
    case Panic:
        return LHS_rule();
    case Dijkstra:
        dijkstra();
        query();
        DEBUG("DJIKSTRA : over\n");
        return  follow_path();
    }
}


#endif
