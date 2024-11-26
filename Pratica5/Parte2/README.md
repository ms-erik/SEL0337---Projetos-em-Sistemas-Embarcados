# Dispositivo de detecção de furto

Aqui temos uma introdução para o desenvolvimento de um detector de furto utilizando uma esp-32 e um sensor HC-SR04, com integração à raspberry-pi utilizando BLE(Bluetooth low energy).

Seu funcionamento consiste em detectar quando a distância até certo objeto de interesse ultrapassa um limiar, indicando o seu deslocamento, o que envia um sinal sinalizando o furto.

# Pré-requisitos

## Software

Para esta aplicação utilizamos a ESP-IDF como framework de desenvolvimento, então será necessário habilitá-la em seu computador.

Para instalação no Windows siga o tutorial em [ESP-IDF Tools Installer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html) 

No linux, deixamos um tutorial na pasta docs deste repositório.

## Hardware

    - Esp-32
    - Raspberry-pi
    - Sendor HC-SR04
    - Protoboard
    - Jumpers

# Organização do repositório

O repositório está separado em duas partes principais, os códigos da Esp-32 e da Raspberry-pi.

## Códigos da Esp
Os códigos para o desenvolvimento das esp32 seguem a seguinte estrutura:

HC_SR04_BLE/
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   ├── main.c
│   ├── ble.c
│   ├── ble.h
│   ├── distance.c
│   └── distance.h

Na main encontram-se os códigos necessários para rodarmos a aplicação.

### BLE

Os códigos ble.c e ble.h são os necessários para iniciar e estabelecer a comunicação utilizando **BLE**, inicializando o advertising, criando e inicializando o serviço. 


### Distance

Os códigos distance.c e distance.h estabelecem o funcionamento do sensor HC-SR04.


### Main

Integração geral.

## Códigos da Rasp

Para integração com a Raspberry-pi é necessário um simples script em python, disponível no diretório Rasp.

# Roteiro de desenvolvimento

O roteiro para o desenvolvimento encontrasse no diretório docs.

