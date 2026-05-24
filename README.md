# 👨‍💻 Integrantes

- Erik Naoki Miyasato RM: 565771
- Gustavo Arthur Carvalho Sartori RM: 561650
- Gutemberg Rocha Silva RM: 562267
- João Henrique Batista Leal RM: 564361
- Juliana da Silva Stigliani RM: 561171

---

# 🐾 Coleira Inteligente Pet — Sistema IoT de Monitoramento Veterinário

Link do Youtube: https://youtu.be/7SJ1Nq3RS0k
> Sistema IoT para monitoramento de consultas veterinárias utilizando ESP32, LEDs indicadores, protocolo MQTT, dashboard em tempo real com Node-RED e persistência em banco de dados MySQL na nuvem.

---

## 📋 Descrição do Projeto

Este projeto implementa uma **coleira inteligente para pets** que utiliza tecnologia IoT para alertar tutores sobre a proximidade de consultas veterinárias. Um ESP32 recebe via protocolo MQTT o número de dias restantes até a próxima consulta de cada animal, acendendo LEDs coloridos conforme a urgência — roxo para consulta em 1 dia (com buzzer de alerta sonoro), verde para até 5 dias, amarelo para até 15 dias e vermelho para mais de 15 dias.

O sistema conta com um dashboard completo no Node-RED onde é possível **cadastrar animais**, **registrar consultas realizadas**, **remover pets** e visualizar o status de todos os animais em tempo real. Todos os dados são persistidos em um banco de dados MySQL hospedado no Clever Cloud, garantindo histórico completo e rastreabilidade veterinária.

---

## 🛠️ Tecnologias Utilizadas

| Tecnologia | Função no Projeto |
|---|---|
| **ESP32** | Microcontrolador principal — recebe dados MQTT e controla LEDs |
| **Wokwi** | Simulador online do ESP32 e do circuito da coleira |
| **LEDs** | Indicadores visuais de urgência: roxo, verde, amarelo e vermelho |
| **Buzzer** | Alerta sonoro ativado quando a consulta é em 1 dia |
| **MQTT (PubSubClient)** | Protocolo de comunicação IoT para envio dos dados |
| **HiveMQ Cloud** | Broker MQTT na nuvem com suporte TLS/SSL (porta 8883) |
| **WiFiClientSecure** | Comunicação segura SSL/TLS no ESP32 |
| **Node-RED** | Plataforma de processamento, lógica e dashboard |
| **Clever Cloud** | Hospedagem do banco de dados MySQL na nuvem |

---

## 💡 Lógica dos LEDs e Buzzer

| Cor do LED | GPIO | Dias restantes | Buzzer |
|---|---|---|---|
| 🟣 Roxo | 2 | ≤ 1 dia | ✅ Bipa 3x |
| 🟢 Verde | 4 | 2 a 5 dias | ❌ |
| 🟡 Amarelo | 5 | 6 a 15 dias | ❌ |
| 🔴 Vermelho | 18 | > 15 dias | ❌ |

O ESP32 recebe o número de dias via tópico MQTT, determina qual LED acender e publica o status atual no tópico, que é exibido no dashboard em tempo real.

---

## ⚙️ Instruções de Execução

### Pré-requisitos

- Conta no [Wokwi](https://wokwi.com) (simulação do ESP32)
- Conta no [HiveMQ Cloud](https://www.hivemq.com/mqtt-cloud-broker/) (broker MQTT gratuito)
- [Node-RED](https://nodered.org/) instalado localmente ou em servidor
- Conta no [Clever Cloud](https://www.clever-cloud.com/) com um add-on MySQL criado

---

### 1️⃣ Configurar o Banco de Dados (Clever Cloud)

1. Acesse o painel do Clever Cloud → seu add-on MySQL → **phpMyAdmin**
2. Selecione seu banco no menu esquerdo
3. Clique na aba **SQL**, cole o conteúdo de `database/schema.sql` e clique em **Executar**
4. Confirme que as 3 tabelas foram criadas: `pets`, `historico_consultas` e `log_alertas`

---

### 2️⃣ Configurar o Node-RED

**Instalar os pacotes necessários:**

Acesse `Menu ≡ → Manage Palette → Install` e instale:
```
node-red-dashboard
node-red-node-mysql
```

**Importar o fluxo:**

1. Acesse `Menu ≡ → Import → Clipboard`
2. Cole o conteúdo de `node-red/flow.json`
3. Clique em **Import**

**Configurar a conexão MySQL:**

1. Dê duplo clique em qualquer nó MySQL roxo no fluxo
2. Clique no ícone de lápis ✏️ ao lado do campo Database
3. Preencha com as credenciais do Clever Cloud:

```
Host:     SEU_HOST-mysql.services.clever-cloud.com
Port:     3306
Database: SEU_NOME_DB
Username: SEU_USUARIO
Password: SUA_SENHA
Timezone: America/Sao_Paulo
```

4. Clique em **Update → Done → Deploy**
5. Verifique se aparece `connected` em verde nos nós MySQL

---

### 3️⃣ Configurar o ESP32 no Wokwi

1. Acesse [wokwi.com](https://wokwi.com) e crie um novo projeto ESP32
2. Cole o conteúdo de `esp32/main.ino` no editor
3. Cole o conteúdo de `esp32/diagram.json` no diagrama
4. Monte o circuito conforme a tabela de pinos:

| Componente | Pino ESP32 |
|---|---|
| LED Roxo (+ resistor 220Ω) | GPIO 2 |
| LED Verde (+ resistor 220Ω) | GPIO 4 |
| LED Amarelo (+ resistor 220Ω) | GPIO 5 |
| LED Vermelho (+ resistor 220Ω) | GPIO 18 |
| Buzzer | GPIO 19 |

5. Clique em **▶ Play** para iniciar a simulação

---

## 🔧 Configurações do Broker HiveMQ

```
Host:     SEU_CLUSTER.s1.eu.hivemq.cloud
Porta:    8883 (TLS/SSL obrigatório)
Usuário:  SEU_USUARIO
Senha:    SUA_SENHA
```

---

## 📌 Observações Técnicas

- O Node-RED verifica o banco a cada **5 minutos** para evitar sobrecarga na conta gratuita do Clever Cloud.
- O log de alertas só é gravado quando o **status LED muda**, evitando gravações desnecessárias.
- Ao deletar um pet pelo dashboard, o sistema remove também o estado da memória interna do Node-RED, evitando inconsistências.
- A data da próxima consulta é atualizada automaticamente ao registrar uma consulta realizada.

---

## 📊 Resultados Parciais

- ✅ Comunicação MQTT funcionando entre ESP32 (Wokwi) e Node-RED via HiveMQ Cloud
- ✅ LEDs respondendo corretamente conforme os dias restantes
- ✅ Buzzer ativando apenas em estado de urgência (1 dia)
- ✅ Dashboard com cadastro, remoção e histórico de consultas
- ✅ Persistência de dados no MySQL (Clever Cloud)
- ✅ Log de alertas registrando apenas mudanças de status
