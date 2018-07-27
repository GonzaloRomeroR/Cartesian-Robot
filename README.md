# Proyecto Final de la cátedra Microcontroladores y electrónica de Potencia.

## Robot cartesiano asistido por visión computacional.

En el presente proyecto se pretende realizar el diseño y análisis de un robot cartesiano que recibe consignas de posición a partir de la información obtenida de la toma de imágenes de su espacio de trabajo.
El sistema consiste en un aplicación en Android que toma imágenes a intervalos regulares de tiempo y envía mediante solicitudes HTTP dichas a imágenes a un servidor. El servidor procesa la imagen clasificándola y obteniendo la posición relativa del objeto en la imagen. Dicha información es enviada a un módulo Wi-Fi ESP8266 que la comunica por I2C a un ATMega328P. Este último es el encargado de mover los motores a las posiciones deseadas.
