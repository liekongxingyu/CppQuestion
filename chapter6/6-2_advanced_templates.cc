#include <iostream>
#include <cstdint>          // 定宽整数类型 (stdint): int8_t, int32_t, uint32_t, int64_t, uint64_t
#include <initializer_list> // std::initializer_list 初始化列表
#include <type_traits>      // 类型萃取与元编程: enable_if, is_integral, void_t 等
#include <vector>
#include <string>

// ============================================================================
// 1. 变量模板 (Variable Template) 与 别名模板 (Alias Template)
// ============================================================================

// 变量模板：支持不同浮点/整数类型获取编译期常量
template <typename T>
constexpr T Pi = static_cast<T>(3.14159265358979323846L);

// 别名模板：简化类型重命名
template <typename T>
using Int32Pair = std::pair<T, int32_t>;

// ============================================================================
// 2. 非类型模板参数 (NTTP) + 全特化与偏特化 (Template Specialization)
// ============================================================================

// NTTP: 非类型参数 std::size_t N
template <typename T, std::size_t N>
class StaticBuffer {
private:
    T data_[N];

public:
    constexpr StaticBuffer() : data_{} {}

    // 使用 std::initializer_list 进行列表初始化
    StaticBuffer(std::initializer_list<T> list) : data_{} {
        std::size_t idx = 0;
        for (const auto &item : list) {
            if (idx < N) {
                data_[idx++] = item;
            }
        }
    }

    constexpr std::size_t size() const { return N; }
    T& operator[](std::size_t idx) { return data_[idx]; }
    const T& operator[](std::size_t idx) const { return data_[idx]; }
};

// 通用模板定义
template <typename T>
struct TypeInspector {
    static void inspect() {
        std::cout << "[TypeInspector] 通用泛型版本" << std::endl;
    }
};

// 全特化 (Full Specialization): 专门针对 int32_t
template <>
struct TypeInspector<int32_t> {
    static void inspect() {
        std::cout << "[TypeInspector] 全特化版本: int32_t" << std::endl;
    }
};

// 偏特化 (Partial Specialization): 针对指针类型 T*
template <typename T>
struct TypeInspector<T*> {
    static void inspect() {
        std::cout << "[TypeInspector] 偏特化版本: 指针类型 T*" << std::endl;
    }
};

// ============================================================================
// 3. 变长参数模板 (Variadic Templates) 与 C++17 折叠表达式 (Fold Expressions)
// ============================================================================

// 折叠表达式求和: (... + args)
template <typename... Args>
auto sumValues(Args... args) {
    return (... + args);
}

// 逗号折叠表达式: 展开并打印任意数量的参数
template <typename... Args>
void printValues(const Args&... args) {
    ((std::cout << args << " "), ...) << std::endl;
}

// ============================================================================
// 4. 模板模板参数 (Template Template Parameters)
// ============================================================================

// 容器适配器：第二个模板参数本身也是一个接受类型的模板 Container
template <typename T, template <typename, typename...> class Container>
class ContainerAdapter {
private:
    Container<T> container_;

public:
    void push(const T &val) {
        container_.push_back(val);
    }

    std::size_t size() const {
        return container_.size();
    }
};

// ============================================================================
// 5. SFINAE (匹配失败不是错误) 与 std::enable_if_t
// ============================================================================

// 重载 1: 仅对整数类型有效 (如 int32_t, uint64_t)
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
formatPrint(T val) {
    std::cout << "[SFINAE - 整数] " << static_cast<int64_t>(val) << std::endl;
}

// 重载 2: 仅对浮点类型有效 (如 float, double)
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type
formatPrint(T val) {
    std::cout << "[SFINAE - 浮点] " << val << std::endl;
}

// ============================================================================
// 6. C++17 if constexpr (编译期条件分支)
// ============================================================================

template <typename T>
void inspectValue(T val) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "[if constexpr] 整数类型, 翻倍值 = " 
                  << static_cast<int64_t>(val * 2) << std::endl;
    } else if constexpr (std::is_pointer_v<T>) {
        std::cout << "[if constexpr] 指针类型, 解引用值 = " << *val << std::endl;
    } else {
        std::cout << "[if constexpr] 其它类型" << std::endl;
    }
}

// ============================================================================
// 7. CRTP (奇异递归模板模式 - 静态多态，零虚表开销)
// ============================================================================

template <typename Derived>
class ShapeCRTP {
public:
    void render() {
        // 编译期静态派发，无虚函数调用开销
        static_cast<Derived*>(this)->renderImpl();
    }
};

class Square : public ShapeCRTP<Square> {
public:
    void renderImpl() {
        std::cout << "[CRTP 静态多态] 渲染正方形 Square" << std::endl;
    }
};

class Triangle : public ShapeCRTP<Triangle> {
public:
    void renderImpl() {
        std::cout << "[CRTP 静态多态] 渲染三角形 Triangle" << std::endl;
    }
};

// ============================================================================
// 8. 模板元编程：编译期常量计算 (Compile-time Metaprogramming)
// ============================================================================

template <uint32_t N>
struct CompileFactorial {
    static constexpr uint64_t value = N * CompileFactorial<N - 1>::value;
};

// 递归基：全特化终止条件
template <>
struct CompileFactorial<0> {
    static constexpr uint64_t value = 1;
};

// ============================================================================
// 9. C++20 概念与约束 (Concepts & Constraints) - 优雅替代复杂 SFINAE
// ============================================================================
#if __cplusplus >= 202002L
#include <concepts>

template <typename T>
concept FixedInt = std::integral<T> && (sizeof(T) == 4 || sizeof(T) == 8);

template <FixedInt T>
T addFixedInts(T a, T b) {
    return a + b;
}
#endif

// ============================================================================
// main 函数演示
// ============================================================================
int main() {
    std::cout << "===== 1. 变量模板与别名模板 =====" << std::endl;
    int32_t pi_int = static_cast<int32_t>(Pi<double>);
    double pi_double = Pi<double>;
    std::cout << "Pi<double>: " << pi_double << ", 转换为 int32_t: " << pi_int << std::endl;

    Int32Pair<std::string> pair_data{"Code", 72};
    std::cout << "别名模板 pair: (" << pair_data.first << ", " << pair_data.second << ")" << std::endl;

    std::cout << "\n===== 2. 非类型模板参数 (NTTP) 与特化 =====" << std::endl;
    StaticBuffer<int32_t, 3> buf{10, 20, 30};
    std::cout << "StaticBuffer 大小: " << buf.size() << ", 内容: ";
    for (std::size_t i = 0; i < buf.size(); ++i) {
        std::cout << buf[i] << " ";
    }
    std::cout << std::endl;

    TypeInspector<double>::inspect();      // 通用泛型
    TypeInspector<int32_t>::inspect();     // 全特化
    int32_t val = 42;
    TypeInspector<decltype(&val)>::inspect(); // 偏特化 (int32_t*)

    std::cout << "\n===== 3. 变长参数模板与折叠表达式 =====" << std::endl;
    int32_t a = 1, b = 2, c = 3;
    int64_t d = 4;
    auto sum = sumValues(a, b, c, d);
    std::cout << "折叠表达式求和: " << sum << std::endl;
    std::cout << "折叠表达式打印: ";
    printValues(a, b, c, d, "Done");

    std::cout << "\n===== 4. 模板模板参数 =====" << std::endl;
    ContainerAdapter<uint32_t, std::vector> adapter;
    adapter.push(100u);
    adapter.push(200u);
    std::cout << "ContainerAdapter 元素个数: " << adapter.size() << std::endl;

    std::cout << "\n===== 5. SFINAE (std::enable_if) =====" << std::endl;
    int32_t int_val = 123;
    double float_val = 3.14;
    formatPrint(int_val);
    formatPrint(float_val);

    std::cout << "\n===== 6. C++17 if constexpr 编译期分支 =====" << std::endl;
    int32_t num = 50;
    inspectValue(num);
    inspectValue(&num);

    std::cout << "\n===== 7. CRTP 静态多态 =====" << std::endl;
    Square sq;
    Triangle tr;
    sq.render();
    tr.render();

    std::cout << "\n===== 8. 编译期常量计算 (模板元编程) =====" << std::endl;
    constexpr uint64_t fact5 = CompileFactorial<5>::value;
    constexpr uint64_t fact10 = CompileFactorial<10>::value;
    std::cout << "5! = " << fact5 << ", 10! = " << fact10 << std::endl;

#if __cplusplus >= 202002L
    std::cout << "\n===== 9. C++20 Concepts 约束 =====" << std::endl;
    int32_t sum_int = addFixedInts(int32_t{10}, int32_t{20});
    std::cout << "Concepts addFixedInts(10, 20) = " << sum_int << std::endl;
#endif

    return 0;
}
