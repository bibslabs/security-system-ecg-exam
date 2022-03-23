# Cifrador/Decifrador de mensagens

Esta ferramenta recebe argumentos em um formanto JSON, para criptografar e descriptografar um conjunto de bytes encodado em base64


Exemplo usado para criptografar usando CLEFIA um conjunto de bytes, por exemplo

```
The quick brown fox jumps over 13 lazy dogs.....s...s...s...s...
```
em base64 ficara encodado como 

```
VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIDEzIGxhenkgZG9ncy4uLi4ucy4uLnMuLi5zLi4ucy4uLg
```

voce envia para o STDIN do programa o seguinte JSON, lembrando que o parâmetro key é uma string que representa os bytes no formato hexadecimal.

```
{
    "data":"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIDEzIGxhenkgZG9ncy4uLi4ucy4uLnMuLi5zLi4ucy4uLg==",
    "algo":"CLEFIA",
    "key":"ABABAB",
    "mode":"encrypt"
 }
```

## Pre-requisitos

choco - https://chocolatey.org/

powershell 7.2 - https://aka.ms/PSWindows

Instalar mingw pelo choco (terminal como administrador)
```
choco install mingw
```

habilitar feature de command parsing do power shell 7.2
```
Enable-ExperimentalFeature PSNativeCommandArgumentPassing
```

Ativar flag de commmand parsing
```
$PSNativeCommandArgumentPassing = 'Standard'
```

Compilar (pasta raíz)
```
mingw32-make.exe
```

Executar 
```
.\bin\main $(Get-Content .\test2.json -Raw)
```

É possível modificar os conteudos do json ou adicionar outros JSONs com comandos diferentes parat este


