#ifndef __WQUEUE_H__
#define __WQUEUE_H__

#include <pthread.h>
#include <list>
#include <iostream>

using namespace std;

class Wqueue
{
    public:
        Wqueue();
        ~Wqueue();
        void add(const std::string &item);
        std::string remove();
        int size();
        void abort();
    private:
        list<std::string>          m_queue;
        pthread_mutex_t  m_mutex;
        //pthread_cond_t   m_condv; 
};

#endif
