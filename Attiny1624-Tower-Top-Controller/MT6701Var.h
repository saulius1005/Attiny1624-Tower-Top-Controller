/**
 * @file MT6701Var.h
 * @brief Header file for defining the MT6701 sensor global variable.
 * @author Saulius
 * @date 2025-02-28
 */

#ifndef MT6701VAR_H_
#define MT6701VAR_H_

/**
 * @brief Global instance of AngleSensorStatus to store MT6701 sensor data.
 *
 * - MagneticFieldStatus:
 *   - 0: Normal
 *   - 1: Magnetic field too strong
 *   - 2: Magnetic field too weak
 *
 * - PushButtonStatus:
 *   - 0: Normal
 *   - 1: Push button detected
 *
 * - TrackStatus:
 *   - 0: Normal
 *   - 1: Loss of track
 *
 * - CRCError:
 *   - 0: Normal
 *   - 1: CRC error detected
 */
AngleSensorStatus MT6701ELEVATION = {
    .Angle = 0,
    .MagneticFieldStatus = 0,
    .PushButtonStatus = 0,
    .TrackStatus = 0,
    .CRCError = 0
};

AngleSensorStatus MT6701AZIMUTH = {
	.Angle = 0,
	.MagneticFieldStatus = 0,
	.PushButtonStatus = 0,
	.TrackStatus = 0,
	.CRCError = 0
};

#endif /* MT6701VAR_H_ */
