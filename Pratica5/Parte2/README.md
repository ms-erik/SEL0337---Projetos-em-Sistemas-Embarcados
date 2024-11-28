# Dispositivo de detecção de furto

Aqui temos uma introdução para o desenvolvimento de um detector de furto utilizando uma esp-32 e um sensor HC-SR04, com integração à raspberry-pi utilizando BLE (Bluetooth low energy).

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

```
HC_SR04_BLE/
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   ├── main.c
│   ├── ble.c
│   ├── ble.h
│   ├── distance.c
│   └── distance.h
```


No diretório main encontram-se os códigos necessários para rodarmos a aplicação.

### BLE

Os códigos ble.c e ble.h são os necessários para iniciar e estabelecer a comunicação utilizando **BLE**, inicializando o advertising, criando e inicializando o serviço. 


### Distance

Os códigos distance.c e distance.h estabelecem o funcionamento do sensor HC-SR04.


### Main

Integração geral.

## Códigos da Rasp

No diretório Rasp temos dois códigos centrais. O notify é um script para mandar um email, indicando o furto, que carrega informações de um arquivo JSON, como senhas e email de contato.
O código principal establece a comunicação com a esp e faz o tratamento dos dados, monitorando a distância do objeto ao sensor e mandando email para o destinatário caso o cenário indesejado ocorra.

# Roteiro de desenvolvimento

O roteiro para o desenvolvimento encontrasse no diretório docs.


# Exemplo de funcionamento

Um vídeo do funcionamento da aplicação encontrasse em [Dispositivo de Furto](https://drive.google.com/file/d/1jwvIjIcPTdUgK3YZiart0jEQs2KiShMS/view?usp=drive_link).

Nele o sensor está inicialmente apontando para a parede a uma distância menor do que $50cm$, sendo monitorado localmente em um computador. Em outro computador é rodado o script em python para interpretar os dados; assim que a distância passa do limiar que decidimos, no caso $50cm$, é enviado um e-mail avisando que seu pertence está sendo movimentado. 

Podemos ver o recebimento do email a seguir:

![Aviso Importante](/home/erik/USP/SEL0337---Projetos-em-Sistemas-Embarcados/Pratica5/Parte2/docs/email.png) 
