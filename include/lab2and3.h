/** @brief Graveyard for old Lab 2 and 3 functions
 *
 * @file lab2and3.h
 *
 *
 * @author cpbove@wpi.edu
 * @date 3-Mar-2016
 * @version 1.0
 */

#ifndef INCLUDE_LAB2AND3_H_
#define INCLUDE_LAB2AND3_H_

/**
 * @brief changes desired joint angle of arm based on button presses for testing
 * @note This does not run the PID control, just sets setpoint
 */
void controlArmWithButtons();
/**
 * @brief changes desired joint angles of arm based on button presses
 * @note This does not run the PID control, just sets setpoint
 */
void controlPositionWithButtons();
/**
 * @brief Lab3 - control motor drive voltages with buttons
 * @details Sets first link to 0, -3, +3, or plus 6 Volts
 * button 4 sets voltage to 0
 * button 5 sets voltage to -3
 * button 6 sets voltage to +3
 * button 7 sets voltage to +6
 */
void controlJoint1WithButtons();
/**
 * @brief state machine for drawing triangles.
 * @details button 4 draws a prescripted triangle
 * button 5 just moves to a pose
 * button 6 records a motion
 * button 7 plays back the motion
 * @note This does not run the PID control, just sets setpoint
 */
void drawTriangleWithButtons();
/**
 * @brief prints the header for streaming joint angles
 */
void printHeaderForJointAngle();
/**
 * @brief prints the header for the main logging of step responses
 */
void printHeaderForLogging();
/**
 * @brief streams joint angles in degrees and (x,y) tip positions in mm
 */
void printJointAnglesAndPos();
/**
 * @brief prints adcval,pot mV, and joint angle of the passed joint number
 * @param  joint 1 or 2 of the joint to print data for
 *
 */
void printJointAngle(int joint);
/**
 * @brief prints time, setpoint, jointAngle, PID output, and motor current
 * @param setPoint the current command to the controller
 *
 */
void printLogLineJoint2(int setPoint);
/**
 * @brief converts a voltage value to a DAC value
 * @param  volts to convert to DAC -7.2 to 7.2 volts
 *
 * @return DAC value 0-4095
 */
int convertVoltsToDACVal(float volts);
/**
 * @brief runs getAccel and converts to G's
 * @param axis 0-2 for x,y,z axis to get g's on
 *
 * @return -3.0 to 3.0 g's of force
 */
float getGs(int axis);


#endif /* INCLUDE_LAB2AND3_H_ */
