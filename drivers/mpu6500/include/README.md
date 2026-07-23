# drivers/mpu6500/include

Interface pública do driver `MPU6500` (`mpu6500.h`). Para o princípio físico do sensor, registradores e fluxo de inicialização/aquisição, ver [`docs/sensors/MPU6500`](../../../docs/sensors/MPU6500); este README documenta apenas a API C++ exposta por este driver.

## Injeção de dependência do barramento I2C

O driver não acessa nenhum periférico STM32 diretamente — ele recebe, no construtor, ponteiros de função para ler/escrever um registrador via I2C:

```cpp
typedef void (*I2C_write_register)(uint8_t addr, uint8_t reg, uint8_t data);
typedef uint8_t (*I2C_read_register)(uint8_t addr, uint8_t reg);
```

Isso mantém o driver independente da implementação de I2C usada (bare-metal, HAL, outra placa etc.) — basta passar funções com essa assinatura. No firmware atual, essas funções são `i2c_write_register`/`i2c_read_register` de [`firmware/lib/i2c`](../../../firmware/lib/i2c).

## `MPU6500`

```cpp
MPU6500(uint8_t addr, I2C_write_register write_func, I2C_read_register read_func);
```

Constrói o driver para um dispositivo no endereço I2C `addr` (`0x68` com `AD0=0`, `0x69` com `AD0=1`), usando `write_func`/`read_func` para toda comunicação com o sensor.

### `bool init()`

Verifica o registrador `WHOAMI` (deve ser `0x70`), tira o sensor do modo sleep, configura full-scale range (acelerômetro e giroscópio) e a frequência de corte do filtro passa-baixa. Retorna `false` se o `WHOAMI` não bater — sinal de endereço errado, fiação incorreta ou sensor não alimentado.

### `AccelData read_accel()`

```cpp
struct AccelData { int16_t x, y, z; };
```

Lê os 6 bytes de aceleração (registradores `0x3B`–`0x40`) e retorna os valores **brutos** de cada eixo, já combinados em `int16_t` (byte alto + byte baixo). Não há conversão para unidade física (g) — para isso, dividir pela sensibilidade correspondente ao full-scale range configurado (ver tabela em `docs/sensors/MPU6500`).

### `GyroData read_gyro()`

```cpp
struct GyroData { int16_t x, y, z; };
```

Mesma lógica de `read_accel()`, para os registradores do giroscópio (`0x43`–`0x48`). Valores brutos em LSB, conversão para graus/s fica a cargo de quem consome o driver.

## Uso típico

```cpp
#include "mpu6500.h"
#include "i2c.h"

MPU6500 mpu(0x68, i2c_write_register, i2c_read_register);

if (!mpu.init()) {
    // sensor não respondeu como esperado
}

MPU6500::AccelData accel = mpu.read_accel();
MPU6500::GyroData gyro = mpu.read_gyro();
```

## Limitações atuais

- Full-scale range e frequência do filtro passa-baixa são fixos em `init()` (não configuráveis por parâmetro).
- Sem leitura de temperatura, apesar do registrador estar disponível no sensor.
- Sem tratamento de erro de barramento (timeout) — os ponteiros de função de I2C injetados são bloqueantes por design (ver `firmware/lib/i2c`).
