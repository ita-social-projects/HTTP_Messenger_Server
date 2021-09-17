#include "ThreadWorker.h"

ThreadWorker::ThreadWorker()
{
    m_threadsCount = std::thread::hardware_concurrency();
    m_processThreadPool = std::thread(&ThreadWorker::ProcessPool, this);
    //TODO Log threads count
    std::cout << m_threadsCount << std::endl;
    InitThreads();
}

void ThreadWorker::PushRequest(IRequests request)
{
    m_mutex.lock();
    m_requestsQueue.push(request);
    m_mutex.lock();
}

void ThreadWorker::InitThreads()
{
    for (size_t i = 0; i < m_threadsCount - 2; i++)
    {
        m_Threads.push_back(std::thread(&ThreadWorker::ThreadProcess, this, ThreadInfo()));
    }

}

ThreadWorker::~ThreadWorker()
{
    m_processThreadPool.detach();
    for (auto& thread : m_Threads)
    {
        thread.detach();
    }
    //TODO Log finished
}

void ThreadWorker::ProcessPool()
{
    while (true)
    {
        if (!m_requestsQueue.empty())
        {
            for (auto& thread : m_ThreadPool)
            {
                if (!thread.isThreadWorking)
                {
                    m_mutex.lock();
                    thread.Request = std::make_unique<IRequests> (m_requestsQueue.front());
                    m_requestsQueue.pop();
                    m_mutex.unlock();
                }
            }
        }
    }
}

void ThreadWorker::ThreadProcess(ThreadInfo threadInfo)
{
    while (true)
    {
        if (threadInfo.isThreadWorking)
        {
            threadInfo.Request->DoStuff();
            threadInfo.Request.reset();
            threadInfo.isThreadWorking = false;
        }
    }
}