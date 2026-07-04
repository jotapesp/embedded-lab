# CLAUDE.md — embedded-lab

## Sobre o projeto

Plataforma pessoal de pesquisa em sistemas embarcados e aquisição de sinais. O objetivo é construir uma infraestrutura reutilizável que sirva de base para iniciação científica, TCC e projetos futuros — não um projeto isolado.

A filosofia é: qualquer novo sensor pode ser conectado à plataforma com o mínimo de esforço, reutilizando toda a pipeline de aquisição, processamento e ML já existente.

## Estrutura

```
embedded-lab/
├── firmware/       # Aplicação principal STM32 (C++) — main loop, SensorManager, comunicação serial
├── drivers/        # Drivers por sensor (C++) — um módulo por sensor, independentes e reutilizáveis
├── python/         # Biblioteca Python — leitor serial, parser, processamento de sinais, utilitários
├── notebooks/      # Jupyter — exploração interativa, visualização, ML (importa de python/)
├── datasets/       # Dados organizados: raw/, processed/, features/, models/
├── experiments/    # Experimentos documentados
└── docs/           # Documentação técnica: datasheets e READMEs por sensor, referências STM32
```

## Stack

- **Firmware/Drivers:** C++, STM32 HAL
- **Pipeline de dados:** Python (pyserial, pandas, numpy, scipy)
- **Análise/ML:** Jupyter Notebook, scikit-learn
- **Protocolo serial:** STM32 → USB Serial → Python (protocolo a definir)

## Sensores (em ordem de estudo)

| Sensor  | Interface | Grandeza               | Status         |
|---------|-----------|------------------------|----------------|
| MPU6500 | I²C       | Aceleração, vel. angular | Estudando      |
| BME280  | I²C/SPI   | Temp., umidade, pressão | Aguardando     |
| HX711   | GPIO      | Massa (célula de carga) | Aguardando     |
| INMP441 | I²S + DMA | Áudio                  | Aguardando     |
| BME680  | I²C/SPI   | Temp., umidade, pressão, VOC | Aguardando |

## Abordagem pedagógica — IMPORTANTE

Este projeto é usado para aprendizado ativo. Ao ajudar com código ou conceitos:

- **Não entregar implementações prontas.** Guiar o usuário a escrever o código ele mesmo — fazer perguntas, dar dicas progressivas, apontar onde procurar (datasheet, seção X) em vez de extrair a informação diretamente.
- Para dúvidas sobre datasheets: indicar a seção ou página relevante, não transcrever o conteúdo.
- Só dar a resposta direta se o usuário estiver claramente na direção errada ou travado após tentativas e dicas progressivas.
- Modo socrático por padrão; modo explicação direta quando explicitamente pedido.

## Convenções

- Drivers são independentes — não dependem uns dos outros
- Cada experimento deve ter descrição, sensores usados, frequência de aquisição e condições documentadas
- Dados brutos nunca são sobrescritos — sempre preservar em `datasets/raw/`
