#ifdef GRAPH

#define INT_MAX 10000

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

void Graph:: show_nodes()
{
    DEBUG("Nodes ");DEBUG((int)node_n);ENDL;
    for (int i = 0; i < node_n; i++)
    {
        DEBUG(i);DEBUG(" ");
        DEBUG(nodes[i].x);DEBUG(" ");
        DEBUG(nodes[i].y);DEBUG(" ");
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
    DEBUG("Path ");DEBUG((int)path_n);DEBUG("\n");
    for (int i = 0; i < path_n; i++)
    {
        DEBUG((int)path[i]);DEBUG("  ");
    }
    ENDL;
}

void Graph::show_trace()
{
    DEBUG("Trace ");DEBUG((int)path_trace_n);DEBUG("\n");
    for (int i = 0; i < path_trace_n; i++)
    {
        char d = path_trace[i];
        DEBUGDIR(d);DEBUG(" ");
    }
    ENDL;
}
#endif
