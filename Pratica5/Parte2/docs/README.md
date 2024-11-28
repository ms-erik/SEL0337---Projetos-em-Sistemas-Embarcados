# Roteiro de desenvolvimento

## Configuração da ESP para leitura de dados utilizando o sensor HC-SR04

O sensor HC-SR04 usa um sonar ultrassonico para determinar a distância entre objetos, sendo o ponto principal da nossa aplicação, o que irá indicar se o objeto em questão foi ou não movimentado.

Para dar inicio a aplicação, copie a estrutura do diretório HC_SR04_BLE, o que pode ser feito com o comando:

```bash
    idf.py create-project HC_SR04_BLE
 ``` 

> [NOTE]
> O tutorial para utilizar no linux está no arquivo ESP-IDF_linux.md, presente nesse diretório.

Ou insira manualmente o conteúdo dos arquivos, não esqueça dos CMakesLists.txt em ambas as pastas do projeto.

## Desenvolvimento do código

Copie os códigos do diretório HC_SR04_BLE/main. Nele temos os códios para:

### Inicializar o sensor HC-SR04

Os códigos distance.c e distance.h estabelecem o funcionamento do sensor

### Inicializar comunicação

Os códigos ble.c e ble.h inicializam a comunicação utilizando BLE, gerenciando serviços e características para permitir comunicação entre dispositivos. 
Também começa a fazer advertising e tratar os dados recebidos.

### main

O código da main faz a integração entre a inicialização dos serviços BLE e funcionamento do sensor de distância.

### Códigos da Rasp

No diretório Rasp temos dois scripts principais:

**notify.py:** Responsável por enviar um email, pegandos as informações privadas de um arquivo json.
**rasp.py:** Código central que estabelece conexão com a ESP, processa os dados recebidos e usa a função notify caso a condição necessária seja atendida.



## Montagem do circuito

Tendo todo código preparado, faça a montagem do circuito na protoboard, como na imagem a seguir:


![Montagem do circuito](esp32-ultrasonic-sensor-wiring-diagram.jpg) 


Atente ao número dos GPIOS escolhidos e os colocados no código.

É necessário habilitar o *Bluetooth*, abra o menu de configuração:

```bash
    idf.py menuconfig
 ``` 
 Navegue até Component config -> Bluetooth e ative; lembre de salvar as configurações antes de sair.

Agora podemos *buildar* o projeto, rodando a seguinte sequencia de comandos:

```bash
    idf.py build

    idf.py -p /dev/ttyACM0 flash monitor
```

Se o circuito foi montado corretamente e os códigos estão certos, o build deverá funcionar corretamente.


## Configuração da Rasp ou do de um ambiente de recebimento

É possível testar a conexão localmente rodando o código em python no diretório rasp em um computador local, e averiguar o recebimento de mensagens, ou rodar na raspberry-pi.

Ao movimentar o objeto alvo do sensor, deverá ser emitido uma mensagem avisando, o que indica alguém tentando mexer em seus pertences.
