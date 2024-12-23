# Device Driver de ADC com ESP32
**Ivan Roberto Wagner Pancheniak Filho - 12624224**

**Leonardo Rodrigues de Sousa - 10716380**

Nesse projeto final foi criado um sistema pelo qual uma ESP32 disponibiliza seu ADC para programas em uma Raspberry 
por meio de um _device driver_, comunicando por I2C.

## Motivação

Esse projeto é motivado pelas possibiliades que ter o ADC de uma ESP32 por um _device driver_ dá a outros projetos que podem ser feitos
em uma Raspberry. Ao se ter o hardware e as conexões da ESP32 abstraidas por um driver, o desenvolvedor não precisa se preocupar com todos
os tratamentos de exeções que podem surgir em uma comunicação I2C, precisando apenas ler um arquivo em `/dev` para acessar dados da
leitura do ADC.
Outros benefícios que surgem ao se ter esse recurso disponibilizado como um arquivo pelo SO, é que tratamentos de _deadlock_, e de quem tem
autoridade sobre o ADC, ficam abstraidos pelo _file handler_, simplificando o tratamento em programas com multiplas _threads_ sobre uma
API única de tratamento de arquivos.

## ESP32

### Programar a Placa

Para programar a ESP32, é necessário ter o SDK padrão da espressif, versão v5.3.1.
Em sistemas com _Nix_, ele pode ser obtido pelo seguinte comando, o qual vai criar um shell com todas as ferramentas necessárias:

```sh
    nix --experimental-features 'nix-command flakes' develop github:mirrexagon/nixpkgs-esp-dev#esp-idf-full -c $SHELL
```

Após isso, é apenas necessário entrar na pasta `parte2/adc-i2c-esp32`, e rodar os seguintes comandos para compilar e carregar o
programa na placa, assumindo que a placa está conectada no computador:

```sh
idf.py set-target esp32-s2 # Trocar esp32-s2 pela placa de interesse
idf.py menuconfig # Isso irá abrir o menuconfig do projeto,
                  # nele são necessárias as seguintes alteracoes:
                  # 1 - Habilitar Component Config → ESP-Driver:I2C Configurations → Enable I2C debug log
                  # 2 - Mudar para Component config → ESP System Settings → Channel for console output → USB CDC
                  # caso a placa seja esp32-s2
idf.py -p /dev/ttyACM0 build flash monitor # Troque /dev/ttyACM0 pela porta em que se encontra a placa
```
### Detalhes da Implementacão

Nesse código, foi utilizado a versão 1.0 do driver de I2C _slave_, podendo então não ser compatível com as próximas versões do SDK 
(veja [aqui](pictures/https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html#i2c-clock-configuration) para
detalhes). Além disso, com a opcão 'Enable I2C debug log' ligada, é possível ver o driver de I2C em acão, como pode ser visto nas 
seguintes fotos.

![parte2-esp32-saida](pictures/parte2-esp32-saida.png)

![parte2-esp32-foto](pictures/parte2-esp32-foto.png)

Para verificar que a ESP realmente está conectando com a rasp, conecte os pinos do I2C e rode o comando `i2cdetect -y 1`, se estiver
funcionando, é para aparecer um _58_, que é o endereco da ESP32.

![parte2-esp32-i2cdetect](pictures/parte2-esp32-i2cdetect.png)

## Kernel Module

### Compilar e carregar o Kernel Module

Para compilar o Kernel Module na pasta `parte2/i2c-device-driver-rasp` é primeiro necessário ter os _Kernel Headers_ da versão do Kernel
que a máquina está rodando, depois, é apenas necessário rodar os seguintes comandos dentro da pasta mencionada:

```sh
    # Para baixar o Linux Header na Raspyberry bookworm
    # sudo apt install linux-headers-rpi-v8
    
    make -C /lib/modules/<versao do kernel>/build M=$(pwd) modules # Compila o Kernel Module
    # Se voce está em uma distribuicao NixOS, voce apenas precisa rodar o seguinte comando, idependentemente de ter os Kernel Headers,
    # apenas trocando linuxPackages_zen.kernel pela versao do kernel que voce está usando
    # make -C $(nix-build -E '(import <nixpkgs> {}).linuxPackages_zen.kernel.dev' --no-out-link)/lib/modules/*/build M=$(pwd) modules
   
    sudo insmod i2c-adc-driver.ko # Carrega o Kernel Module
    ls -lah /dev/i2c_adc_device   # Validar que o Kernel Module criou um arquivo para ser interfaceado
    cat /dev/i2c_adc_device       # Ler o valor do ADC da ESP32
    sudo rmsmod i2c-adc-driver.ko # Remove o Kernel Module
```

Tendo dmseg rodando ao lado, é esperado ter uma resposta parecida com a seguinte (cat falha caso a ESP32 não esteja conectada, como é
o caso na figura).

![parte2-driver-saida](pictures/parte2-driver-saida.png)

#### Detalhes da Implementacão

Mesmo sendo um _Kernel Module_, nem tudo é feito _"do zero"_. Há o driver de I2C no Kernel do Linux, do qual o nosso faz uma requesicão 
de um dispositivo I2C, e quando ele encontro, nos é passado um _handler_ que permite que ele seja escrito e lido.

## Montagem

Para fazer a montagem são necessários 3 jumpers femea-femea e 1 jumper femea-macho.
Com eles, faca as seguintes conexoes:

| RASP | ESP32 |
|------|-------|
|GPIO2 | 1     |  
|GPIO3 | 2     | 
|GND   | GND   |  
|3V3   | 3V3   | 

Com o jumper femea-macho no pino 21 da ESP, esse será o seu probe do ADC.
![parte2-connections](pictures/parte2-connections.png)
![parte2-montagem](pictures/parte2-montagem.jpg)
