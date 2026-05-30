/**
 * this is a implementation of the mediator pattern
 */
#include<queue>
#include<stack>
typedef std::deque<std::pair<Process*,ProcessExecutionConfig>> ProcessExecutionPoolStructure;
struct ProcessConfig {
    int64_t memPool;
    int reqCycles;
};
struct ProcessExecutionConfig{
    char state;
    int remainingCycles; 
};
class Process;
class ProcessMediator;

template <typename T>
class Mediator {
    public :
        virtual void notify(T*) = 0;
 };


 class ProcessStrategy {
    public :
        virtual void exec(Process*,ProcessExecutionPoolStructure*) const = 0;
 };

 class ProcessStartStrategy : public ProcessStrategy{
    public:
        void exec(Process* proc,ProcessExecutionPoolStructure* execPool) const override{
                return;
        }
 } ;

  class ProcessStopStrategy : public ProcessStrategy{
    public:
        void exec(Process* proc,ProcessExecutionPoolStructure* execPool) const override {
            ProcessExecutionPoolStructure::iterator procIter = execPool->begin();
            for(;procIter != execPool->end();procIter++){
                   if(procIter->first == proc){
                        execPool->erase(procIter);
                        break;
                   }
            }
             return;
        }
 } ;

  class ProcessPauseStrategy : public ProcessStrategy{
    public:
        void exec(Process* proc,ProcessExecutionPoolStructure* execPool) const override{
            return;
        }
 } ;

 class ProcessStrategyRegistry{
    public :
        static ProcessStrategy* procPause; 
        static ProcessStrategy* procStop; 
        static ProcessStrategy* procStart;
        static void Initialize(){
            ProcessStrategyRegistry::procStart = new ProcessStartStrategy();
            ProcessStrategyRegistry::procStop = new ProcessStopStrategy();
            ProcessStrategyRegistry::procPause = new ProcessPauseStrategy();
        } 
 };


 class Process {
    private:
        ProcessMediator* mediator;
        ProcessConfig config;
        ProcessStrategy* strategy;
    public:
        Process(ProcessConfig conf){
            this->config  = conf;
        }
        ProcessStrategy* getStrategy(){
            return this->strategy;
        }
        ProcessConfig* getConfig(){
            return &(this->config);
        }
        ProcessMediator* getProcessMediator(){
            return this->mediator;
        }
        void addProcess(ProcessMediator* mediator){
            this->mediator = mediator;
            if(mediator != nullptr){
                this->mediator->addProcess(this);
            }
        }
        void processStart(){
           this->strategy = ProcessStrategyRegistry::procStart;
           if(mediator == nullptr)return;
           this->mediator->notify(this);
        }
        void processStop(){
           this->strategy = ProcessStrategyRegistry::procStop;
           if(mediator == nullptr)return;
           this->mediator->notify(this);
        }
        void processPause(){
           this->strategy = ProcessStrategyRegistry::procPause;
           if(mediator == nullptr)return;
           this->mediator->notify(this);
        }
 };



 class ProcessMediator : public Mediator<Process>{
    private :
        std::deque<Process*> processPool;
        int64_t memPool;
        int64_t remMemPool;
        ProcessExecutionPoolStructure executionPool;
    public:
        ProcessMediator(int64_t memory){
            ProcessStrategyRegistry::Initialize();
            memPool = memory; 
            remMemPool= memPool;
        };
        void notify(Process* proc){
            proc->getStrategy()->exec(proc, &executionPool);
        }
        void addProcess(Process* proc){
            // logic to add process to processPool
        }

        void findAndloadProcToExecPool(){
            std::stack<Process*> leftOverStack;
            while(processPool.size() >0 && remMemPool >0 ){
                Process* proc = processPool.front();
                processPool.pop_front();
                ProcessConfig conf = *proc->getConfig();
                if(conf.memPool > memPool){
                    
                    delete(proc); // deletes the process ;
                    continue;
                } 

                if(conf.memPool > remMemPool){
                    leftOverStack.push(proc);
                    continue;
                }

                ProcessExecutionConfig confExec;
                confExec.remainingCycles = conf.reqCycles;
                confExec.state = 'L';
                executionPool.emplace_back(proc, confExec);
            }

            while(leftOverStack.size() > 0){
                Process* proc = leftOverStack.top();
                leftOverStack.pop();
                processPool.push_front(proc);
            }
        }
        

        void executeCurrentCycle(){
            ProcessExecutionPoolStructure::iterator procIter = executionPool.begin();
            std::queue<Process*> procStopQueue;
            for(;procIter != executionPool.end();procIter++){
                 if(procIter->second.state == 'L'){
                    procIter->first->processStart();
                 }else if (procIter->first->getStrategy() == ProcessStrategyRegistry::procStart){
                    procIter->second.remainingCycles-=1;
                    if(procIter->second.remainingCycles == 0){
                        procStopQueue.push(procIter->first);

                    }
                }
            }
            while(procStopQueue.size() >0){
                Process* front = procStopQueue.front();
                procStopQueue.pop();
                front->processStop();
            }
        }

        
        void runProcessScheduler(){

            while (true){
                if(processPool.size() == 0){
                    //sleep for 1 sec wait for process to be added
                }else{
                    // pick up from pool nextCycle:
                    if(executionPool.size() >0){
                        this->executeCurrentCycle();
                    }
                    this->findAndloadProcToExecPool();
                }
            };
        }
 };




 /**
  * @brief 
  * 
  * this pattern is to implement a mediator design pattern however this mediator pattern is not a strict mediator pattern becaue:
  *     the mediation function (notify ) delegates most of the tasks to strategies{making notify thin} and 
  *        the eventloop handles the remainign processes but in a clean mediator the notify should be the one managing other processes
  *        the initation happens through processes not mediator
  *  
  *  why it is mediator like:
  *     - processes dont communicate with each other directly, mediator mediates the messages,
  *     - the mediator class commands all the processes
  */



  /***
   * CHAT GPT RESPONSE ON DESIGN REVIEW:
   * 
   * /**
            * Verdict:
            * This implementation demonstrates a solid Mediator-based design where ProcessMediator
            * acts as the central authority coordinating process lifecycle, scheduling, and resource allocation.
            * Processes do not interact directly and instead communicate via the mediator, aligning with core Mediator principles.
            * The scheduler loop and shared execution pool reinforce mediator-driven system control.
            * The use of Strategy introduces flexibility for process actions but slightly diffuses mediator responsibility.
            * Overall, this is a strong mediator-inspired architecture with elements of command-style behavior.
            * With minor refinements (clearer responsibility boundaries and memory safety), it approaches production-quality design.
            */
   