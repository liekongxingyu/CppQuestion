#!/usr/bin/env python3
import socket
import time
import threading

def client_worker(client_id, stop_event):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect(('127.0.0.1', 8080))
        welcome = s.recv(1024).decode('utf-8', errors='ignore')
        print(f"[Client {client_id}] 收到欢迎消息:\n{welcome}")

        # 发送单播 / Echo 消息
        time.sleep(0.5)
        s.sendall(f"Hello from client {client_id}\n".encode('utf-8'))
        reply = s.recv(1024).decode('utf-8', errors='ignore')
        print(f"[Client {client_id}] 收到服务器响应: {reply.strip()}")

        # 如果是 Client 1，发送一条广播消息
        if client_id == 1:
            time.sleep(0.5)
            s.sendall(b"all:Everyone hello, I am Client 1!\n")
            print(f"[Client 1] 已发送广播请求: all:Everyone hello, I am Client 1!")

        # 接收广播或其他消息
        s.settimeout(2.0)
        while not stop_event.is_set():
            try:
                data = s.recv(1024)
                if not data:
                    break
                print(f"[Client {client_id}] 接收到数据: {data.decode('utf-8', errors='ignore').strip()}")
            except socket.timeout:
                break
    except Exception as e:
        print(f"[Client {client_id}] 异常: {e}")
    finally:
        s.close()
        print(f"[Client {client_id}] 连接已关闭。")

def main():
    print(">>> 启动 3 个并发客户端测试服务器多连接支持...")
    threads = []
    stop_event = threading.Event()

    for i in range(1, 4):
        t = threading.Thread(target=client_worker, args=(i, stop_event))
        t.start()
        threads.append(t)
        time.sleep(0.2)

    time.sleep(3.0)
    stop_event.set()

    for t in threads:
        t.join()

    print(">>> 并发客户端测试完成。")

if __name__ == '__main__':
    main()
