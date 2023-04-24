# Labirinto-Recorrente

O programa receberá o input de um arquivo que contém, um número K de labirintos de tamanho NxN. Um garoto percorrera os labirintos pegando itens que o ajudam e enfrentando perigos de forma randômica até que ele consiga ganhar o jogo ou morrer.

## Visão geral

O goroto será posicionado aleatóriamente no primeiro labirinto contido no arquivo e se moverá de forma aleatória, quando atingir o limite do labirinto atual, será teletransportado aleatóriamente para o próximo labirinto do arquivo.

O objetivo do garoto é fazer um caminho sem encontrar nenhum item no chão e percorrer todos os labirintos do arquivo.

## Entrada

O programa espera uma entrada na forma de um arquivo chamado "input.data" dentro da pasta "dataset" no mesmo diretório que o executável.

A primeira linha do arquivo deverá conter o tamanho de cada labirinto e a quantidade de labirintos no arquivo. Coforme o exemplo mais abaixo.

Cada labirinto deverá ter um espaço de uma linha em branco de seu anterior, e cada casa do labirinto deverá ser separada por um espaço de sua próxima.

As casas do labirinto são codificadas na seguinte forma:

- Numeros representam casas sem perigo e com items, a quantidade de itens na casa é igual ao número;
- Paredes são representadas por '#';
- Perigos são representados por '*'.

Assim, a imagem a seguir prove um exemplo de entrada válida

![Entrada](/img/Entrada.PNG)
Imagem 1

Temos uma entrada de três labirintos 7x7 cada um.

## Regras

```Geral:``` 

- O garoto pode se mover nas oito direções, isto é, em todas as casas adjacentes.
- O garoto só consegue mover uma casa por vez
- O garoto não consegue ultrapssar paredes ou mover para sua posição

```Items:``` 

- O garoto possui uma sacola, nela ele pode guardar até 4 itens e quando atingir esse limite consegue regenerar um ponto de vida perdido. Caso o garoto esteja de vida cheia seus itens serão jogados no limbo.
- Toda vez que o garoto anda em uma casa com item, ele é obrigado a pegar um item
- O garoto só pode pegar um item por vez

```Vida e perigos:``` 

- A vida máxima e inicial do garoto é 10
- Quando encontrar um perigo, o garoto perde 1 de vida, os perigos não somem quando enfrentados
- Se a vida do garoto for igual a 0, ele morre e perde o jogo

```Movimentação:``` 

- Se o garoto atingir o limite do labirinto atual, será teletransportado para o próximo labirinto em uma posição aleatória. Se ele estiver no ultimo labirinto e for teletranportado, ele voltará para o primeiro labirinto.
- Para o garoto vencer o jogo ele deve percorrer todos os labirintos, porém, ele não deve pegar nenhum item quando percorrer do primeiro ao último labirinto.


```Casos Especiais: ``` 

- Caso após cair em um portal o garoto fique cercado de paredes, o código retornará que o garoto ficou preso entre paredes e foi impossível seguir a sua jornada pelos labirintos.

## Implementação
 
- Foi criado uma matriz do mesmo tamanho dos labirintos, foram criados também 2 arquivos auxiliares, o primeiro arquivo "state" guarda as informações dos labirintos durante a execução, o segundo arquivo guarda se as posições foram exploradas ou não. 
- Para o caminhamento de forma randômica, foi criado duas variáveis que recebiam um valor aleatório de 0 a 1, onde a primeira representava a linha e a segunda representava a coluna. Logo após, o sinal de cada uma das posições é decidido randomicamente. Assim, a cada movimento a posição x e y pode variar entre 1 e -1 cada um.
- A "struct player" armazena todos os valores do jogador. Nela foram contados a quantidade de passos, perigos enfrentados, vida e itens na sacola.
- O arquivo "explored" guarda uma cópia dos labirintos onde cada casa é representada por 0 ou 1, sendo 0 uma casa não explorada e 1 uma casa que já foi explorada. Essas representações são usadas para calcular quantas casa foram exploradas pelo garoto. É importante notar que dessa forma as paredes do labirinto sempre contam como não exploradas.
- Os labirintos em si são representados por uma matriz de uma strutura "tile" que contém um valor inteiro que representa o que há na casa e outro booleano que representa se ela foi explorada ou não.
- A posição inicial é selecionada aleatóriamente e contará como uma interação, por exemplo: se o garoto começar em um perigo ele ja tomará dano.

## Exemplo de execução

Usando a mesma entrada da imagem 1:

![Entrada](/img/Entrada.PNG)

O programa mostrará o estado final dos labirintos assim como informações importantes.

![Saída](/img/Saída.PNG)

No exemplo acima, o garoto infelizmente não conseguiu completar a sua missão pois o mesmo perdeu todas suas vidas, logo após avisar sobre sua derrota, foi listado abaixo todos os dados de sua jornada como as casas percorridas, os itens coletados pelo caminho, as casas não exploradas e os perigos enfrentados.

## Conclusão

Apesar da solução aleatória funcionar em certos casos, ela se mostra muito ineficiente, principalmente devido ao fato de não evitar perigos ou buscar o caminho mais perto para o próximo labirinto. Qualquer solução que implemente esses princípios deva conseguir resolver o problema de forma mais eficiente.

O custo computacional do algoritmo não pode ser calculado devido a sua aleatóriedade em alguns casos ele poderá resolver de forma eficiente e em outros da pior forma possível. Assim, como não há padrão em como o algoritmo resolve o problema, não é possível estimar seu custo e eficiência.

Lidar com os diferentes arquivos e com a dinâmica de ler e salvar o que estava na memória em arquivos é trabalhosa (pricipalmente em C), mas deixa o código mais robusto visto que evita errors de memória em entradas muito grande.

## Estrutura de arquivos

* ```main.c```: Arquivo e função principal do código;
* ```methods.h```: Criação das funções utilizadas;
* ```methods.c```: Implementação das funções descritas no arquivo h;
* ```input.data```: Entrada do programa;
* ```state.data```: Onde o prgrama grava o estado dos labirintos durante a execução;
* ```explored.data```: Onde o prgrama grave se cada casa dos labirintos foi explorada ou não;
* ```Makefile```: Automatiza processos de compilação;

## Compilação e Execução

Esse código possui um arquivo Makefile que realiza todo o procedimento de compilação e execução. Nele temos as seguintes diretrizes de execução:


| Comando                |  Função                                                                                           |                     
| -----------------------| ------------------------------------------------------------------------------------------------- |
|  `make clean`          | Apaga a última compilação realizada contida na pasta build                                        |
|  `make`                | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build           |
|  `make run`            | Executa o programa da pasta build após a realização da compilação                                 |
