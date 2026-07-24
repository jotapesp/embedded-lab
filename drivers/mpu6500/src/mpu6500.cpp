#include "mpu6500.h"

MPU6500::MPU6500(uint8_t addr, I2C_write_register write_func, I2C_read_register read_func){
    _addr = addr;
    _write_reg = write_func;
    _read_reg = read_func;
}

bool MPU6500::init(){
    uint8_t val, curr_val, write_data;
    // checar se o WHOAMI (0x75) contém o valor default (0x70)
    val = _read_reg(_addr, 0x75);
    _last_read_val = val;
    if (val != 0x70){
        return false;
    }
    // limpar bit de sleep (bit 6) no reg PWR_MGMT_1 (0x6B)
    curr_val = _read_reg(_addr, 0x6B);
    write_data = curr_val & ~(1<<6);
    _write_reg(_addr, 0x6B, write_data);

    // configurar full scale range
    curr_val = _read_reg(_addr, 0x1B);
    write_data = curr_val & ~(3<<3); // seta bits [4:3] para [00] - full scale 250d/s e maior sensibilidade
    _write_reg(_addr, 0x1B, write_data);
    curr_val = _read_reg(_addr, 0x1C);
    write_data = curr_val & ~(3<<3); // seta bits [4:3] para [00] - full scale 2g e maior sensibilidade
    _write_reg(_addr, 0x1C, write_data);

    // configurar freq filtro passa-baixa
    curr_val = _read_reg(_addr, 0x1A);
    write_data = curr_val & ~(7); // limpa os 3 LSB e também
                                // seta os 3 LSB (DLPF-CFG) para 000 (freq corte 250 Hz)
    _write_reg(_addr, 0x1A, write_data);

    return true;
}

MPU6500::AccelData MPU6500::read_accel(){

    AccelData read_data;
    read_data.x = _read_reg(_addr, 0x3B) << 8;
    read_data.x |= _read_reg(_addr, 0x3B + 1);
    read_data.y = _read_reg(_addr, 0x3B + 2) << 8;
    read_data.y |= _read_reg(_addr, 0x3B + 3);
    read_data.z = _read_reg(_addr, 0x3B + 4) << 8;
    read_data.z |= _read_reg(_addr, 0x3B + 5);

    return read_data;
}

MPU6500::GyroData MPU6500::read_gyro(){

    GyroData read_data;
    read_data.x = _read_reg(_addr, 0x43) << 8;
    read_data.x |= _read_reg(_addr, 0x43 + 1);
    read_data.y = _read_reg(_addr, 0x43 + 2) << 8;
    read_data.y |= _read_reg(_addr, 0x43 + 3);
    read_data.z = _read_reg(_addr, 0x43 + 4) << 8;
    read_data.z |= _read_reg(_addr, 0x43 + 5);

    return read_data;
}