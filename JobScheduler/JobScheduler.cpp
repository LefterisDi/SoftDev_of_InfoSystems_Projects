#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

#include "./JobScheduler.hpp"

using namespace std;

JobScheduler::JobScheduler(int threadCount, int queueSize){
    
    /* Initialize */
    this->amountOfJobs = 0;
    this->finished = 0;
    this->threadCount = 0;
    this->queueSize = queueSize;
    this->head = this->tail = this->count = 0;
    this->shutdown = this->started = 0;
    this->threads = new pthread_t[threadCount];
    this->queue = new Job[queueSize];
    pthread_mutex_init(&(this->lockMutex), NULL);
    pthread_mutex_init(&(this->waiter), NULL);
    pthread_cond_init(&(this->waiterCond), NULL);
    pthread_cond_init(&(this->signal), NULL);

    // Create threads
    for(int i = 0; i < threadCount; i++) {
        if(pthread_create(&(this->threads[i]), NULL, &JobScheduler::workerThreadCreation, this) != 0) {
            cerr << "Error in initializing threads" << endl;
            throw NULL;
        }
        this->threadCount++;
        this->started++;
    }

}

int JobScheduler::waitAllTasks(int flags){

    int i, err = 0;

    if (this == NULL){
        return THREADPOOL_ERROR;
    }

    if(pthread_mutex_lock(&(this->lockMutex)) != 0) {
      return THREADPOOL_LOCK_ERROR;
    }

    do {
        if(this->shutdown) {
            err = THREADPOOL_SHUTDOWN;
            break;
        }

        this->shutdown = (flags & GRACEFULL) ?
            GRACEFULL_SHUTDOWN : IMMEDIATE_SHUTDOWN;

        if((pthread_cond_broadcast(&(this->signal)) != 0) ||
            (pthread_mutex_unlock(&(this->lockMutex)) != 0)) {
            err = THREADPOOL_LOCK_ERROR;
            break;
        }

        for(i = 0; i < this->threadCount; i++) {
            if(pthread_join(this->threads[i], NULL) != 0) {
                err = THREADPOOL_THREAD_ERROR;
            }
        }
    } while(0);

    return err;
}

int JobScheduler::addNewJob(void (*function)(void *), void *argument){
    int err = 0;
    int next;

    if(this == NULL || function == NULL) {
        return THREADPOOL_ERROR;
    }

    if(pthread_mutex_lock(&(this->lockMutex)) != 0) {
        return THREADPOOL_LOCK_ERROR;
    }

    next = (this->tail + 1) % this->queueSize;

    do {
        //check if full
        if(this->count == this->queueSize) {
            err = THREADPOOL_FULL_QUEUE;
            break;
        }

        //check is we shut down
        if(this->shutdown) {
            err = THREADPOOL_SHUTDOWN;
            break;
        }

        //add a new job to the queue
        this->queue[this->tail].function = function;
        this->queue[this->tail].argument = argument;
        this->tail = next;
        this->count += 1;
        this->amountOfJobs++;

        if(pthread_cond_signal(&(this->signal)) != 0) {
            err = THREADPOOL_LOCK_ERROR;
            break;
        }
    } while(0);

    if(pthread_mutex_unlock(&this->lockMutex) != 0) {
        err = THREADPOOL_LOCK_ERROR;
    }

    return err;
}

int JobScheduler::destroyScheduler(int flags){
    int i, err = 0;

    if(this == NULL) {
        return THREADPOOL_ERROR;
    }

    if(pthread_mutex_lock(&(this->lockMutex)) != 0) {
        return THREADPOOL_LOCK_ERROR;
    }

    do {
        //check for shutdown
        if(this->shutdown) {
            err = THREADPOOL_SHUTDOWN;
            break;
        }

        this->shutdown = (flags & GRACEFULL) ?
            GRACEFULL_SHUTDOWN : IMMEDIATE_SHUTDOWN;

        //wake up the  threads
        if((pthread_cond_broadcast(&(this->signal)) != 0) ||
           (pthread_mutex_unlock(&(this->lockMutex)) != 0)) {
            err = THREADPOOL_LOCK_ERROR;
            break;
        }

        //join the threads
        for(i = 0; i < this->threadCount; i++) {
            if(pthread_join(this->threads[i], NULL) != 0) {
                err = THREADPOOL_THREAD_ERROR;
            }
        }
    } while(0);

    //if nothing went wrong here we deallocate the scheduler
    if(!err) {
        this->deleteScheduler();
    }

    return err;
}

int JobScheduler::deleteScheduler(){

    if(this == NULL || this->started > 0) {
        return -1;
    }

    //make sure we destroyed the threads
    if(this->threads) {
        delete[] this->threads;
        delete[] this->queue;


        pthread_mutex_lock(&(this->lockMutex));
        pthread_mutex_destroy(&(this->lockMutex));
        pthread_cond_destroy(&(this->signal));
    }

    // delete this;
    return 0;
}


void* JobScheduler::workerThread(void){

    Job task;

    while(1) {
        //lock mutex
        pthread_mutex_lock(&(this->lockMutex));

        //wait for condition variable so we can own the mutex
        while((this->count == 0) && (!this->shutdown)) {
            pthread_cond_wait(&(this->signal), &(this->lockMutex));
        }

        if((this->shutdown == IMMEDIATE_SHUTDOWN) ||
           ((this->shutdown == GRACEFULL_SHUTDOWN) &&
            (this->count == 0))) {
            break;
        }

        //take a job from the queue
        task.function = this->queue[this->head].function;
        task.argument = this->queue[this->head].argument;
        this->head = (this->head + 1) % this->queueSize;
        this->count -= 1;


        pthread_mutex_unlock(&(this->lockMutex));

        //execute the job
        (*(task.function))(task.argument);
    }
    this->started--;

    pthread_mutex_unlock(&(this->lockMutex));
    pthread_exit(NULL);

    return(NULL);
}
