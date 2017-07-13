#include "wqueue.h"
#include <unistd.h>
wqueue::wqueue()
{
    mIsRunnable = true;
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_condv, NULL);
}
wqueue::~wqueue() 
{
    std::cout << "wqueue::~wqueue" << std::endl;
    pthread_mutex_lock(&m_mutex);
    std::cout << "wqueue::~wqueue mIsRunnable = false" << std::endl;
    mIsRunnable = false;
    //pthread_cond_signal(&m_condv);
    std::cout << "wqueue::remove 7" << std::endl;
    pthread_mutex_unlock(&m_mutex);
    std::cout << "wqueue::remove 8" << std::endl;
    pthread_mutex_destroy(&m_mutex);
    std::cout << "wqueue::remove 9" << std::endl;
    pthread_cond_destroy(&m_condv);
    std::cout << "wqueue::~wqueue finished" << std::endl;
}
void wqueue::add(const std::string &item) 
{
    pthread_mutex_lock(&m_mutex);
    m_queue.push_back(item);
    //pthread_cond_signal(&m_condv);
    pthread_mutex_unlock(&m_mutex);
}
std::string wqueue::remove() 
{
    pthread_mutex_lock(&m_mutex);
    std::cout << "wqueue::remove 0" << std::endl;
    if(m_queue.size() == 0)
    {
        pthread_mutex_unlock(&m_mutex);
        return "";
    }
    /*
    while (m_queue.size() == 0 && mIsRunnable) {
        std::cout << "wqueue::remove size0" << std::endl;
        pthread_cond_wait(&m_condv, &m_mutex);
        std::cout << "wqueue::remove 10" << std::endl;
    }
    std::cout << "wqueue::remove 1" << std::endl;
    if(!mIsRunnable)
    {
    std::cout << "wqueue::remove 2" << std::endl;
    pthread_mutex_unlock(&m_mutex);
        return "";
    }
    */
    std::cout << "wqueue::remove 3" << std::endl;
    std::string item = m_queue.front();
    std::cout << "wqueue::remove 4" << std::endl;
    m_queue.pop_front();
    std::cout << "wqueue::remove 5" << std::endl;
    pthread_mutex_unlock(&m_mutex);
    std::cout << "wqueue::remove 6" << std::endl;
    return item;
}
int wqueue::size() {
    pthread_mutex_lock(&m_mutex);
    int size = m_queue.size();
    pthread_mutex_unlock(&m_mutex);
    return size;
}
void wqueue::abort()
{
    pthread_mutex_lock(&m_mutex);
    mIsRunnable = false;
    //pthread_cond_signal(&m_condv);
    pthread_mutex_unlock(&m_mutex);
}
