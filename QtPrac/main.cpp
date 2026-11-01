#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QDebug>
#include <iostream>

// ============================================================================
// 1. 发送者类 (Sender)：定义并触发信号
// ============================================================================
class Newspaper : public QObject
{
    // 必须包含 Q_OBJECT 宏，Qt 元对象系统（moc）才能处理信号与槽
    Q_OBJECT

public:
    explicit Newspaper(const QString &name, QObject *parent = nullptr)
        : QObject(parent), name_(name) {}

    // 业务方法：发布新闻，向外发射信号
    void publishNews(const QString &title, int issueNumber)
    {
        qDebug() << "\n[" << name_ << "] 发布了第" << issueNumber << "期报纸: 《" << title << "》";
        // 使用 emit 关键字发射信号（通知所有连接的槽函数）
        emit newsPublished(title, issueNumber);
    }

    void announceEmergency(const QString &msg)
    {
        emit emergencyAlert(msg);
    }

// 信号区 (signals): 只需声明，绝不能自己实现函数体！
signals:
    // 信号 1: 带有两个参数 (新闻标题, 期号)
    void newsPublished(const QString &title, int issueNumber);

    // 信号 2: 紧急消息广播
    void emergencyAlert(const QString &msg);

private:
    QString name_;
};

// ============================================================================
// 2. 接收者类 (Receiver)：定义处理信号的“槽函数 (Slot)”
// ============================================================================
class Reader : public QObject
{
    Q_OBJECT

public:
    explicit Reader(const QString &name, QObject *parent = nullptr)
        : QObject(parent), name_(name) {}

// 槽函数区 (slots): 接收信号并执行具体业务代码
public slots:
    void onNewsReceived(const QString &title, int issueNumber)
    {
        qDebug() << "  -> [读者" << name_ << "] 收到了报纸，正在阅读: 《" 
                 << title << "》(第" << issueNumber << "期)";
    }

    // 槽函数的参数可以少于信号的参数（自动忽略多余参数）
    void onSimpleNotice(const QString &title)
    {
        qDebug() << "  -> [读者" << name_ << "] 只关心标题: 《" << title << "》";
    }

private:
    QString name_;
};

// ============================================================================
// 主函数：演示现代 Qt 信号与槽的核心特性
// ============================================================================
int main(int argc, char *argv[])
{
    // QCoreApplication 负责初始化 Qt 核心事件循环
    QCoreApplication a(argc, argv);

    Newspaper dailyPost("每日晨报");
    Reader alice("Alice");
    Reader bob("Bob");

    std::cout << "==================================================" << std::endl;
    std::cout << "      Qt 信号与槽 (Signals & Slots) 全景演示      " << std::endl;
    std::cout << "==================================================" << std::endl;

    // ------------------------------------------------------------------------
    // 特性 1：现代 Qt5/Qt6 标准连接（类型安全，基于函数指针）
    // 语法：QObject::connect(发送者, &类名::信号, 接收者, &类名::槽函数);
    // ------------------------------------------------------------------------
    QObject::connect(&dailyPost, &Newspaper::newsPublished,
                     &alice, &Reader::onNewsReceived);

    // ------------------------------------------------------------------------
    // 特性 2：一对多连接（一个信号可以同时连接多个接收者）
    // ------------------------------------------------------------------------
    QObject::connect(&dailyPost, &Newspaper::newsPublished,
                     &bob, &Reader::onNewsReceived);

    // ------------------------------------------------------------------------
    // 特性 3：参数裁剪（槽函数的形参个数可以比信号少，自动忽略尾部参数）
    // ------------------------------------------------------------------------
    QObject::connect(&dailyPost, &Newspaper::newsPublished,
                     &bob, &Reader::onSimpleNotice);

    // ------------------------------------------------------------------------
    // 特性 4：C++11 Lambda 表达式直接作为槽函数（极为轻量高效）
    // ------------------------------------------------------------------------
    QObject::connect(&dailyPost, &Newspaper::emergencyAlert, [](const QString &msg) {
        qDebug() << "  -> [Lambda 槽触发] 收到突发紧急推送:" << msg;
    });

    // ------------------------------------------------------------------------
    // 特性 5：保存连接句柄，支持手动断开 (disconnect)
    // ------------------------------------------------------------------------
    auto connAlice = QObject::connect(&dailyPost, &Newspaper::emergencyAlert,
                                      [](const QString &msg) {
                                          qDebug() << "  -> [临时监听器] 记录警报:" << msg;
                                      });

    // --- 第一次测试发布 ---
    dailyPost.publishNews("人工智能在嵌入式系统的全面落地", 101);
    dailyPost.announceEmergency("暴雨红色预警，暂停派送！");

    // --- 断开连接演示 ---
    std::cout << "\n>>> 正在断开 Bob 的订阅，并移除临时监听器..." << std::endl;
    QObject::disconnect(&dailyPost, &Newspaper::newsPublished, &bob, &Reader::onNewsReceived);
    QObject::disconnect(connAlice); // 通过连接句柄断开

    // --- 第二次测试发布 ---
    dailyPost.publishNews("C++26 带来革命性反射特性", 102);
    dailyPost.announceEmergency("暴雨警报解除。");

    return 0;
}

// 注意：如果将 Q_OBJECT 类直接写在 .cpp 文件中，必须在末尾引入此 moc 文件
// 让 CMake AUTOMOC 能够自动生成并合并元对象编译代码
#include "main.moc"
