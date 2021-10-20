#include "ThreadWorker.h"


ThreadWorker::ThreadWorker()
{
    m_threadsCount = std::thread::hardware_concurrency();
    InitThreads();
}

using std::min;
void ThreadWorker::InitThreads()
{
    int ThreadsCount = min(m_threadsCount, 1);
   
    for (int i = 0; i < ThreadsCount; i++)
    {
        std::shared_ptr<ThreadInfo> tempShared = std::make_shared<ThreadInfo>(ThreadInfo());
        m_Threads.push_back(std::thread(&ThreadWorker::ThreadProcess, this, tempShared));
        m_ThreadPool.push_back(tempShared);
    }
    DetachThreads();
    LOG_DEBUG("Programm is using such count of threads: ", ThreadsCount);
}

void ThreadWorker::DetachThreads()
{
    for (auto& thread : m_Threads)
    {
        thread.detach();
    }
}

ThreadWorker::~ThreadWorker()
{}


void ThreadWorker::ProcessPool()
{
        if (!m_requestsQueue.empty())
        {
            for (auto& threadInfo : m_ThreadPool)
            {
                if (!threadInfo->isThreadWorking && !m_requestsQueue.empty())
                {
                    AnswerContainer* tempr = m_requestsQueue.front().get();
                    m_requestsQueue.front().release();
                    std::unique_lock<std::mutex> lock(m_mutex);
                    threadInfo->set_Request(tempr);
                    threadInfo->isThreadWorking = true;
                    m_requestsQueue.pop();
                    lock.unlock();
                }
            }
        }
}

void ThreadWorker::ThreadProcess(std::shared_ptr<ThreadInfo> threadInfo)
{
    while (true)
    {
        ProcessPool();
        if (threadInfo->isThreadWorking)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            threadInfo->ProcessRequest();
            LOG_DEBUG("Request Processed successfully");
            lock.unlock();
        }
    }
}
void ThreadWorker::PushRequest(AnswerContainer* request)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    LOG_DEBUG("New request pushed");
    m_requestsQueue.push(std::make_unique<AnswerContainer>(*request));
    lock.unlock();
}


ThreadInfo::ThreadInfo()
{
    isThreadWorking = false;
    Request = nullptr;
}

void ThreadInfo::set_Request(AnswerContainer* req)
{
    Request = req;
}

void ThreadInfo::ProcessRequest()
{
    Request->ProcessRequest();
    Request->RespondOnRequest();
    isThreadWorking = false;
    delete Request;
    Request = nullptr;
}

ThreadInfo::~ThreadInfo()
{
    isThreadWorking = false;
}