/**
 * @file CRC.c
 * @brief Implementation of CRC-6 (X^6 + X + 1) checksum calculations for MT6701 sensor.
 * @author Saulius
 * @date 2024-12-04
 */

#include "Settings.h"

/**
 * @brief CRC-6 (X^6 + X + 1) lookup table for fast checksum calculation.
 */
const uint8_t crc6_table[64] = {
    0x00, 0x03, 0x06, 0x05, 0x0C, 0x0F, 0x0A, 0x09,
    0x18, 0x1B, 0x1E, 0x1D, 0x14, 0x17, 0x12, 0x11,
    0x30, 0x33, 0x36, 0x35, 0x3C, 0x3F, 0x3A, 0x39,
    0x28, 0x2B, 0x2E, 0x2D, 0x24, 0x27, 0x22, 0x21,
    0x23, 0x20, 0x25, 0x26, 0x2F, 0x2C, 0x29, 0x2A,
    0x3B, 0x38, 0x3D, 0x3E, 0x37, 0x34, 0x31, 0x32,
    0x13, 0x10, 0x15, 0x16, 0x1F, 0x1C, 0x19, 0x1A,
    0x0B, 0x08, 0x0D, 0x0E, 0x07, 0x04, 0x01, 0x02,
};

/**
 * @brief Calculates the CRC-6 checksum using a lookup table.
 * 
 * @param data 18-bit data word (excluding CRC) from the MT6701 sensor.
 * @return Computed 6-bit CRC value.
 */
uint8_t crc6_mt6701_lookup(uint32_t data) {
    return crc6_table[crc6_table[crc6_table[0 ^ ((data >> 12) & 0x3F)] ^ ((data >> 6) & 0x3F)] ^ (data & 0x3F)];
}

/**
 * @brief Verifies the CRC-6 checksum of the MT6701 sensor data.
 *
 * This function extracts the received CRC from the 24-bit data, removes it from the data,
 * and calculates the expected CRC to verify correctness.
 *
 * @param[in,out] data Pointer to the 24-bit data word with CRC.
 *                     The function modifies this value by removing the last 6 bits (CRC).
 * @return 0 if the CRC is correct, 1 otherwise.
 */
uint8_t MT6701CRC(uint32_t *data) {
    uint8_t received_crc = *data & 0x3F; ///< Extract the CRC (last 6 bits)
    *data = *data >> 6; ///< Remove CRC, leaving only the actual data
    uint8_t calculated_crc = crc6_mt6701_lookup(*data); ///< Compute expected CRC
    return received_crc == calculated_crc ? 0 : 1; ///< Return 0 if CRC matches, otherwise 1
}