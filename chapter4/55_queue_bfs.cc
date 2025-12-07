#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

int main()
{
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 为了输出顺序稳定（通常题目希望小的先访问），把邻接表排序一下
    for (int i = 1; i <= n; ++i)
    {
        std::sort(adj[i].begin(), adj[i].end());
    }

    std::vector<int> visited(n + 1, 0);
    std::queue<int> q;

    visited[1] = 1;
    q.push(1);

    bool first = true;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        if (!first)
            std::cout << ' ';
        first = false;
        std::cout << u;

        for (std::size_t i = 0; i < adj[u].size(); ++i)
        {
            int v = adj[u][i];
            if (!visited[v])
            {
                visited[v] = 1;
                q.push(v);
            }
        }
    }

    std::cout << '\n';
    return 0;
}
