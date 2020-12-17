#pragma once
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A 3D vector class used by the BerryIMU. 
 * 
 * Literally just three floats pasted together and given individual names.
 * 
 */
typedef struct berryIMU_vec3f{
    float x, y, z;
}berryIMU_vec3f_t;

/**
 * @brief An acceleration, a velocity, value, and timestamp in one package.
 * 
 * All of type berryIMU_vec3f_t (except for time which is the standard time_t)
 * 
 */
typedef struct berryIMU_measurement{
    berryIMU_vec3f_t value;
    berryIMU_vec3f_t velocity;
    berryIMU_vec3f_t acceleration;
    time_t timestamp;
}berryIMU_measurement_t;

/**
 * @brief Description of the lateral physical movement of the IMU.
 * 
 * Units are in:
 *   m     for the value
 *   m/s   for the velocity
 *   m/s^2 for the acceleration
 */
extern berryIMU_measurement_t berryIMU_current_position;

/**
 * @brief Description of the lateral physical movement of the IMU.
 * 
 * Units are in:
 *   rad     for the value
 *   rad/s   for the velocity
 *   rad/s^2 for the acceleration
 */
extern berryIMU_measurement_t berryIMU_current_rotation;

/**
 * @brief A vector pointing to the north pole (I think?)
 * 
 */
extern berryIMU_vec3f_t berryIMU_current_magnet_direction;

/**
 * @brief Initializes the IMU
 * 
 * @return int an error code. 0 means the process was successful.
 */
int berryIMU_IMU_init();

/**
 * @brief Updates berryIMU_current_position, berryIMU_current_position, and 
 * berryIMU_current_magnet_direction using calibrated readings from the physical IMU
 * 
 * @return int an error code. 0 means the process was successful.
 */
int berryIMU_IMU_update(){
    berryIMU_IMU_update_position();
    berryIMU_IMU_update_rotation();
    berryIMU_IMU_update_magnet();
}

/**
 * @brief Updates berryIMU_current_position using calibrated readings from 
 * the physical IMU
 * 
 * Units are in:
 *   m     for the value
 *   m/s   for the velocity
 *   m/s^2 for the acceleration
 * 
 * @return int an error code. 0 means the process was successful.
 */
int berryIMU_IMU_update_position();

/**
 * @brief Updates berryIMU_current_rotation using calibrated readings from 
 * the physical IMU
 * 
 * Units are in:
 *   rad     for the value
 *   rad/s   for the velocity
 *   rad/s^2 for the acceleration
 * 
 * @return int an error code. 0 means the process was successful.
 */
int berryIMU_IMU_update_rotation();

/**
 * @brief Updates berryIMU_current_magnet_direction using calibrated readings from 
 * the physical IMU
 * 
 * @return int an error code. 0 means the process was successful.
 */
int berryIMU_IMU_update_magnet();

/**
 * @brief Calibrates the IMU for more accurate readings.
 * 
 * This function assumes that the IMU is on a flat surface and is not moving.
 * 
 * @return int an error code. 0 means the process was successful.
 */
int berryIMU_IMU_calibrate(unsigned int duration);

/**
 * @brief De-initializes the IMU
 * 
 * @return int an error code. 0 means the process was successful.
 */
int berryIMU_IMU_destroy();


#ifdef __cplusplus
}
#endif