#ifndef STOPWATCH_H
#define STOPWATCH_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Stopwatch                                                           //
//                                                                      //
// Stopwatch class. This class returns the real and cpu time between    //
// the start and stop events.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <stdexcept>

#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
static double gTicks = 0;

class Stopwatch {
public:
  Stopwatch() {
    if (gTicks <= 0.0)
      gTicks = static_cast<double>(sysconf(_SC_CLK_TCK));

    fStopRealTime = 0;
    fStopCpuTime = 0;

    Start();
  }
  void Start(bool reset = true) {
    if (reset) {
      fState = State::undefined;
      fTotalCpuTime = 0;
      fTotalRealTime = 0;
      fCounter = 0;
    }
    if (fState != State::running) {
      fStartRealTime = GetRealTime();
      fStartCpuTime = GetCPUTime();
    }
    fState = State::running;
    fCounter++;
  }

  void Stop() {
    fStopRealTime = GetRealTime();
    fStopCpuTime = GetCPUTime();

    if (fState == State::running) {
      fTotalCpuTime += fStopCpuTime - fStartCpuTime;
      fTotalRealTime += fStopRealTime - fStartRealTime;
    }
    fState = State::stopped;
  }

  void Continue() {
    if (fState == State::undefined)
      throw std::runtime_error("stopwatch not started");

    if (fState == State::stopped) {
      fTotalCpuTime -= fStopCpuTime - fStartCpuTime;
      fTotalRealTime -= fStopRealTime - fStartRealTime;
    }

    fState = State::running;
  }

  int Counter() const { return fCounter; }

  double RealTime() {
    if (fState == State::undefined)
      throw std::runtime_error("stopwatch not started");

    if (fState == State::running)
      Stop();

    return fTotalRealTime;
  }

  void Reset() {
    ResetCpuTime();
    ResetRealTime();
  }

  void ResetCpuTime(double time = 0) {
    Stop();
    fTotalCpuTime = time;
  }

  void ResetRealTime(double time = 0) {
    Stop();
    fTotalRealTime = time;
  }

  double CpuTime() {
    if (fState == State::undefined)
      throw std::runtime_error("stopwatch not started");

    if (fState == State::running)
      Stop();

    return fTotalCpuTime;
  }

  void Print(const char *option = "") const {
    double realt = const_cast<Stopwatch *>(this)->RealTime();
    double cput = const_cast<Stopwatch *>(this)->CpuTime();

    int hours = int(realt / 3600);
    realt -= hours * 3600;
    int min = int(realt / 60);
    realt -= min * 60;
    int sec = int(realt);

    if (realt < 0)
      realt = 0;
    if (cput < 0)
      cput = 0;

    if (option && *option == 'm') {
      if (Counter() > 1) {
        printf("Real time %d:%02d:%06.3f, CP time %.3f, %d slices\n", hours,
               min, realt, cput, Counter());
      } else {
        printf("Real time %d:%02d:%06.3f, CP time %.3f\n", hours, min, realt,
               cput);
      }
    } else if (option && *option == 'u') {
      if (Counter() > 1) {
        printf("Real time %d:%02d:%09.6f, CP time %.3f, %d slices\n", hours,
               min, realt, cput, Counter());
      } else {
        printf("Real time %d:%02d:%09.6f, CP time %.3f\n", hours, min, realt,
               cput);
      }
    } else {
      if (Counter() > 1) {
        printf("Real time %d:%02d:%02d, CP time %.3f, %d slices\n", hours, min,
               sec, cput, Counter());
      } else {
        printf("Real time %d:%02d:%02d, CP time %.3f\n", hours, min, sec, cput);
      }
    }
  }

private:
  enum class State { undefined, stopped, running };

  static double GetRealTime() {
    struct timeval tp;
    gettimeofday(&tp, 0);
    return tp.tv_sec + 1e-6 * tp.tv_usec;
  }

  static double GetCPUTime() {
    struct tms cpt;
    times(&cpt);
    return static_cast<double>(cpt.tms_utime + cpt.tms_stime) / gTicks;
  }

  double fStartRealTime; // wall clock start time
  double fStopRealTime;  // wall clock stop time
  double fStartCpuTime;  // cpu start time
  double fStopCpuTime;   // cpu stop time
  double fTotalCpuTime;  // total cpu time
  double fTotalRealTime; // total real time
  State fState;          // stopwatch state
  int fCounter;          // number of times the stopwatch was started
};

#endif
