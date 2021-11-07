# Sistema esp32

lembretes
- antes de compilar e usar os comandos ```idf.py``` executar o script get-idf.ps1
- compilar ```$ idf.py build```
- gravar ```$ idf.py flash```
- serial ```$ idf.py monitor```

## Arquivo ecg

o arquivo do ecg esta embutido no firmware em application/data_transmission/dump.txt

a leitura dos arquivos do physionet pode ser utilizado pelo script ```parse_data.py``` com isso é possível ler .dat usar numpy para adquirir uma numpy.array dos dados, e converter em bytes. o arquivo dump.txt é acoplado ao firmware
por meio da diretiva do cmake EMBED_TXTFILES, assim ao compilar o código este dado estará dentro do firmware

a leitura ECG embutida possui por volta de 80000 bytes e trata-se de uma leitura de ~15 segundos o arquivo é enviado como uma numpy array, e no lado do servidor ele irá re-encodar os bytes de novo em um numpy array


## Servidor websocket python

para testes simples de código foi criado um servidor em python wesockets que recebe os pacotes do ESP32 e adquire os dados, a fim de testar as implementações do código para rodar o servidor basta executar ```$ python websockinho.py```

Servidor consegue pegar cabeçalhos de comandos e converter bytes recebidos em um plot do eletrocardiograma.

### Bibliotecas necessárias
- websockets
- wfdb -> biblioteca para tratar dados physionet
- pycrypto (ou pycryptodome no windows)

## Criptografias

por meio da implementação mbedTLS do framework esp-idf do esp32 é possível implementar uma série de criptografias
como AES 3DES, blowfish e etc. 

## Esquema de comunicação 

Ao ser ligado (ou pressionado botão) o esp irá iniciar o proceso de conexão segura com o servidor central, ele irá enviar um JSON inicial por meio de _websockets_ que conterá informações básicas acerca do dispositivo e da conexão

```JSON
{
    "MAC":"MAC-ID",
    "Paciente":"Pessoa",
    "chave publica":"<chave>"
}
```

em seguida o próximo pacote será o cabeçalho dos dados do ECG, este cabeçalho atualmente possui a criptografia utilizada e o tamanho da próxima transferência

```JSON
    "criptografia":"aes_cbc",
    "Tamanho":80000,
```

Em seguida o próximo pacote será um _stream_ de bytes, o servidor terá que armazenar o buffer de bytes para posteriormente descriptografar (se necessario) e ler ECG


### Troca de chaves

será implementado uma metodologia de troca de chaves de diffie hellman que adicionará mais uma camada de segurança para a comunicação do servidor, fazendo com que mesmo que algum intruso consiga adentrar a comunicação TLS segura, os dados ainda estejam protegidos por outra camada de criptografia.

A troca de chaves se dará pelo primeiro cabeçalho de autenticação que contém uma chave publica com isso o servidor e o cliente esp32 terão uma chave mixada igual de maneira privada
