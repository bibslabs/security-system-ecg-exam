## Criptografias

Este módulo do projeto é onde está contida as mais variadas implementações de criptografia, quando possível estas utilizam o framewok mbedTLS, quando não, serão imeplementações puras de software da literatura.

Atualmente possuimos as seguintes implementações de criptografia, sumário obtido de

https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=9328432

1. AES-CBC (hardware ✅ software ❌)
   
   Criptografia mais comum utilizada, pode ser acelerada por hardware pois muitos microcontroladores atuais possuem aceleração de criptografia AES.

2. 3DES (software ❌)
   
   Criptografia secundária, mais leve, faz parte da suite de criptografias que o TLS suporta.

3. Midori (software ❌)
   
   Criptografia de 128 bits desenvolvida para ser eficiente energeticamente, para ser usada por exemplo em implantes médicos

4. PRESENT (software ✅)
   
   Criptografia de 128 bits aprovada pela ISO/IEC(29192-2P:2012) 

5. Simon (software ✅)
   
   Algoritmo desenvolvido pela NSA (National Security Agency) dos estados unidos, aceita vários tamanhos de chave (64 a 256 bits).

6. Speck (software ✅)
    
   Algoritmo irmão do Simon e também desenvolvido pela NSA mais compacto que o simon e focado em implementações de software, usa chaves de 128 bits


Cada qual destas criptografias possui peculiaridades, com vantagens e desvantagens.
