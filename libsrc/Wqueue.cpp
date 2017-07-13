#include "Wqueue.h"
#include <unistd.h>
Wqueue::Wqueue()
{
    pthread_mutex_init(&m_mutex, NULL);
    //pthread_cond_init(&m_condv, NULL);
}
Wqueue::~Wqueue() 
{
    std::cout << "Wqueue::~Wqueue" << std::endl;
    pthread_mutex_lock(&m_mutex);
    //pthread_cond_signal(&m_condv);
    pthread_mutex_unlock(&m_mutex);
    pthread_mutex_destroy(&m_mutex);
    //pthread_cond_destroy(&m_condv);
    std::cout << "Wqueue::~Wqueue finished" << std::endl;
}
void Wqueue::add(const std::string &item) 
{
    pthread_mutex_lock(&m_mutex);
    m_queue.push_back(item);
    //pthread_cond_signal(&m_condv);
    pthread_mutex_unlock(&m_mutex);
}
std::string Wqueue::remove() 
{
    pthread_mutex_lock(&m_mutex);
    //std::cout << "Wqueue::remove 0" << std::endl;
    if(m_queue.size() == 0)
    {
        pthread_mutex_unlock(&m_mutex);
        return "";
    }
    //std::cout << "Wqueue::remove 3" << std::endl;
    std::string item = m_queue.front();
    //std::cout << "Wqueue::remove 4" << std::endl;
    m_queue.pop_front();
    //std::cout << "Wqueue::remove 5" << std::endl;
    pthread_mutex_unlock(&m_mutex);
    //std::cout << "Wqueue::remove 6" << std::endl;
    return item;
}
int Wqueue::size() {
    pthread_mutex_lock(&m_mutex);
    int size = m_queue.size();
    pthread_mutex_unlock(&m_mutex);
    return size;
}
void Wqueue::abort()
{
    pthread_mutex_lock(&m_mutex);
    //pthread_cond_signal(&m_condv);
    pthread_mutex_unlock(&m_mutex);
}
