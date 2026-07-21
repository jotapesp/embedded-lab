# MPU6500

## Princípio físico
### **Acelerômetro**
- Massas de prova em cada eixo se deslocam de acordo com o valor da força causada pela aceleração naquele eixo. A massa de prova funciona como a segunda placa "compartilhada" entre os 2 capacitores de cada eixo, e quando ela se desloca se aproximando ou afastando de uma das placas, gera variação nas capacitâncias dos capacitores, o que causa variação de tensão, e, consequentemente, gera um sinal digital que é armazenado nos registradores;

### **Giroscópio**
- Massas vibrando em frequências conhecidas em cada eixo são utilizadas para detectar a componente da velocidade angular para cada eixo através do efeito Coriolis. Quando tem-se uma massa vibrando no eixo X, por exemplo, e rotaciona-se o sistema em torno do eixo Z, gera-se uma força (Coriolis) perpendicular à  vibração (nesse caso, no eixo Y), uma vez que a massa tende a manter-se vibrando no eixo X. Essa força é  proporcional à componente da velocidade angular naquele eixo.

## Grandeza medida

Giroscópio - velocidade angular (graus/s)
Acelerômetro - aceleração linear (m/s^2)

## Interface

- SPI ou I2C;

### I2C (barramento, serial)

| Parâmetro | Valor |
|---|---|
| Endereço (AD0=0) | 0x68 |
| Endereço (AD0=1) | 0x69 |
| Frequência de comunicação (f SCL) | 100kHz |
| Frequência de comunicação (f SCL), máxima | 400kHz |

## Frequência máxima

### Frequência de operação I2C

| Modo | Frequência |
|---|---|
| Padrão | 100kHz |
| Fast-mode | 400kHz |

## Alimentação

| Pino | Faixa |
|---|---|
| VDD | 1.8 - 3.3 V +-5% |
| VDDIO | 1.8 - 3.3 V +-5% |

## Resolução

- LSB: unidade do valor do registrador

### Giroscópio

| Full-scale | Resolução |
|---|---|
| +- 250 graus/s | 0.0076 (graus/s)/LSB |
| +- 500 graus/s | 0.015 (graus/s)/LSB |
| +- 1000 graus/s | 0.031 (graus/s)/LSB |
| +- 2000 graus/s | 0.061 (graus/s)/LSB |

### Acelerômetro

| Full-scale | Resolução |
|---|---|
| +- 2g | 0.000061 (g/LSB) |
| +- 4g | 0.000122 (g/LSB) |
| +- 8g | 0.000244 (g/LSB) |
| +- 16g | 0.000488 (g/LSB) |

## Sensibilidade

- É 1/resolução.
- Unidade real = registrador / sensibilidade.

### Giroscópio

| Full-scale | Sensibilidade |
|---|---|
| +- 250 graus/s | 131 LSB/(graus/s) |
| +- 500 graus/s | 65.5 LSB/(graus/s) |
| +- 1000 graus/s | 32.8 LSB/(graus/s) |
| +- 2000 graus/s | 16.4 LSB/(graus/s) |

### Acelerômetro

| Full-scale | Sensibilidade |
|---|---|
| +- 2g | 16384 (LSB/g) |
| +- 4g | 8192 (LSB/g) |
| +- 8g | 4096 (LSB/g) |
| +- 16g | 2048 (LSB/g) |

## Precisão

### Giroscópio
| Parâmetro | Valor |
|---|---|
| Offset (ZRO) | +-5 (graus/s) |
| RMS Noise | 0.1 (graus/s-RMS) |
| Noise Density | 0.01 (graus/s/sqrt(Hz)) |

### Acelerômetro
| Parâmetro | Valor |
|---|---|
| Offset | +- 60 (m g) |
| Noise Density | 300 u g/sqrt(Hz) |

## Registradores importantes

| Registrador | Descrição |
|---|---|
| 107 (0x6B) | PWR_MGMT_1 (acordar o sensor) |
| 26 (0x1A) | configuração do filtro passa-baixa (frequencia de corte configurável entre 5-260Hz) |
| 27/28 (0x1B/0x1C) | configuração do giroscópio/acelerômetro (full-scale range) |
| 59-64 (0x3B-0x40)| dados do acelerômetro |
| 65-66 | temperatura |
| 67-72 (0x43-0x48) | dados do giroscópio |
| 106 | User Control |

## Fluxo de inicialização

0. Endereço i2c (no MPU-6500 0x68 ou 0x69 dependendo do valor no pino AD0)
1. Identificar o chip: ler WHOAMI = 0x70 (default)
2. Tirar do modo dormir: Limpar o bit SLEEP (6) do PWR_MGMT_1 (0x6B)
3. Configurar a full scale range do giroscópio e acelerômetro (0x1B e 0x1C)
4. Configurar filtro passa-baixa (frequência de corte entre 5 (6 (110) - máximo) e 250 Hz (0 (000) - desativado)): Reg 0x1A configurar os bits DLPF-CFG [2:0] 
5. Ler dados

## Fluxo de aquisição

1. Requisitar leitura do registrador 59 (0x3B)
2. Ler 6 bytes consecutivos (acelerômetro X, Y, Z)
3. Requisitar leitura a partir do registrador 67
4. Ler 6 bytes consecutivos (giroscópio X, Y, Z)
5. Combinar bytes alto e baixo de cada eixo
6. Dividir pela sensibilidade → valor em unidade física

- OBS: Cada registrador armazena 8 bits, a palavra do sensor é 16 bits (2 bytes), então cada dado/informação sobre um eixo ocupa 2 bytes para cada sensor (acelerômetro e giroscópio). O i2c transfere 8bits (1 byte) por vez.

### Fluxo I2C:

1. Envia endereço do dispositivo (Ox68) + bit escrita
2. Envia endereço do registrador
3. Reinicia
4. Envia endereço do dispositivo (Ox68) + bit leitura
5. MPU responde

## Cuidados

- Necessita resistores pull-up: 4k ou 10k Ohms ligados ao VCC;
- Frequência de corte do filtro deve ser de acordo com o que se deseja medir;

## Possíveis aplicações

## Ideias de experimentos

## Ideias de pesquisa

## Datasheet

[MPU6500 Product Specification](./datasheet2.pdf)
[Register Map](./register_map.pdf)

## Artigos interessantes