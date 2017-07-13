/*
   wqueue.h

   Worker thread queue based on the Standard C++ library list
   template class.

   ------------------------------------------

   Copyright (c) 2013 Vic Hargrave

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef __wqueue_h__
#define __wqueue_h__

#include <pthread.h>
#include <list>
#include <iostream>

using namespace std;

class wqueue
{
    public:
        wqueue();
        ~wqueue();
        void add(const std::string &item);
        std::string remove();
        int size();
        void abort();
    private:
        list<std::string>          m_queue;
        pthread_mutex_t  m_mutex;
        pthread_cond_t   m_condv; 
        bool volatile mIsRunnable;
};

#endif
