# Código Template para Sensores com ESP32

Esta branch apresenta um código genérico para integrar múltiplos tipos de sensores em uma ESP32, com comunicação via BLE (Bluetooth Low Energy). O código suporta configuração dinâmica dos sensores através de um script Python.

⚠️ **Nota:** Esta branch **feat/escalability** é apenas um exemplo escalável e **não será mergeada** na branch principal. Para o código funcional específico de uma aplicação prática, consulte a branch `main`.

---

## Estrutura do Repositório

A estrutura do repositório é organizada da seguinte forma:

```
HC_SR04_BLE/
├── main/
│   ├── sensors/
│   │   ├── distance.c         # Código para o sensor de distância
│   │   ├── distance.h         # Header para o sensor de distância
│   ├── ble.c                  # Código para BLE
│   ├── ble.h                  # Header para BLE
│   ├── sensor_config.c        # Funções de configuração de sensores
│   ├── sensor_config.h        # Arquivo de configuração de sensores
│   ├── main.c                 # Integração e lógica principal
│   ├── CMakeLists.txt         # Configuração de build para ESP-IDF
├── generate_sensor.py         # Script Python para configurar sensores
├── config.json                # Arquivo JSON de configuração
├── Rasp/
│   ├── notify.py              # Script de notificação por e-mail
│   ├── rasp.py                # Código principal da Raspberry Pi
├── sdkconfig                  # Configuração da ESP-IDF
└── README.md                  # Este arquivo
```

---

## Principais Componentes

### Diretório `main`

#### **`sensors/`**
- **`distance.c` e `distance.h`**: Implementam a funcionalidade para o sensor HC-SR04 (distância).

#### **BLE**
- **`ble.c` e `ble.h`**: Configurações e funções para comunicação via BLE.

#### **`sensor_config`**
- **`sensor_config.c` e `sensor_config.h`**: Arquivos responsáveis por definir o tipo de sensor, sua descrição e ID único, configurados dinamicamente.

#### **`main.c`**
- Arquivo principal que integra BLE e sensores.

### Diretório `Rasp`

- **`notify.py`**: Gerencia o envio de e-mails de notificação.
- **`rasp.py`**: Realiza a comunicação com a ESP32 e monitora os dados recebidos.

---

## Configuração de Sensores

Utilize o script **`generate_sensor.py`** para configurar os sensores. Este script edita dinamicamente o arquivo `sensor_config.h`, permitindo:

- Seleção do tipo de sensor (definido no `sensor_config.h`).
- Definição de uma descrição personalizada para o sensor.
- Geração automática de um ID único (`device_id`).

### Exemplo de Uso do Script

1. Execute o script:
   ```bash
   python generate_sensor.py
   ```
2. Siga as instruções interativas para configurar o sensor.
3. Compile e faça o upload para a ESP32 com o ESP-IDF.

---

## Sistema GATTS Generalista

### Como funciona

O sistema utiliza o **GATTS** (Generic Attribute Profile Server) da ESP32 para criar um servidor BLE que gerencia serviços e características. No contexto desta aplicação:

1. **Estrutura Generalista**:  
   Cada sensor envia informações completas ao cliente BLE (Raspberry Pi), incluindo:
   - **Tipo de sensor**: Identifica o sensor.
   - **Descrição personalizada**: Nome ou uso do sensor.
   - **Valor lido**: Dado atual medido pelo sensor.

2. **Eliminação de Configurações Manuais**:  
   Graças à inclusão desses metadados (tipo e descrição do sensor) na mensagem, o código Python na Raspberry Pi interpreta automaticamente os dados, independentemente do sensor utilizado, sem a necessidade de alterações no script.

3. **Flexibilidade para Novos Sensores**:  
   Adicionar um novo sensor requer apenas sua configuração no arquivo `config.json` e a geração de suas funções no código da ESP32. A Raspberry Pi processa os dados de forma genérica.

### Como o GATTS é Utilizado

- **Serviços e Características**: Um serviço BLE com uma característica armazena as informações do sensor.  
- **Notificações**: Quando um sensor detecta uma alteração, uma mensagem é enviada para o cliente BLE.  
- **Estrutura da Mensagem**:  
  A mensagem enviada ao cliente BLE inclui:
  - ID do sensor
  - Tipo do sensor
  - Valor lido
  - Descrição do sensor  

Essa estrutura permite ao cliente Python na Raspberry Pi tratar os dados sem conhecer os detalhes do sensor.

---

## Exemplo de Funcionamento

1. O sensor HC-SR04 é configurado na ESP32 para monitorar a distância.
2. Quando a distância detectada ultrapassa o limiar configurado (ex.: 50 cm), a ESP32 envia os dados ao cliente BLE.
3. A Raspberry Pi interpreta os dados e dispara um e-mail notificando o evento.

---

## Exemplo de Mensagem BLE

Exemplo de payload enviado pela ESP32:

```json
{
  "sensor_id": 1,
  "sensor_type": DISTANCE_SENSOR,
  "value": 12.5,
  "description": "Sensor de distância"
}
```

Neste caso, a Raspberry Pi interpreta os campos e toma a ação necessária com base no tipo e valor do sensor.

---

## Propósito da Branch

Esta branch serve como exemplo de escalabilidade e flexibilidade no desenvolvimento com ESP32. Ela ilustra como gerenciar diferentes sensores de maneira genérica, mas **não será utilizada em produção**. Consulte a branch principal para o código funcional.
