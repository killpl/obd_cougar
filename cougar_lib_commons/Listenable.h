//
//  cougar_bluetooth.h
//  binding
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//

#pragma once

#include <vector>
#include <memory>
#include <mutex>

template<typename T> class Listenable
{
public:
    
    virtual ~Listenable() {}
    
    virtual inline void AddListener(std::shared_ptr<T> listener)
    {
        std::lock_guard<std::mutex> lock(_lock);
        _listeners.push_back(listener);
    }
    
    void RemoveListener(std::shared_ptr<T> listener)
    {
        std::lock_guard<std::mutex> lock(_lock);
        while (_listeners.find(listener) != _listeners.end())
        {
            _listeners.erase(_listeners.find(listener));
        }
    }
    
    void RemoveAllListeners()
    {
        std::lock_guard<std::mutex> lock(_lock);
        _listeners.clear();
    }
  
    
private:
    /**
     * Listeners
     */
    std::vector<std::shared_ptr<T> > _listeners;
    
    /**
     *
     */
    std::mutex _lock;
};
