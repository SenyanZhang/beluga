/**
 * @author Zhang Senyan
 * Date: 2019-04-25
 *
 */

#ifndef BELUGA_ACCEPTTHREADPOOL_H
#define BELUGA_ACCEPTTHREADPOOL_H

#include <cstddef>
#include <vector>
#include <memory>

#include "beluga/thread/AcceptThread.h"
#include "beluga/net/Connection.h"


class AcceptThreadPool {
public:
    AcceptThreadPool(int size,int maxConnSize);
    void setTaskQueue(const std::shared_ptr<TaskQueue>& taskQueue);
    int putConnection(std::shared_ptr<Connection> conn);
    std::shared_ptr<AcceptThread> getMinConnThread();
    void startLoop();
    int size();
    int maxConnSize();
    int connSize();

    void setDropConnectionCallBack(beluga::ConnFunctor dropFunctor);
private:
    std::vector<std::shared_ptr<AcceptThread>> _threads;
    size_t _connNum;
    size_t _maxNum;
};



#endif //BELUGA_ACCEPTTHREADPOOL_H
