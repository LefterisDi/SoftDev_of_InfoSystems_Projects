#ifndef _JOBSCHEDULER_H_
#define _JOBSCHEDULER_H_

#define THREADPOOL_ERROR -1
#define THREADPOOL_LOCK_ERROR -2
#define THREADPOOL_FULL_QUEUE -3
#define THREADPOOL_SHUTDOWN -4
#define THREADPOOL_THREAD_ERROR -5
#define GRACEFULL_SHUTDOWN 2
#define IMMEDIATE_SHUTDOWN 1
#define GRACEFULL 1

typedef struct Job{
    void (*function)(void *);
    void *argument;
} Job;


 class JobScheduler {

  private:
    pthread_mutex_t lockMutex;
    pthread_cond_t signal;
    pthread_mutex_t waiter;
    pthread_cond_t waiterCond;
    pthread_t *threads;
    Job *queue;
    int threadCount;
    int amountOfJobs;
    int tail;
    int head;
    int count;
    int shutdown;
    int queueSize;
    int started;
    
    int deleteScheduler();

  public:
    int finished = 0;

    JobScheduler(int, int);
    int waitAllTasks(int = 1);
    int addNewJob(void (*)(void *), void *);
    int destroyScheduler(int = 0);
    void* workerThread(void);

    static void* workerThreadCreation(void* js){
      return ((JobScheduler *)js)->workerThread();
    };

};


#endif
