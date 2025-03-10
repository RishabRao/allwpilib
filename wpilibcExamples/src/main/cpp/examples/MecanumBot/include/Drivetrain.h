/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/AnalogGyro.h>
#include <frc/Encoder.h>
#include <frc/Spark.h>
#include <frc/controller/PIDController.h>
#include <frc/geometry/Translation2d.h>
#include <frc/kinematics/MecanumDriveKinematics.h>
#include <frc/kinematics/MecanumDriveOdometry.h>
#include <frc/kinematics/MecanumDriveWheelSpeeds.h>
#include <wpi/math>

/**
 * Represents a mecanum drive style drivetrain.
 */
class Drivetrain {
 public:
  Drivetrain() { m_gyro.Reset(); }

  /**
   * Get the robot angle as a Rotation2d
   */
  frc::Rotation2d GetAngle() const {
    // Negating the angle because WPILib Gyros are CW positive.
    return frc::Rotation2d(units::degree_t(-m_gyro.GetAngle()));
  }

  frc::MecanumDriveWheelSpeeds GetCurrentState() const;
  void SetSpeeds(const frc::MecanumDriveWheelSpeeds& wheelSpeeds);
  void Drive(units::meters_per_second_t xSpeed,
             units::meters_per_second_t ySpeed, units::radians_per_second_t rot,
             bool fieldRelative);
  void UpdateOdometry();

  static constexpr units::meters_per_second_t kMaxSpeed =
      3.0_mps;  // 3 meters per second
  static constexpr units::radians_per_second_t kMaxAngularSpeed{
      wpi::math::pi};  // 1/2 rotation per second

 private:
  frc::Spark m_frontLeftMotor{1};
  frc::Spark m_frontRightMotor{2};
  frc::Spark m_backLeftMotor{3};
  frc::Spark m_backRightMotor{4};

  frc::Encoder m_frontLeftEncoder{0, 1};
  frc::Encoder m_frontRightEncoder{0, 1};
  frc::Encoder m_backLeftEncoder{0, 1};
  frc::Encoder m_backRightEncoder{0, 1};

  frc::Translation2d m_frontLeftLocation{0.381_m, 0.381_m};
  frc::Translation2d m_frontRightLocation{0.381_m, -0.381_m};
  frc::Translation2d m_backLeftLocation{-0.381_m, 0.381_m};
  frc::Translation2d m_backRightLocation{-0.381_m, -0.381_m};

  frc2::PIDController m_frontLeftPIDController{1.0, 0.0, 0.0};
  frc2::PIDController m_frontRightPIDController{1.0, 0.0, 0.0};
  frc2::PIDController m_backLeftPIDController{1.0, 0.0, 0.0};
  frc2::PIDController m_backRightPIDController{1.0, 0.0, 0.0};

  frc::AnalogGyro m_gyro{0};

  frc::MecanumDriveKinematics m_kinematics{
      m_frontLeftLocation, m_frontRightLocation, m_backLeftLocation,
      m_backRightLocation};

  frc::MecanumDriveOdometry m_odometry{m_kinematics};
};
