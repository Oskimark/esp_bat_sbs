# SBS_ESP: Sistema de Batería Inteligente para ESP
codigo inicial inspirado fuertemente de https://github.com/teliot/SBS
Esta librería permite que un ESP8266 (o ESP32) lea datos de baterías compatibles con el estándar "Smart Battery System" (SBS), que normalmente se encuentran en laptops, drones y otros dispositivos portátiles.

¡Esta versión ha sido fuertemente optimizada para la memoria del microcontrolador y adaptada para uso moderno en el taller!

## 🔧 Configuración de Hardware (Pinout)

La mayoría de las baterías de laptop utilizan una interfaz estándar SMBus/I2C. Normalmente encontrarás de 5 a 9 pines en el conector de la batería. El estándar más común es:
1. **VCC (+) / BAT+**: Usualmente varios pines unidos en un extremo.
2. **GND (-) / BAT-**: Usualmente varios pines unidos en el otro extremo.
3. **SCL (Reloj / Clock)**: Pin de reloj SMBus.
4. **SDA (Datos / Data)**: Pin de datos SMBus.
5. **System Present (SysPres/T)**: A menudo necesita estar conectado a GND para despertar la batería y permitir la lectura o descarga.

**Conexión ESP8266 (Ejemplo WebSerial por defecto):**
* ESP `D1` -> Batería `SCL`
* ESP `D2` -> Batería `SDA`
* ESP `GND` -> Batería `GND`
* *Nota: ¡NO conectes el VCC de la batería directamente al ESP a menos que estés usando un regulador step-down adecuado, ya que las baterías de laptop a menudo tienen entre 11.1V y 14.8V!*

> **⚠️ ADVERTENCIA**: Las baterías de laptop pueden entregar corrientes extremadamente altas. ¡Ten cuidado de no hacer cortocircuito en los pines mientras los mides!

## 🚀 Aplicación Web para el Taller (¡Sin Instalación!)

Hemos incluido una **Aplicación Web** moderna y hermosa que se conecta directamente a tu ESP8266 vía USB (usando la API Web Serial). ¡No necesitas pantallas adicionales ni configurar WiFi!

### Cómo usarla:
1. Abre el sketch `examples/WebSerial/WebSerial.ino` en Arduino IDE y compílalo/súbelo a tu ESP8266.
2. Abre el archivo `examples/WebSerial/webapp.html` en un navegador moderno (como Google Chrome o Microsoft Edge).
3. Haz clic en el botón de **Connect**, selecciona el puerto COM de tu ESP, e instantáneamente verás un panel interactivo con la Salud, Estado de Carga (SOC), Voltaje, Corriente, e información del fabricante de tu batería.

### ⚠️ Modo Experto (Programación de Chips)
Para chips como el **BQ40Z551** de Texas Instruments (Familia Z), la aplicación y el firmware ahora soportan funciones avanzadas. Activa el **Modo Experto** en la interfaz para desbloquear el chip (Unseal), borrar el registro histórico (Lifetime Data Reset) y forzar la reescritura de ciclos de carga enviando de forma segura los comandos MAC necesarios vía hardware.

## Nota de la Librería Principal

Para usar esta librería de forma confiable, es posible que necesites reducir la velocidad del reloj I2C a la recomendada por SMBus. La librería maneja esto automáticamente internamente llamando a:
```cpp
Wire.setClockStretchLimit(35000);
Wire.setClock(40000);
```
*(Ver https://github.com/esp8266/Arduino/issues/2524 para contexto histórico).*

## TODOs / Tareas Pendientes
* [x] Añadir archivo de Licencia GPL
* [x] Arreglar fugas de memoria con objetos String
* [x] Añadir Interfaz Gráfica de Taller (Web App local)
* [x] Añadir guardado de Pinouts 
* [x] Implementar Desbloqueo y Escritura para BQ40Z551 (Modo Experto)
