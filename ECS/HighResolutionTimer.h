#pragma once
#include <Windows.h>


class HighResolutionTimer
{
public:
    HighResolutionTimer();
    double GetFrequency() const;
    void Start() ;
    double Stop();
    double GetTime() const;

private:
    LARGE_INTEGER start;
    LARGE_INTEGER stop;
    double frequency;
};

HighResolutionTimer::HighResolutionTimer()
{
    frequency = GetFrequency();
}

double HighResolutionTimer::GetFrequency() const
{
    LARGE_INTEGER proc_freq;
    if (!::QueryPerformanceFrequency(&proc_freq))
        return -1;
    return static_cast< double >( proc_freq.QuadPart );
}

void HighResolutionTimer::Start()
{
    DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
    ::QueryPerformanceCounter(&start);
    ::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
}

double HighResolutionTimer::Stop()
{
    DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
    ::QueryPerformanceCounter(&stop);
    ::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
    return ((stop.QuadPart - start.QuadPart) / frequency);
} 

double HighResolutionTimer::GetTime() const
{
    LARGE_INTEGER time;
    ::QueryPerformanceCounter(&time);
    return time.QuadPart / frequency;
}