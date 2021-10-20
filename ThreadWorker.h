#include <vector>
#include <thread>
#include <queue>
#include <memory>
#include <mutex>

#include <iostream> // For Debug
#include "AnswerContainer.h"
typedef std::shared_ptr<AnswerContainer> Request;

struct ThreadInfo
{
    bool isThreadWorking;
    AnswerContainer* Request;
    ThreadInfo();

    void set_Request(AnswerContainer* req);

    void ProcessRequest();

    ~ThreadInfo();
};
typedef std::vector<std::shared_ptr<ThreadInfo>> VSptrThreadInfo;
typedef std::queue<std::unique_ptr<AnswerContainer>> AnswerContainerQueue;

class ThreadWorker
{
private:
    AnswerContainerQueue m_requestsQueue;
    std::vector<std::thread> m_Threads;
    std::thread m_processThreadPool;
    int m_threadsCount;
    std::mutex m_mutex;
    void ProcessPool();
    void InitThreads();
    void ThreadProcess(std::shared_ptr<ThreadInfo>  threadInfo);
    void DetachThreads();
public:
    ThreadWorker();
    ~ThreadWorker();
    VSptrThreadInfo m_ThreadPool;
    void PushRequest(AnswerContainer*);

};



