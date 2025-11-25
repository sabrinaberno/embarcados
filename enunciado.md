# Comedouro Automático para Animais

## Integrantes do Grupo

| Nome Completo | Matrícula |
|---------------|-----------|
| Sabrina Caldas Berno | 211029586 |
| Kathlyn Lara Murussi | 180042378 |
| Daniel Nunes Duarte  | 211062910 |
| Luis Guilherme Gaboardi Lins | 180022962 |

## 1. Descrição do Produto Selecionado

![Exemplo de Comedouro Automático](amazon.png)

**Exemplo de produto comercial:** [PETLIBRO Alimentador automático](https://petlibro.com/products/one-rfid-pet-feeder)

### Funções Principais, Público-alvo e Contexto de Uso

O comedouro automático para animais é um dispositivo que automatiza a alimentação de pets, permitindo que o dono monitore remotamente a quantidade de alimento e as condições de armazenamento.

**Funções principais:**
- Liberação automática de ração mediante detecção de presença do animal **e** identificação via RFID
- Identificação individual do animal através de RFID para diferenciação entre animais
- Monitoramento da quantidade de ração disponível
- Controle de qualidade através de medição de temperatura e umidade do ambiente
- Monitoramento remoto de eventos importantes via ThingsBoard

**Observação:** O sistema utiliza detector de presença e RFID trabalhando em conjunto. O pote só abre quando todas as condições forem atendidas: detecção de presença **E** tag RFID válida **E** temperatura **E** umidade adequada **E** nível de ração suficiente. O LED RGB acende em vermelho quando há temperatura inadequada, e em amarelo quando o nível está baixo. Inicialmente, será configurado para apenas um animal específico.

**Público-alvo:** Proprietários de animais domésticos que buscam praticidade e monitoramento remoto da alimentação de seus pets.

**Contexto de uso:** Residências com animais domésticos, especialmente para donos que passam longos períodos fora de casa ou possuem múltiplos pets.

### Componentes e Sensores Utilizados

**Sensores:**
- **Sensor de Movimento (PIR):** detecta aproximação do animal através de radiação infravermelha
- **Leitor RFID:** identifica o animal através de tag RFID (módulo RC522 - não está na lista mas é necessário para a funcionalidade)
- **Sensor de Temperatura Digital DS18B20:** monitora temperatura do ambiente para garantir qualidade da ração
- **Sensor de peso:** célula de carga com HX711 para medir quantidade de ração (não está na lista, mas é necessário para detectar nível baixo)

**Atuadores e Saídas:**
- **Driver para motores L298N:** controla motor DC que libera ou bloqueia a ração
- **Led RGB 5mm ou Led RGB SMD:** indica status do sistema (verde: normal, amarelo: nível baixo, vermelho: condições inadequadas)

### Tecnologias de Comunicação e Controle Embarcadas

- **Wi-Fi (ESP32):** comunicação sem fio obrigatória para envio de eventos importantes ao ThingsBoard
- **Protocolo MQTT:** transmissão simplificada apenas de eventos críticos para ThingsBoard
- **Plataforma ThingsBoard:** uso simplificado para notificações de eventos importantes (alimentação realizada, alertas)

## 2. Análise Técnica do Funcionamento

### Principais Módulos do Sistema

#### Módulo de Sensoriamento

**Detecção de Presença:**
- Utiliza **Sensor de Movimento (PIR)** para detectar aproximação do animal através de radiação infravermelha
- O sensor PIR é simples e adequado para detectar movimento de animais
- Quando detecta presença, ativa o módulo RFID para leitura da tag

**Identificação via RFID:**
- Módulo RC522 (MFRC522) lê tags RFID passivas (13.56 MHz)
- Tag RFID pode ser uma etiqueta de coleira leve (similar ao PETLIBRO que usa tag de 4,2g)
- O sistema armazena o ID da tag autorizada na memória do ESP32
- Leitura ocorre apenas quando presença é detectada (economia de energia)

**Medição de Peso:**
- Célula de carga com amplificador HX711 mede a quantidade de ração no reservatório
- Permite detectar quando o nível está baixo (necessário reabastecer)
- Calibração simples através de pesos conhecidos

**Monitoramento Ambiental:**
- **Sensor de Temperatura DHT11:** mede temperatura e umidade do ambiente
- Garante que a ração esteja armazenada em condições adequadas
- Leitura baseada em eventos (após detecção de presença) e envio de alertas se fora dos limites

#### Módulo de Atuação

**Controle de Liberação de Ração:**
- **Driver para motores L298N** controla motor DC que aciona o mecanismo de liberação de ração
- Implementação simples: motor aciona um rotor ou válvula que libera ração quando gira
- O motor só é acionado quando todas as condições de segurança são atendidas

**LED Indicador de Status:**
- **Led RGB 5mm ou Led RGB SMD** indica estados do sistema:
  - Verde: sistema funcionando normalmente
  - Amarelo: nível de ração baixo
  - Vermelho: temperatura inadequada
- Feedback visual imediato para o usuário

#### Módulo de Controle

**Processamento Central (ESP32):**
- ESP32 processa todas as leituras dos sensores
- Implementa a lógica de validação múltipla:
  1. Presença detectada **E**
  2. Tag RFID válida **E**
  3. Temperatura adequada (via DS18B20) **E**
  4. Nível de ração suficiente
- Apenas quando todas as condições são verdadeiras, o motor é acionado
- Sistema inicialmente configurado para um único animal (simplificação)

**Gerenciamento de Estado:**
- Estados possíveis: IDLE, DETECTING, READING_RFID, VALIDATING, FEEDING, ERROR
- Máquina de estados simples facilita implementação e depuração

#### Módulo de Interface e Conectividade

**Comunicação Wi-Fi:**
- ESP32 conecta-se à rede Wi-Fi doméstica
- Configuração inicial via portal de captura (WiFiManager) ou credenciais hardcoded para simplificação

**Integração com ThingsBoard (Simplificada):**
- Integração via Wi-Fi para monitoramento remoto
- **Envio apenas de eventos importantes**:
  - Evento de alimentação realizada (timestamp + ID do animal)
  - Alertas críticos: nível de ração baixo, temperatura/umidade inadequada
- Dados enviados via MQTT apenas quando ocorre evento (event-driven, não periódico)
- Wi-Fi é ativado apenas quando necessário para enviar eventos (economia de energia)
- Feedback local sempre disponível via LED RGB, independente da conexão Wi-Fi

### Tecnologias Críticas

#### Protocolos Sem Fio

**Wi-Fi (IEEE 802.11):**
- Protocolo padrão para comunicação sem fio de curto alcance
- ESP32 possui stack Wi-Fi integrado, facilitando implementação
- Permite conexão direta com internet e serviços cloud

**MQTT (Message Queuing Telemetry Transport):**
- Protocolo leve de mensageria publish/subscribe ideal para IoT
- Baixo overhead de rede, adequado para dispositivos com recursos limitados
- Uso simplificado: apenas para envio de eventos
- Reduz consumo de energia e processamento ao enviar dados apenas quando necessário

#### Sistema Operacional Embarcado

**FreeRTOS (via ESP-IDF):**
- Sistema operacional em tempo real usado pelo ESP32
- Permite multitarefa através de tasks, facilitando gerenciamento de sensores e comunicação
- Gerenciamento de memória e recursos do sistema

**ESP-IDF (Espressif IoT Development Framework):**
- Framework oficial da Espressif para desenvolvimento com ESP32
- Fornece APIs de alto nível para Wi-Fi, Bluetooth, sensores e periféricos
- Simplifica desenvolvimento comparado a programação bare-metal

#### Técnicas de Economia de Energia

**Modo Deep Sleep:**
- ESP32 pode entrar em modo deep sleep entre leituras de sensores
- Reduz consumo de energia quando não há atividade
- Acorda periodicamente ou via interrupção externa (ex: sensor PIR)

**Desativação Seletiva de Periféricos:**
- RFID e sensores podem ser desligados quando não em uso
- Wi-Fi é ativado apenas quando necessário para enviar eventos ao ThingsBoard (não mantém conexão permanente)
- Após envio do evento, Wi-Fi pode ser desativado temporariamente para economia de energia
- LED indicador pode usar PWM para reduzir consumo

**Otimização de Ciclo de Leitura:**
- Sensor PIR atua como "gatilho" para ativar outros sensores
- RFID só é lido quando presença é detectada
- Leitura de peso e ambiente pode ocorrer em intervalos maiores (ex: a cada 30 segundos)

## 3. Proposta de Reprodução com ESP32

### 3.1 Descrição Conceitual de Implementação

A implementação utilizará ESP32 como unidade central, integrando 6 dispositivos:

1. **Sensor de Movimento (PIR):** detecta aproximação do animal
2. **Módulo RFID RC522:** identifica o animal através de tag RFID (necessário para funcionalidade, não está na lista)
3. **Sensor de Umidade e Temperatura DHT11:** monitora temperatura e umidade do ambiente
4. **Sensor de peso:** célula de carga com HX711 para medir quantidade de ração (necessário para funcionalidade, não está na lista)
5. **Driver para motores L298N:** controla motor DC que libera a ração
6. **Led RGB 5mm ou Led RGB SMD:** indica status do sistema

**Lógica de funcionamento:**
1. Sensor de Movimento (PIR) detecta animal → ativa RFID
2. Sistema lê tag RFID e verifica se é válida
3. Se tag válida, verifica condições ambientais:
   - Temperatura e umidade (via DHT11) dentro dos limites aceitáveis
   - Nível de ração suficiente (via sensor de peso)
4. Se todas as condições estiverem OK, motor DC (via L298N) libera ração
5. Se qualquer condição estiver inadequada, motor não aciona
6. LED RGB indica status:
   - Verde: sistema funcionando normalmente
   - Amarelo: nível de ração baixo
   - Vermelho: temperatura/umidade inadequada
7. Eventos importantes enviados para ThingsBoard via Wi-Fi quando ocorrem:
   - Alimentação realizada (timestamp)
   - Alertas: nível baixo, temperatura inadequada
   - Wi-Fi é ativado apenas para envio de eventos (economia de energia)

### 3.2. Diagrama de Blocos

![Diagrama de Blocos do Comedouro Automático](fse-t2.drawio.png)

### 3.3. Desafios e Limitações Esperadas
Durante a fase de concepção e análise bibliográfica, identificámos quatro desafios técnicos e limitações principais que prevemos enfrentar durante a implementação do protótipo:

#### 3.3.1. Vibração do Motor vs. Precisão do Sensor de Peso
A célula de carga é um sensor de alta precisão, enquanto o motor DC, controlado pelo L298N, gera vibração considerável durante o seu funcionamento.

* **O Desafio:** No momento em que o motor for acionado para dispensar a ração, a vibração pode corromper a leitura da célula de carga em tempo real.
* **Implicação:** Isto impede uma lógica de funcionamento simples (como "parar o motor quando o peso X for atingido"). Teremos de implementar uma lógica de controlo em ciclo (medir, acionar por X segundos, parar, esperar estabilizar, medir novamente) para garantir a dispensa correta da porção.

#### 3.3.2. Fiabilidade e Posicionamento do Leitor RFID
O nosso design depende da fusão de um gatilho de presença (PIR) com um gatilho de identidade (RFID). A fiabilidade deste último é um ponto crítico.

* **O Desafio (Distância):** A pesquisa bibliográfica sugere que o módulo RFID RC522 tem uma distância de leitura efetiva muito curta, na ordem dos 5 cm. Isto exigirá que o animal encoste a sua tag RFID (coleira) quase diretamente no leitor, o que pode ser um desafio de usabilidade para o animal.

* **Implicação:** O posicionamento físico do leitor RFID no comedouro deve ser cuidadosamente projetado para maximizar a probabilidade de leitura correta, possivelmente exigindo um design que guie o animal a aproximar-se corretamente.

* **O Desafio (Interferência):** Outro estudo apontou que leitores HF-RFID (13.56 MHz) são suscetíveis a interferência de estruturas metálicas. O posicionamento do leitor RFID em relação ao motor DC e outros componentes exigirá testes cuidadosos para evitar falhas de leitura.

#### 3.3.3. Lógica de Identificação em Cenário Multi-Pet ("Roubo de Ração")
Embora o nosso escopo inicial seja para um animal, identificámos uma falha lógica no nosso design híbrido (PIR + RFID) quando aplicado a um ambiente com múltiplos animais.

* **O Desafio:** O sistema valida o animal *antes* de dispensar a comida.
* **Implicação:** Um animal autorizado (Animal 1) pode aproximar-se, ser validado pelo RFID e acionar a dispensa de comida. No entanto, um segundo animal (Animal 2, não autorizado ou dominante) pode ouvir o motor, correr, expulsar o Animal 1 e comer a ração. O nosso sistema registará incorretamente no ThingsBoard que o Animal 1 foi alimentado, quando na verdade foi o Animal 2.

## 4. Pesquisa bibliográfica e tecnológica

### 4.1. Artigos científicos que detalhem uma ou mais de uma das principais tecnologias que viabilizam a existência do produto

#### 4.1.1. Passive Infrared Sensor-Based Occupancy Monitoring in Smart Buildings: A Review of Methodologies and Machine Learning Approaches
<div align="justify">
&emsp;&emsp;
Este artigo é uma revisão abrangente da literatura (de 2015 a 2023) focada exclusivamente no uso de <strong>sensores PIR (Passive Infrared)</strong> para monitorização de ocupação em edifícios inteligentes e sistemas IoT. Os autores destacam que, apesar de existirem muitos tipos de sensores (como câmeras), os PIR são os mais amplamente utilizados devido a três vantagens principais: respeito pela privacidade (são não-intrusivos e não capturam imagens), baixo custo e <strong>consumo de energia extremamente baixo</strong> (cerca de 50 µW), tornando-os a escolha ideal para dispositivos IoT alimentados por bateria.

&emsp;&emsp;
A revisão classifica os sensores PIR em dois tipos: <strong>"binários" (binary-based)</strong> e <strong>"baseados em sinal" (signal-based)</strong>. Os sensores PIR binários, que são os mais comuns e de menor custo, fornecem uma saída digital simples: '1' (movimento detetado) ou '0' (sem movimento). Em contrapartida, os sensores "baseados em sinal" fornecem um sinal analógico mais complexo que pode ser processado para extrair mais detalhes, como a velocidade ou a direção do movimento.

&emsp;&emsp;
A principal conclusão do artigo é que os sensores PIR são excelentes para <strong>"Deteção"</strong> (saber se um espaço está ocupado) e <strong>"Deteção de Atividade"</strong> (saber que <i>algo</i> está a acontecer). No entanto, o artigo identifica que os PIR, por si sós, não conseguem fornecer o nível de informação de <strong>"Identidade"</strong> (saber <i>quem</i> está no espaço). Estas vantagens e desvantagens deixa claro a necessidade de uma arbodagem híbrida para aplicações que exijam identificação individual sem comprometer as vantagens dos PIR.
</div>

#### 4.1.2. Validation of a System for Monitoring Feeding Behavior of Dairy Cows
<div align="justify">
&emsp;&emsp;
Este artigo aborda a tecnologia de um sistema HF-RFID (High-Frequency), que opera a 13.56 MHz. O objetivo do estudo foi comparar diretamente os dados recolhidos pelo sistema RFID com gravações de vídeo "time-lapse" para avaliar a sua precisão na monitorização de vacas .

&emsp;&emsp;
Os resultados mostraram que a tecnologia HF-RFID é extremamente precisa para medir padrões de comportamento. O sistema teve uma "concordância perfeita" (definida por r2=1) com o vídeo na contagem do número total de refeições e uma "correlação excelente" (R2=0.98) na medição da duração total de cada refeição .

&emsp;&emsp;
O estudo também identificou as limitações tecnológicas do HF-RFID. Foram registados 12,6% de falsos negativos (a vaca estava presente, mas a tag não foi lida) e 3,5% de falsos positivos (leituras extras). Os autores atribuem estes erros à orientação variável da tag (ex: a coleira virou) e, principalmente, à interferência de estruturas metálicas próximas que podem atuar como antenas não intencionais.
</div>

#### 4.1.3. Power Evaluation of IOT Application Layer Protocols
<div align="justify"> 
&emsp;&emsp;
Este estudo realiza uma avaliação técnica e comparativa do consumo de energia de quatro protocolos de aplicação de IoT: <strong>MQTT, MQTT-SN, CoAP e HTTP</strong>. A análise destaca as arquiteturas de transporte fundamentalmente diferentes de cada um, um fator crítico para a seleção em projetos de engenharia. O artigo estabelece que protocolos como o CoAP operam sobre UDP, sendo, portanto, "connectionless" (sem conexão) e não garantindo a fiabilidade da entrega. Da mesma forma, o MQTT-SN foi especificamente desenhado para redes de sensores que não dependem do TCP/IP.

&emsp;&emsp;
Em contraste, o MQTT (standard) e o HTTP operam sobre TCP, estabelecendo uma comunicação "orientada à conexão" que é essencial para cenários que exigem <strong>fiabilidade na entrega de mensagens</strong>. Tendo estabelecido esta divisão, a pesquisa compara o desempenho energético entre as duas opções baseadas em TCP, utilizando o simulador COOJA e a ferramenta Powertrace para decompor o consumo de energia (mW) nos estados de CPU, Transmissão (TX) e Receção (RX).

&emsp;&emsp;
Os resultados quantitativos demonstram uma diferença drástica de eficiência entre as duas opções TCP. O protocolo HTTP apresentou um consumo de energia muito elevado, especialmente no modo de Transmissão (TX), levando o estudo a concluir que o seu uso "não é de todo recomendado no campo da Internet das Coisas". Embora o MQTT (standard) tenha apresentado um consumo de energia total ligeiramente superior ao das suas contrapartes baseadas em UDP (CoAP e MQTT-SN), ele é vastamente mais eficiente que o HTTP. O estudo posiciona o MQTT como a solução fiável (baseada em TCP) viável, cujo principal overhead de energia em relação ao CoAP se deve ao maior custo de transmissão (TX) exigido pelo TCP.
</div>

#### 4.1.4. Development of an Automated Electronic Estimation Weighing Scale

<div align="justify">
&emsp;&emsp;
Este artigo detalha a tecnologia de medição de peso digital, focando no funcionamento e na justificação da combinação de uma célula de carga (load cell) com um módulo amplificador HX711. O estudo justifica esta abordagem como uma solução para a baixa precisão, baixa resolução e problemas crónicos de calibração encontrados em métodos de medição analógicos.

&emsp;&emsp;
A pesquisa explica a função de cada componente: a célula de carga atua como o sensor (transdutor) que converte a pressão mecânica (peso) num sinal elétrico equivalente, que é proporcional à carga aplicada. No entanto, este sinal elétrico é de nível muito baixo e não pode ser lido diretamente por um microcontrolador.

&emsp;&emsp;
É aqui que o HX711 é introduzido como o componente tecnológico chave. O HX711 é um circuito integrado (CI) dedicado, especificamente desenhado para "permitir que as células de carga meçam o peso". A sua função principal é amplificar e processar o sinal analógico da célula de carga, lendo a diferença de resistência do sensor e convertendo-a para um formato digital que o microcontrolador (neste caso, um Atmega328) pode processar. O artigo também salienta que a calibração com um peso conhecido é um passo fundamental e necessário no processo para garantir medições precisas. Os resultados dos testes validaram que, após a calibração, o sistema atingiu alta precisão, "sem erros".

</div>

### 4.2. Artigos científicos sobre aplicação / uso do produto

#### 4.2.1. Wireless Sensor Networks for Real-Time Monitoring and Controlling of Poultry Farm
<div align="justify">
&emsp;&emsp;
Este artigo propõe um sistema de monitorização e controlo automático para aviários, focado em substituir os processos manuais para melhorar a produção e reduzir custos. O trabalho estabelece que a gestão de fatores ambientais e operacionais é crítica para a saúde das aves e o sucesso do aviário. O sistema proposto valida a eficácia da utilização de uma rede de sensores sem fios (WSN) para monitorizar e controlar ativamente estas condições em tempo real.

&emsp;&emsp;
O design implementa múltiplos circuitos de controlo baseados em sensores, demonstrando como diferentes sensores podem acionar atuadores específicos para manter as condições ideais:

<ul>
  <li><strong>Controlo de Temperatura:</strong> Um sensor de temperatura LM35 monitoriza continuamente o ambiente. Se a temperatura ultrapassar um limite pré-definido, o sistema aciona automaticamente um ventilador (FAN) para arrefecer o local.</li>
  <li><strong>Controlo de Nível de Água:</strong> O sistema utiliza um sensor de nível de água para garantir que os bebedouros não fiquem secos. Quando o nível de água está baixo, o controlador ativa um motor (bomba de água) para reabastecer, desligando-o quando o nível adequado é atingido.</li>
  <li><strong>Controlo de Luz:</strong> Um sensor de luz (LDR ou resistência sensível à luz) é usado para detetar a intensidade da luz ambiente. Com base nessa leitura, o sistema liga ou desliga automaticamente a iluminação interna do aviário.</li>
  <li><strong>Controlo de Segurança:</strong> O estado do portão é monitorizado (simulado por um "Door Button"). Se o portão for aberto, um alarme é acionado, e o estado é enviado para a página de monitorização.</li>
</ul>
</div>

#### 4.2.2. Online feeding behavior monitoring of individual group-housed grow-finish pigs using a low-frequency RFID electronic feeding system
<div align="justify">
&emsp;&emsp;
Este artigo apresenta uma estrutura para o <strong>monitoramento do comportamento alimentar em tempo real</strong> de porcos, utilizando um sistema de alimentação eletrônico baseado em RFID de baixa frequência (LF). O principal objetivo do estudo é usar os dados recolhidos (tempo diário no comedouro) para construir um modelo preditivo capaz de <strong>detectar anomalias no comportamento alimentar</strong>, que são indicadores precoces de <strong>problemas de saúde</strong>.

&emsp;&emsp;
A metodologia envolveu a monitorização individual de 2.826 porcos. Os dados de RFID, recolhidos a cada 20 segundos, foram primeiro agrupados em "visitas" e depois em "refeições", usando um critério de intervalo mínimo de 13 minutos entre as refeições. Utilizando estes dados, desenvolveram um modelo matemático para prever o comportamento alimentar esperado de cada porco. O sistema então comparava o comportamento real de cada porco com o comportamento previsto, gerando um "alerta de saúde" sempre que uma anomalia era detetada .

&emsp;&emsp;
Os resultados demonstraram o enorme potencial de aplicação desta tecnologia para a <strong>gestão da saúde animal</strong>. O sistema de alerta conseguiu detetar 55,7% dos diagnósticos registados pelos tratadores humanos, além de detectar estes problemas de saúde, em média, 2,8 dias mais cedo do que o tratador.
</div>

#### 4.2.3. Smart dog feeder design using wireless communication, MQTT and Android client

<div align="justify"> 
&emsp;&emsp; Este artigo detalha o projeto de um comedouro inteligente para cães (Smart Dog Feeder) focado em resolver o problema da alimentação irregular para donos ausentes. A arquitetura combina um microcontrolador Arduino Uno como unidade de controlo principal com um módulo NodeMCU (ESP8266) dedicado à comunicação Wi-Fi e ao protocolo MQTT. O sistema utiliza o MQTT para ligar o comedouro a um servidor (baseado em Node.js e MySQL) e, subsequentemente, a uma aplicação Android, permitindo que o utilizador defina horários e porções remotamente.

&emsp;&emsp; O projeto implementa duas funcionalidades centrais que são críticas para o produto: 1) Autenticação seletiva, utilizando um leitor RFID (RC522) para verificar a identidade do cão através de uma tag na coleira antes de dispensar o alimento ; e 2) Medição de porção, utilizando uma célula de carga (load cell) e um módulo amplificador HX711 para medir a quantidade de ração servida. Os dados de configuração e os relatórios de alimentação são trocados entre o dispositivo e a aplicação através de objetos JSON sobre MQTT. Os testes confirmaram a funcionalidade do sistema, mas identificaram limitações na precisão da porção, atribuídas ao design mecânico do dispensador (uma hélice) que libertava a ração em incrementos de 20g.
</div>

#### 4.2.4. The Design of Smart Prototype Pet Feeder Using Passive InfaRed (PIR) Sensors

<div align="justify"> &emsp;&emsp; Este artigo apresenta o projeto de um protótipo de comedouro automático para gatos, focado numa implementação de baixo custo. O sistema é controlado por um microcontrolador ATMega8535 e utiliza um sensor PIR (Passive InfraRed) como o seu principal e único gatilho de ativação. A lógica de operação é direta: quando o sensor PIR deteta movimento (a uma distância efetiva testada de até 6 metros), ele envia um sinal ao microcontrolador. Este, por sua vez, aciona um motor DC para abrir uma válvula e dispensar a ração.

&emsp;&emsp; Ao contrário de sistemas baseados em peso, a medição da porção neste projeto é controlada por um sensor Fotodíodo, que deteta quando a comida dispensada atinge um limite pré-determinado no prato, fechando então a válvula. Embora os testes tenham comprovado o funcionamento do mecanismo, o artigo conclui identificando a principal fraqueza do design: a falta de seletividade. O sensor PIR aciona o comedouro para qualquer ser vivo que emita calor (incluindo humanos ou outros animais) que passe por perto, não apenas o animal de estimação designado.
</div>

## 5. Análise de Produtos Relacionados (Benchmarking)

### 5.1. Contextualização e Metodologia

A presente seção realiza uma análise comparativa (benchmarking) entre um protótipo de comedouro automático baseado em ESP32 com identificação RFID e monitoramento ambiental de um conjunto de produtos comerciais disponíveis no mercado.

O objetivo é situar o projeto, identificar seus diferenciais competitivos e compreender as soluções tecnológicas adotadas pela indústria para problemas análogos. Os produtos selecionados para análise representam gerações ou arquétipos tecnológicos distintos no segmento de alimentadores pet.

### 5.2. Produtos Selecionados para Análise

Os produtos de mercado analisados foram categorizados da seguinte forma:

#### Tipos de Opções 1: Automação Local (Stand-alone)

**Produtos:** P1 (ML MLB-3573904067) e P5 (Amazon B0CF7LZ5KN)

**Descrição Arquitetural:** Soluções que não implementam conectividade (Wi-Fi ou Bluetooth). Toda a lógica de automação (programação de horários e porções) é executada localmente, e a interface com o usuário (IHM) é realizada via painel físico (LCD e botões).

#### Tipos de Opções 2: IoT Conectada (Padrão de Mercado)

**Produtos:** P2 (AliExpress 100500...) e P3 (Amazon B0FBV87WTR)

**Descrição Arquitetural:** Dispositivos IoT que utilizam conectividade Wi-Fi (tipicamente 2.4GHz) para integração com uma plataforma de nuvem proprietária. O controle, programação e monitoramento são abstraídos para um aplicativo móvel (App). O foco é o controle remoto e a flexibilidade.

#### Tipos de Opções 3: IoT com Identificação (Nicho Específico)

**Produto:** P4 (Amazon B0FPCTGQTW)

**Descrição Arquitetural:** Este produto representa o concorrente funcional direto do projeto em estudo. Ele implementa a funcionalidade-núcleo de identificação do animal (via tag RFID/coleira) para controle de acesso individual. Diferencia-se do nosso projeto ao utilizar Bluetooth para comunicação com o App, em vez de Wi-Fi para uma plataforma de nuvem.

### 5.3. Tabela Comparativa de Especificações Técnicas

| Característica Técnica | Comedouro Automatico | P1: Programável (ML) | P2: Smart Wi-Fi (AliExpress) | P3: Smart Wi-Fi 6L (Amazon) | P4: Smart RFID (Amazon) | P5: Programável 5L (Amazon) |
|---|---|---|---|---|---|---|
| Geração / Arquitetura | IoT / DIY / Plataforma Aberta | Eletrônica (Stand-alone) | IoT (Cloud Proprietária) | IoT (Cloud Proprietária) | IoT (Controle Local via BT) | Eletrônica (Stand-alone) |
| Identificação do Pet | Sim (RFID RC522) | Não | Não | Não | Sim (Tag RFID proprietária) | Não |
| Controle de Acesso | Sim (Lógica no ESP32) | Não | Não | Não | Sim (Lógica local) | Não |
| Monitoramento de Qualidade | Sim (Sensor Temp. DS18B20) | Não | Não | Sim (Vedação c/ Sílica) | Sim (Rotor Selado) | Não |
| Monitoramento de Nível | Sim (Sensor de Peso HX711) | Visual (Reservatório) | Provável (Sensor IR no App) | Provável (Sensor IR no App) | Visual (Tigela 400ml) | Visual (Reservatório) |
| Interface de Controle | Lógica customizada | Painel LCD + Botões | Aplicativo Móvel | Aplicativo Móvel | Aplicativo Móvel | Painel LCD + Botões |
| Conectividade | Wi-Fi | Nenhuma | Wi-Fi (2.4GHz) | Wi-Fi (2.4G) | Bluetooth | Nenhuma |
| Plataforma de Dados | ThingsBoard (via MQTT) | N/A | App Proprietário | App Proprietário | App Proprietário | N/A |
| Fonte de Energia Primária | Fonte DC (Customizável) | USB | USB (DC5V/1A) | USB | Tomada (Tipo C) | 3 Pilhas D |
| Energia de Backup | Pode ser implementado | 3 Pilhas AA | 3 Pilhas AA | 1 Bateria Lítio (Inclusa) | 4 Pilhas LR14 | N/A (Fonte única) |

### 5.4. Síntese da Análise Comparativa

Conclui-se que o mercado de alimentadores automáticos está segmentado entre soluções de baixo custo sem conectividade (Geração 1) e soluções conectadas (Geração 2), que oferecem conveniência via App, mas não resolvem o problema de controle de acesso em lares com múltiplos animais.

O produto P4 (Geração 3) aborda esse nicho específico, assim como o nosso projeto. Contudo, o Comedouro Automatico se diferencia em dois aspectos técnicos cruciais:

**Monitoramento de Qualidade:** O projeto integra um sensor de temperatura (DS18B20) para monitorar ativamente o ambiente de armazenamento da ração, uma funcionalidade de controle de qualidade não implementada ativamente nos concorrentes (que dependem de vedação passiva).

**Plataforma Aberta:** O uso de Wi-Fi e MQTT para integração com o ThingsBoard confere ao projeto uma flexibilidade e escalabilidade superiores às soluções comerciais, que dependem de aplicativos proprietários e conectividade Bluetooth (como o P4) ou de nuvens fechadas.

O projeto em estudo, portanto, ocupa um nicho de "solução IoT avançada", combinando o controle de acesso individualizado com o monitoramento ambiental ativo.


## 6. Referências

Abdulwahab, M. M., Waled, M. ., Osman, A. A. ., & Al-Tahir, A. . (2022). **Wireless Sensor Networks for Real-Time Monitoring and Controlling of Poultry Farm.** Journal of Telecommunication, Electronic and Computer Engineering (JTEC), 14(1), 21–26. Disponível em: https://doi.org/10.54554/jtec.2022.14.01.004. Acessado em 09 de novembro de 2025.

DeVries, T. J., Von Keyserlingk, M. A. G., Weary, D. M., & Beauchemin, K. A. (2003). **Validation of a system for monitoring feeding behavior of dairy cows.** Journal of dairy science, 86(11), 3571-3574.

Erdoğan, S., & Çindemir, S. (2024). **Investigation of Force Measurement Processes of Load Cells Using STM32G030F6P6TR Microcontroller and HX711 24 Bit Analog to Digital**. 2024 15th National Conference on Electrical and Electronics Engineering (ELECO), 1-5. Disponível em: https://doi.org/10.1109/eleco64362.2024.10847210. Acessado em 09 de novembro de 2025.

Funk, T., Rohrer, G., Brown-Brandl, T., & Keel, B. (2024). **Online feeding behavior monitoring of individual group-housed grow-finish pigs using a low-frequency RFID electronic feeding system**. Translational animal science, 8, txae051 . Disponível em: https://doi.org/10.1093/tas/txae051. Acessado em 09 de novembro de 2025.

Karyono, Kanisius, and I. Hargyo Tri Nugroho. **Smart dog feeder design using wireless communication, MQTT and Android client.** 2016 International Conference on Computer, Control, Informatics and its Applications (IC3INA). IEEE, 2016.

Marlina, Erni. **The Design of Smart Prototype Pet Feeder Using Passive InfaRed (PIR) Sensors.** Journal of Computer Networks, Architecture and High Performance Computing 5.1 (2023): 313-320.

Shahrokhi, A., & Ahmadi, M. (2023). **Power Evaluation of IOT Application Layer Protocols**. 2023 7th International Conference on Internet of Things and Applications (IoT), 1-7. Disponível em: https://doi.org/10.1109/iot60973.2023.10365351. Acessado em 09 de novembro de 2025.

Shokrollahi, A., Persson, J., Malekian, R., Sarkheyli-Hägele, A., & Karlsson, F. (2024). **Passive Infrared Sensor-Based Occupancy Monitoring in Smart Buildings: A Review of Methodologies and Machine Learning Approaches.** Sensors (Basel, Switzerland), 24. Disponível em: https://doi.org/10.3390/s24051533. Acessado em 09 de novembro de 2025.



