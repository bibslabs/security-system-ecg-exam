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
