# Agri-Bot

## ESP32 code

(include wiring diagrams at some point)

consider multithreading the receiver or all the ESPs as such:
Radio ESP:
core 0: main UART code
core 1: tasks for running mqtt

Motor ESPs:
core 0: main MQTT sub code
core 1: change motor speed