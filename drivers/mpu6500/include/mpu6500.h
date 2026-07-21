#pragma once

#include <stdint.h>

typedef void (*I2C_write_register)(uint8_t addr, uint8_t reg, uint8_t data);
typedef uint8_t (*I2C_read_register)(uint8_t addr, uint8_t reg);

class MPU6500 {

    private:
    uint8_t _addr;
    I2C_write_register _write_reg;
    I2C_read_register _read_reg;

    public:
    MPU6500(uint8_t addr, I2C_write_register write_func, I2C_read_register read_func);

    struct AccelData {
        int16_t x, y, z;
    };

    bool init();
    AccelData read_accel();
};