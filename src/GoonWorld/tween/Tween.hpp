#pragma once
#include <functional>
#include <GoonWorld/interfaces/ITween.hpp>
#include <GoonEngine/debug.h>
#include <GoonEngine/easing.h>

namespace GoonWorld
{
    enum class Easings
    {
        Linear,
        QuadEaseIn,
        QuadEaseOut,
        QuadEaseInOut,
        CubicEaseIn,
        CubicEaseOut,
        CubicEaseInOut,
        QuarticEaseIn,
        QuarticEaseOut,
        QuarticEaseInOut,
        QuinticEaseIn,
        QuinticEaseOut,
        QuinticEaseInOut,
        SineEaseIn,
        SineEaseOut,
        SineEaseInOut,
        CircularEaseIn,
        CircularEaseOut,
        CircularEaseInOut,
        ExponentialEaseIn,
        ExponentialEaseOut,
        ExponentialEaseInOut,
        ElasticEaseIn,
        ElasticEaseOut,
        ElasticEaseInOut,
        BackEaseIn,
        BackEaseOut,
        BackEaseInOut,
        BounceEaseIn,
        BounceEaseOut,
        BounceEaseInOut,
    };

    template <typename T>
    class Tween : public ITween
    {
    public:
        Tween(T &start, T end, double time, Easings easeType);
        ~Tween();
        void Update(double &deltaTimeSeconds) override;
        inline void SetCallback(std::function<void(void *args)> callback) { _callback = callback; }
        inline void SetCallbackArgs(void *args) { _args = args; }
        void Interpolate(double &deltaTimeSeconds);

    private:
        double GetProgressPercent();
        T *_value;
        T _start;
        T _end;
        double _currentTime;
        double _endTime;
        bool _looping;
        bool _completed;
        std::function<void(void *args)> _callback;
        void *_args;
        Easings _easeType;
    };

    template <typename T>
    double Tween<T>::GetProgressPercent()
    {
        switch (_easeType)
        {
        case Easings::Linear:
            return geLinearInterpolation(_currentTime / _endTime);
        case Easings::QuadEaseIn:
            return geQuadraticEaseIn(_currentTime / _endTime);
        case Easings::QuadEaseOut:
            return geQuadraticEaseOut(_currentTime / _endTime);
        default:
            LogWarn("Ease type not implemented! %d", _easeType);
            return 0;
        }
    }

    template <typename T>
    Tween<T>::Tween(T &start, T end, double time, Easings easeType)
        : _value(&start), _start(start), _end(end), _currentTime(0), _endTime(time), _looping(false), _completed(false), _callback(nullptr), _args(nullptr), _easeType(easeType)
    {
    }
    template <typename T>
    Tween<T>::~Tween()
    {
    }

    template <typename T>
    void Tween<T>::Update(double &deltaTimeSeconds)
    {
        if (!_completed)
            Interpolate(deltaTimeSeconds);
    }

}
