/**
 * @file MT6701.h
 * @brief Header file for MT6701 sensor data structure.
 * @author Saulius
 * @date 2025-02-28
 */

#ifndef MT6701_H_
#define MT6701_H_

/**
 * @struct AngleSensorStatus
 * @brief Structure to store MT6701 sensor data.
 *
 * This structure holds the sensor's angle measurement, magnetic field status,
 * push button status, track status, and CRC error flag.
 */
typedef struct {
    double Angle;                 ///< Measured angle in degrees
    uint8_t MagneticFieldStatus;  ///< Magnetic field status (validity flag)
    uint8_t PushButtonStatus;     ///< Push button status (if applicable)
    uint8_t TrackStatus;          ///< Tracking status
    uint8_t CRCError;             ///< CRC error flag (0 = valid, 1 = error detected)
} AngleSensorStatus;

/**
 * @brief Global variable to store the latest MT6701 sensor data.
 */
extern AngleSensorStatus MT6701;

#endif /* MT6701_H_ */