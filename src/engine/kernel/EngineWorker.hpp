#ifndef __ENGINE_WORKER_HPP_
#define __ENGINE_WORKER_HPP_

#include <memory>
#include <vector>
#include <algorithm>

namespace vd::kernel
{
    class Observer
    {
    public:
        virtual void init() = 0;
        virtual void update(bool shadowUpdate) = 0;
        virtual void cleanUp() = 0;
    };
    typedef std::shared_ptr<Observer>	ObserverPtr;

    class Observable
    {
    public:
        void subscribe(const ObserverPtr& observer);
        void unsubscribe(const ObserverPtr& observer);

        void broadcastInit();
        void broadcastUpdate(bool shadowUpdate);
        void broadcastCleanUp();
    private:
        std::vector<ObserverPtr> observers;
    };

    class EngineWorker : public Observable
    {
    public:
        EngineWorker();
        ~EngineWorker();

        void init();
        void update(bool shadowUpdate);
        void cleanUp();
    };
    typedef std::shared_ptr<EngineWorker>	EngineWorkerPtr;
}

#endif // !__ENGINE_WORKER_HPP_