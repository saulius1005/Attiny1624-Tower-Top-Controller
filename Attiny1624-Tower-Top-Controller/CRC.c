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

/**
 * @brief Precomputed lookup table for CRC-8 calculation using the CDMA2000 polynomial.
 * 
 * This table contains CRC-8 values for all possible byte values (0x00 to 0xFF), 
 * computed using the CDMA2000 CRC polynomial. It is used for fast CRC calculation 
 * by allowing quick lookup for each byte in the input data.
 * 
 * @note The table is critical for efficient CRC-8 calculation and must remain constant throughout the application.
 */
const uint8_t crc8_table[256] = {
    0x00, 0x9B, 0xAD, 0x36, 0xC1, 0x5A, 0x6C, 0xF7, 0x19, 0x82, 0xB4, 0x2F, 0xD8, 0x43, 0x75, 0xEE,
    0x32, 0xA9, 0x9F, 0x04, 0xF3, 0x68, 0x5E, 0xC5, 0x2B, 0xB0, 0x86, 0x1D, 0xEA, 0x71, 0x47, 0xDC,
    0x64, 0xFF, 0xC9, 0x52, 0xA5, 0x3E, 0x08, 0x93, 0x7D, 0xE6, 0xD0, 0x4B, 0xBC, 0x27, 0x11, 0x8A,
    0x56, 0xCD, 0xFB, 0x60, 0x97, 0x0C, 0x3A, 0xA1, 0x4F, 0xD4, 0xE2, 0x79, 0x8E, 0x15, 0x23, 0xB8,
    0xC8, 0x53, 0x65, 0xFE, 0x09, 0x92, 0xA4, 0x3F, 0xD1, 0x4A, 0x7C, 0xE7, 0x10, 0x8B, 0xBD, 0x26,
    0xFA, 0x61, 0x57, 0xCC, 0x3B, 0xA0, 0x96, 0x0D, 0xE3, 0x78, 0x4E, 0xD5, 0x22, 0xB9, 0x8F, 0x14,
    0xAC, 0x37, 0x01, 0x9A, 0x6D, 0xF6, 0xC0, 0x5B, 0xB5, 0x2E, 0x18, 0x83, 0x74, 0xEF, 0xD9, 0x42,
    0x9E, 0x05, 0x33, 0xA8, 0x5F, 0xC4, 0xF2, 0x69, 0x87, 0x1C, 0x2A, 0xB1, 0x46, 0xDD, 0xEB, 0x70,
    0x0B, 0x90, 0xA6, 0x3D, 0xCA, 0x51, 0x67, 0xFC, 0x12, 0x89, 0xBF, 0x24, 0xD3, 0x48, 0x7E, 0xE5,
    0x39, 0xA2, 0x94, 0x0F, 0xF8, 0x63, 0x55, 0xCE, 0x20, 0xBB, 0x8D, 0x16, 0xE1, 0x7A, 0x4C, 0xD7,
    0x6F, 0xF4, 0xC2, 0x59, 0xAE, 0x35, 0x03, 0x98, 0x76, 0xED, 0xDB, 0x40, 0xB7, 0x2C, 0x1A, 0x81,
    0x5D, 0xC6, 0xF0, 0x6B, 0x9C, 0x07, 0x31, 0xAA, 0x44, 0xDF, 0xE9, 0x72, 0x85, 0x1E, 0x28, 0xB3,
    0xC3, 0x58, 0x6E, 0xF5, 0x02, 0x99, 0xAF, 0x34, 0xDA, 0x41, 0x77, 0xEC, 0x1B, 0x80, 0xB6, 0x2D,
    0xF1, 0x6A, 0x5C, 0xC7, 0x30, 0xAB, 0x9D, 0x06, 0xE8, 0x73, 0x45, 0xDE, 0x29, 0xB2, 0x84, 0x1F,
    0xA7, 0x3C, 0x0A, 0x91, 0x66, 0xFD, 0xCB, 0x50, 0xBE, 0x25, 0x13, 0x88, 0x7F, 0xE4, 0xD2, 0x49,
    0x95, 0x0E, 0x38, 0xA3, 0x54, 0xCF, 0xF9, 0x62, 0x8C, 0x17, 0x21, 0xBA, 0x4D, 0xD6, 0xE0, 0x7B
};

/**
 * @brief CRC-8 calculation for CDMA2000 using a precomputed lookup table.
 * 
 * This function calculates the CRC-8 checksum for the input data using the CDMA2000
 * CRC polynomial. It uses a lookup table to quickly compute the CRC for each byte
 * of the input data. The result is returned as a single byte (CRC value).
 * 
 * @param data The input data to calculate the CRC for.
 * @return The CRC-8 checksum value for the data.
 */
uint8_t crc8_cdma2000(uint64_t data) {
    uint8_t crc = 0xFF;    ///< Initial CRC value.
    size_t length = 0;     ///< Length of the data in bytes.
    uint64_t temp = data;

    // Calculate the number of bytes in the data.
    while (temp) {
        length++;
        temp >>= 8;
    }

    // Process each byte in the data.
    while (length--) {
        crc = crc8_table[crc ^ ((data >> (length * 8)) & 0xFF)];
    }
    return crc;  ///< Return the calculated CRC.
}
