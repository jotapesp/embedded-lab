# embedded-lab

Plataforma pessoal de pesquisa em sistemas embarcados e aquisição de sinais, construída em torno de um STM32 e drivers de sensores modulares. O objetivo não é um projeto isolado, mas uma infraestrutura reutilizável — firmware, drivers e pipeline de dados — que sirva de base para iniciação científica, TCC e projetos futuros: qualquer novo sensor deve poder ser conectado à plataforma com o mínimo de esforço, reaproveitando toda a aquisição, processamento e ML já existentes.

## Hardware

- **MCU:** STM32F103C8 ("Bluepill"), acesso direto a registrador via CMSIS (sem HAL/framework de abstração)
- **Toolchain:** [PlatformIO](https://platformio.org/) (`env:bluepill_f103c6`)
- **Comunicação com o host:** USB Serial (STM32 → Python), protocolo ainda a definir

## Estrutura

```
embedded-lab/
├── firmware/       # Aplicação principal STM32 (C++) — main loop, comunicação serial
│   ├── src/        # main.cpp
│   └── lib/        # Bibliotecas internas (GPIO, I2C, UART, delay) — ver firmware/lib/README
├── drivers/        # Drivers por sensor (C++) — um módulo por sensor, independentes e reutilizáveis
├── python/         # Biblioteca Python — leitor serial, parser, processamento de sinais, utilitários
├── notebooks/      # Jupyter — exploração interativa, visualização, ML (importa de python/)
├── datasets/       # Dados organizados: raw/, processed/, features/, models/
├── experiments/    # Experimentos documentados (descrição, sensores, frequência, condições)
└── docs/           # Documentação técnica: datasheets e READMEs por sensor, referências STM32
```

`python/`, `notebooks/`, `datasets/` e `experiments/` ainda estão vazios — o trabalho até aqui foi concentrado no firmware e nos drivers.

## Sensores

Estudados em ordem, um de cada vez, para consolidar o fluxo completo (driver → aquisição → pipeline) antes de passar ao próximo.

| Sensor  | Interface | Grandeza                      | Status     |
|---------|-----------|--------------------------------|------------|
| MPU6500 | I²C       | Aceleração, velocidade angular | Estudando  |
| BME280  | I²C/SPI   | Temperatura, umidade, pressão  | Aguardando |
| HX711   | GPIO      | Massa (célula de carga)        | Aguardando |
| INMP441 | I²S + DMA | Áudio                          | Aguardando |
| BME680  | I²C/SPI   | Temp., umidade, pressão, VOC   | Aguardando |

Documentação de cada sensor (princípio físico, registradores, fluxo de inicialização/aquisição) fica em [`docs/sensors/`](docs/sensors). Documentação da API de cada driver fica junto ao próprio driver, em `drivers/<sensor>/include/`.

## Stack

| Camada              | Tecnologia                          |
|---------------------|--------------------------------------|
| Firmware / Drivers   | C++, acesso direto a registrador (CMSIS) |
| Pipeline de dados    | Python (pyserial, pandas, numpy, scipy) |
| Análise / ML         | Jupyter Notebook, scikit-learn       |

## Build e upload do firmware

Com o [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/index.html) instalado:

```bash
cd firmware
pio run              # compila
pio run -t upload    # compila e grava na placa
pio device monitor    # abre o monitor serial
```

`drivers/` é incluído no build via `lib_extra_dirs` em `firmware/platformio.ini`, então qualquer driver novo em `drivers/<sensor>/` fica disponível automaticamente para `#include` em `firmware/src/`.

## Convenções

- Drivers são independentes entre si — não há dependência de um driver de sensor sobre outro.
- Dados brutos nunca são sobrescritos — sempre preservados em `datasets/raw/`.
- Cada experimento é documentado com descrição, sensores usados, frequência de aquisição e condições.

## Abordagem pedagógica

Este projeto é usado para aprendizado ativo: a implementação (firmware, drivers, pipeline) é escrita pelo autor, não gerada — assistência de IA neste repositório segue um modo socrático (dicas e direcionamento a datasheets/reference manuals, não código pronto). Detalhes em [`CLAUDE.md`](CLAUDE.md).

## Licença

[MIT](LICENSE)
