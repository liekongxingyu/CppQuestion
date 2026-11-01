#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Task {
    int priority;
    std::string name;
};

struct TaskCmp {
    // priority 大的排在前面；priority 相同则按 name 字典序小的在前（可选规则，保证稳定的确定性）
    bool operator()(const Task& a, const Task& b) const {
        if (a.priority != b.priority) return a.priority < b.priority; // 小的算“更差”，放后面
        return a.name > b.name; // 同优先级时 name 小的更靠前
    }
};

int main() {
    int n;
    std::cin >> n;

    // 原型：<元素类型,底层容器,比较器>
    std::priority_queue<Task, std::vector<Task>, TaskCmp> pq;

    for (int i = 0; i < n; ++i) {
        Task t;
        std::cin >> t.priority >> t.name;
        pq.push(t);
    }

    while (!pq.empty()) {
        std::cout << pq.top().name << '\n';
        // pop为弹出当前最高优先级任务，top为取出当前最高优先级任务
        pq.pop();
    }

    return 0;
}
