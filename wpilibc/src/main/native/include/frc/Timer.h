/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <units/units.h>
#include <wpi/deprecated.h>
#include <wpi/mutex.h>

#include "frc/Base.h"

namespace frc {

using TimerInterruptHandler = void (*)(void* param);

/**
 * Pause the task for a specified time.
 *
 * Pause the execution of the program for a specified period of time given in
 * seconds. Motors will continue to run at their last assigned values, and
 * sensors will continue to update. Only the task containing the wait will pause
 * until the wait time is expired.
 *
 * @param seconds Length of time to pause, in seconds.
 */
void Wait(double seconds);

/**
 * @brief  Gives real-time clock system time with nanosecond resolution
 * @return The time, just in case you want the robot to start autonomous at 8pm
 *         on Saturday.
 */
double GetTime();

/**
 * Timer objects measure accumulated time in seconds.
 *
 * The timer object functions like a stopwatch. It can be started, stopped, and
 * cleared. When the timer is running its value counts up in seconds. When
 * stopped, the timer holds the current value. The implementation simply records
 * the time when started and subtracts the current time whenever the value is
 * requested.
 */
class Timer {
 public:
  /**
   * Create a new timer object.
   *
   * Create a new timer object and reset the time to zero. The timer is
   * initially not running and must be started.
   */
  Timer();

  virtual ~Timer() = default;

  Timer(const Timer& rhs);
  Timer& operator=(const Timer& rhs);
  Timer(Timer&& rhs);
  Timer& operator=(Timer&& rhs);

  /**
   * Get the current time from the timer. If the clock is running it is derived
   * from the current system clock the start time stored in the timer class. If
   * the clock is not running, then return the time when it was last stopped.
   *
   * @return Current time value for this timer in seconds
   */
  double Get() const;

  /**
   * Reset the timer by setting the time to 0.
   *
   * Make the timer startTime the current time so new requests will be relative
   * to now.
   */
  void Reset();

  /**
   * Start the timer running.
   *
   * Just set the running flag to true indicating that all time requests should
   * be relative to the system clock.
   */
  void Start();

  /**
   * Stop the timer.
   *
   * This computes the time as of now and clears the running flag, causing all
   * subsequent time requests to be read from the accumulated time rather than
   * looking at the system clock.
   */
  void Stop();

  /**
   * Check if the period specified has passed and if it has, advance the start
   * time by that period. This is useful to decide if it's time to do periodic
   * work without drifting later by the time it took to get around to checking.
   *
   * @param period The period to check for (in seconds).
   * @return       True if the period has passed.
   */
  WPI_DEPRECATED("Use unit-safe HasPeriodPassed method instead.")
  bool HasPeriodPassed(double period);

  /**
   * Check if the period specified has passed and if it has, advance the start
   * time by that period. This is useful to decide if it's time to do periodic
   * work without drifting later by the time it took to get around to checking.
   *
   * @param period The period to check for.
   * @return       True if the period has passed.
   */
  bool HasPeriodPassed(units::second_t period);

  /**
   * Return the FPGA system clock time in seconds.
   *
   * Return the time from the FPGA hardware clock in seconds since the FPGA
   * started. Rolls over after 71 minutes.
   *
   * @returns Robot running time in seconds.
   */
  static double GetFPGATimestamp();

  /**
   * Return the approximate match time.
   *
   * The FMS does not send an official match time to the robots, but does send
   * an approximate match time. The value will count down the time remaining in
   * the current period (auto or teleop).
   *
   * Warning: This is not an official time (so it cannot be used to dispute ref
   * calls or guarantee that a function will trigger before the match ends).
   *
   * The Practice Match function of the DS approximates the behavior seen on the
   * field.
   *
   * @return Time remaining in current match period (auto or teleop)
   */
  static double GetMatchTime();

  // The time, in seconds, at which the 32-bit FPGA timestamp rolls over to 0
  static const double kRolloverTime;

 private:
  double m_startTime = 0.0;
  double m_accumulatedTime = 0.0;
  bool m_running = false;
  mutable wpi::mutex m_mutex;
};

}  // namespace frc
