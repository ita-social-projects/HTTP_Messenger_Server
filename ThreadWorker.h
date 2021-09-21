//
// Created by pavlo on 9/2/21.
//
#ifndef BACKEND_THREADWORKER_H
#define BACKEND_THREADWORKER_H
#include <vector>
#include <thread>
#include <queue>
#include <memory>
#include <mutex>
#include "./IRequests/IRequests.h"
#include <iostream>

struct ThreadInfo
{
    bool isThreadWorking;
    std::unique_ptr<IRequests> Request;
};


class ThreadWorker
{
private:
    std::queue<IRequests> m_requestsQueue;
    std::vector<std::thread> m_Threads;
    std::thread m_processThreadPool;
    int m_threadsCount;
    std::mutex m_mutex;
    void ProcessPool();
    void InitThreads();
    void ThreadProcess(ThreadInfo);

public:
    ThreadWorker();
    ~ThreadWorker();
    std::vector<ThreadInfo> m_ThreadPool;
    void PushRequest(IRequests req);

};

#endif //BACKEND_THREADWORKER_H
