#include <vector>
#include <thread>
#include <queue>
#include <memory>
#include <mutex>

#include <iostream> // For Debug
#include "IRequests.h"
typedef std::shared_ptr<IRequests> Request;

struct ThreadInfo
{
    bool isThreadWorking;
    IRequests* Request;
    ThreadInfo()
    {
        isThreadWorking = false;
        Request = nullptr;
    }

    void set_Request(IRequests*  req)
    {
        Request = req;
    }
    ~ThreadInfo()
    {
        isThreadWorking = false;
    }
};
typedef std::vector<std::shared_ptr<ThreadInfo>> VSptrThreadInfo;
typedef std::queue<std::unique_ptr<IRequests>> IRequestsQueue;

class ThreadWorker
{
private:
    IRequestsQueue m_requestsQueue;
    IRequests* globalRequest;
    std::vector<std::thread> m_Threads;
    std::thread m_processThreadPool;
    int m_threadsCount;
    std::mutex m_mutex;
    void ProcessPool();
    void InitThreads();
    void ThreadProcess(std::shared_ptr<ThreadInfo>  threadInfo);
    void JoinThreads();
public:
    ThreadWorker();
    ~ThreadWorker();
    VSptrThreadInfo m_ThreadPool;
    template <typename T>
    void PushRequest(T*);

};

template <typename T>
void ThreadWorker::PushRequest(T* request)
{
    m_mutex.lock();
    m_requestsQueue.push(std::make_unique<T> (*request));
    m_mutex.unlock();
}

