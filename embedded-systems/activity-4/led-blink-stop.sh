#!/bin/sh
# Fazer export do pino para avisarmos que ele esta sendo desativado
echo 18 > /sys/class/gpio/export
# Sinalizacao de que ele esta sendo desativado
for i in $(seq 0 3)
do  
  echo 1 > /sys/class/gpio/gpio18/value
  sleep 0.1s
  echo 0 > /sys/class/gpio/gpio18/value
  sleep 0.1s
done
# Fazer unexport para liberar o uso do pino
echo 18 >/sys/class/gpio/unexport
