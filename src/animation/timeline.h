#ifndef TIMELINE_H
#define TIMELINE_H

#include <algorithm>
#include <vector>

namespace Au{

template<typename T>
struct Keyframe
{
    Keyframe()
    : time(0)
    {}
    Keyframe(unsigned int time)
    : time(time)
    {}
    Keyframe(unsigned int time, T value)
    : time(time), value(value) 
    {}
    
    unsigned int    time;
    T               value;
    
    bool operator<(const Keyframe& other)
    {
        return time < other.time;
    }
    
    Keyframe& operator=(const T& value)
    {
        this->value = value;
        return *this;
    }
};
    
template<typename T>
class InterpolatorFlat
{
public:  
    T operator()(const Keyframe<T>& first, const Keyframe<T>& second, unsigned int time)
    {
        return first.value;
    }
};

template<typename T, typename INTERPOLATOR = InterpolatorFlat<T>>
class Timeline
{
public:
    

    void SetKey(unsigned int time, T value)
    {
        (*this)[time] = value;
    }
    
    void ClearKey(unsigned int time)
    {
        for(unsigned int i = 0; i < timeline.size(); ++i)
        {
            if(timeline[i].time == time)
            {
                timeline.erase(timeline.begin() + i);
                break;
            }
        }
    }
    
    Keyframe<T>& operator[](unsigned int time)
    {
        for(unsigned int i = 0; i < timeline.size(); ++i)
            if(timeline[i].time == time)
                return timeline[i];
        
        timeline.push_back(Keyframe<T>(time));
        std::sort(timeline.begin(), timeline.end());
        
        for(unsigned int i = 0; i < timeline.size(); ++i)
        {
            if(timeline[i].time == time)
                return timeline[i];
        }
        
        return timeline[0];
    }
    
    T Evaluate(unsigned int time)
    {
        if(timeline.empty())
            return T();
        
        Keyframe<T> first;
        Keyframe<T> second;
        
        for(unsigned int i = timeline.size() - 1; i >= 0; --i)
        {
            if(timeline[i].time <= time)
            {
                first = timeline[i];
                if(i == timeline.size() - 1)
                    second = timeline[0];
                else
                    second = timeline[i+1];
                break;
            }
        }
        
        return INTERPOLATOR()(first, second, time);
    }
private:
    std::vector<Keyframe<T>> timeline;
};

}

#endif
