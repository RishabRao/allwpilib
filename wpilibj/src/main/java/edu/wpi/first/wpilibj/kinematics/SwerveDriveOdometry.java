/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.kinematics;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Twist2d;

/**
 * Class for swerve drive odometry. Odometry allows you to track the robot's
 * position on the field over a course of a match using readings from your
 * swerve drive encoders and swerve azimuth encoders.
 *
 * <p>Teams can use odometry during the autonomous period for complex tasks like
 * path following. Furthermore, odometry can be used for latency compensation
 * when using computer-vision systems.
 */
public class SwerveDriveOdometry {
  private final SwerveDriveKinematics m_kinematics;
  private Pose2d m_poseMeters;
  private double m_prevTimeSeconds = -1;

  private Rotation2d m_previousAngle;

  /**
   * Constructs a SwerveDriveOdometry object.
   *
   * @param kinematics  The swerve drive kinematics for your drivetrain.
   * @param initialPose The starting position of the robot on the field.
   */
  public SwerveDriveOdometry(SwerveDriveKinematics kinematics, Pose2d initialPose) {
    m_kinematics = kinematics;
    m_poseMeters = initialPose;
    m_previousAngle = initialPose.getRotation();
  }

  /**
   * Constructs a SwerveDriveOdometry object with the default pose at the origin.
   *
   * @param kinematics The swerve drive kinematics for your drivetrain.
   */
  public SwerveDriveOdometry(SwerveDriveKinematics kinematics) {
    this(kinematics, new Pose2d());
  }

  /**
   * Resets the robot's position on the field.
   *
   * @param pose The position on the field that your robot is at.
   */
  public void resetPosition(Pose2d pose) {
    m_poseMeters = pose;
    m_previousAngle = pose.getRotation();
  }

  /**
   * Returns the position of the robot on the field.
   * @return The pose of the robot (x and y are in meters).
   */
  public Pose2d getPoseMeters() {
    return m_poseMeters;
  }

  /**
   * Updates the robot's position on the field using forward kinematics and
   * integration of the pose over time. This method takes in the current time as
   * a parameter to calculate period (difference between two timestamps). The
   * period is used to calculate the change in distance from a velocity. This
   * also takes in an angle parameter which is used instead of the
   * angular rate that is calculated from forward kinematics.
   *
   * @param currentTimeSeconds The current time in seconds.
   * @param angle              The angle of the robot.
   * @param moduleStates       The current state of all swerve modules. Please provide
   *                           the states in the same order in which you instantiated your
   *                           SwerveDriveKinematics.
   * @return The new pose of the robot.
   */
  public Pose2d updateWithTime(double currentTimeSeconds, Rotation2d angle,
                               SwerveModuleState... moduleStates) {
    double period = m_prevTimeSeconds >= 0 ? currentTimeSeconds - m_prevTimeSeconds : 0.0;
    m_prevTimeSeconds = currentTimeSeconds;

    var chassisState = m_kinematics.toChassisSpeeds(moduleStates);
    var newPose = m_poseMeters.exp(
        new Twist2d(chassisState.vxMetersPerSecond * period,
            chassisState.vyMetersPerSecond * period,
            angle.minus(m_previousAngle).getRadians()));

    m_previousAngle = angle;
    m_poseMeters = new Pose2d(newPose.getTranslation(), angle);

    return m_poseMeters;
  }

  /**
   * Updates the robot's position on the field using forward kinematics and
   * integration of the pose over time. This method automatically calculates the
   * current time to calculate period (difference between two timestamps). The
   * period is used to calculate the change in distance from a velocity. This
   * also takes in an angle parameter which is used instead of the angular
   * rate that is calculated from forward kinematics.
   *
   * @param angle        The angle of the robot.
   * @param moduleStates The current state of all swerve modules. Please provide
   *                     the states in the same order in which you instantiated your
   *                     SwerveDriveKinematics.
   * @return The new pose of the robot.
   */
  public Pose2d update(Rotation2d angle, SwerveModuleState... moduleStates) {
    return updateWithTime(System.currentTimeMillis() / 1000.0, angle, moduleStates);
  }
}
