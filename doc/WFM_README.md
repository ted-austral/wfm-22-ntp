


![Austral Ingenieria](https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcQooGo7vQn4t9-6Bt46qZF-UY4_QFpYOeh7kVWzwpr_lbLr5wka)

# Sobre WiFi en ESP32 y el uso de la biblioteca WiFiManager

## Objetivo

 Una de las razones importantes de la elección del microcontrolador ESP32 es la posibilidad que tiene de conectarse a Internet por medio de WiFi; sin embargo, siendo la conexión a WiFi muy dependiente de la ubicación donde se encuentre el microcontrolador, ya que por su bajo costo, su pequeño tamaño y su relativo bajo consumo, es un candidato a hacer sistemas transportables o que pueden ser instalados en lugares que no se pueden decidir en el momento del desarrollo.  

 Obviamente para conectarse a WiFi hay que saber de antemano el nombre del Punto de Aceeso (_AP o Access Point_) y la clave bajo la cual permite la conexión, con lo cual debe proveerse algún método que pueda cambiarse fácilmente de acuerdo a las circunstancias.  

 Existe ya desde hace mucho tiempo (aproximadamente año 2015) una biblioteca denominada _WiFiManager_ (_tzapu_) que fué creada para el _ESP8266_ que es el antecesor del _ESP32_. Lamentablemente, esa biblioteca no fué expandida para el _ESP32_ por _tzapu_ (quien tampoco ya mantiene la de _ESP8266_); sin embargo, es la biblioteca con más posibilidades de todas las existentes y, por lo tanto, la migración a _ESP32_ y su mantenimiento ha sido tomada por _tablatronix_.   

 A continuación, se dan los links al repositorio original de _tzapu_ asi como las nuevas versiones mantenidas por _tablatronix_.  

  [tzapu/WiFiManager](https://github.com/tzapu/WiFiManager)  
  [WiFi Configuration manager with web configuration portal for Espressif ESPx boards](https://www.arduinolibraries.info/libraries/wi-fi-manager)

 Si bien en el IDE de Arduino, no hay problemas de cargar la biblioteca extendida a _ESP32_ y mantenidas por _tablatronix_, en _Platformio_ no está catalogada la biblioteca _WiFiManager_ para _ESP32_; la razón es justamente que no está mantenida por su autor original, por lo cual hay que pasar a instalarla como biblioteca propia de cada proyecto; para ello, puede observarse que en el directorio _lib_ del proyecto _wfm-22-ntp_ existe otro directorio denominado _WiFiManager-2.0.9-beta_ que es justamente la versión _2.0.9-beta_ extraída el respectivo portal

 Si bien la biblioteca es muy extensa y esta llena de opciones, se ha decidido hacer un manejo sencillo pero completo basado en las necesidades mínimas que un proyecto debe tener, por lo cual el funcionamiento es así:    

  1. _WiFiManager_, para poder elegir el AP con que se va a conectar, se pone el mismo _ESP32_ como AP y crea una página de Web residente en el _ESP32_ que permite seleccionar el AP deseado y colocar la clave de dicho AP para conectarse
  2. Una vez elegido el AP deseado y haber colocado la clave correcta, se cambia de servicio de AP del _ESP32_ a servicio de estación y se conecta con el AP elegido
  3. Además, el AP conectado y su clave se guardan en la memoria no volatil del ESP32
  4. La proxima vez que el ESP32 se vuelva a conectar, tratará de hacerlo sobre los AP que tiene recordados; en caso que no pueda conectarse con ninguno de ellos, va a repetir el proceso que se comenzó a explicar en el punto 1.
  5. Puede ser que el usuario necesite cambiar en todo o en parte de los AP que están guardados, por lo cual se recurre al método de borrar todos los datos salvados y comenzar nuevamente con la selección de AP

 Es obvio que este procedimiento es lo suficientemente detallado como para hacerlo _a ciegas_ (solo puede ser seguido en el momento de desarrollo por estar conectado a una PC y poder seguir el rastro de por donde va la secuencia de configuración por la conexión serie del _ESP32_).  

Por lo tanto, se agregó una pequeña cantidad de _hardware_ para ayudar al proceso de conexión a WIfi

### Hardware

  1 LED rojo (o del color que se desee)  
  1 resistor de 220 ohm  
  1 pulsador  

### Conexión de hardware

 _Ver platformio.ini para la asignación a los correspondientes GPIO_

 El ánodo del LED rojo se conecta a GPIO _LED_RED_  
 El cátodo del LED se conecta a uno de los extremos del resistor   
 El otro extremo del resistor se conecta a GND  
 El pulsador se conecta en un extremo al GPIO _RESET_WIFI_  
 El otro extremo del pulsador se conecta a GND  

### Codificación

 El funcionamiento del _hardware_ agregado se realiza en dos archivos fuentes de C y sus respectivos archivos de inclusión

 1. _wifi_mgr.cpp_: implementa la lógica de funcionamiento descripta a través de la única función pública _wifi_connect_  
 2. _wifi_mgr.h_  : posee el prototipo de la única función implementada, a saber _wifi_connect_  
    - _wifi_connect_, para poder accionar mediante el pulsador y señalizar mendiante el LED, asi como enviar los mensajes a consola, utiliza funciones de _led_signal.cpp_  
 3. _led_signal.cpp_ implementa 3 funciones y el uso de 3 constantes, a saber:  
    - _led_signal_init_, que inicializa los GPIO del  LED y del pulsador  
    - _get_reset_switch_, que espera para ver si se acciona el pulsador, avisando mediante el LED tiilante el intervalo de tiempo para oprimirlo  
    - _led_signal_, que permite colocar mensajes de seguimiemto en pantalla, asi como enviar una constante que indica el estado de la secuencia de selección de WiFi  
    - Las 3 constantes que indican el estado para la función anterior  
 4. _led_signal.h_: posee los prototipos de las 3 funciones así como la definición de las 3 constantes de estado
 5. Finalmente, en el programa principal del proyecto, para utilizar los servicios de _WiFiManager_ solamente se deben colocar lo siguiente:  
    - Incluir el archivo _wifi_mgr.h_ (línea 35)  
    - En _setup_, luego de inicalizar _Serial_ (lo cual es muy importante), llamar a _wifi_connect_  (línea 57)

### Secuencia de funcionamiento

 1. Ni bien comienza a funcionar el programa, titilará el LED, indicando que mientras titile, se aceptará el cierre del pulsador. La ventana de tiempo para apretar el pulsador esta indicada en _platformio.ini_ como _TIME_KEY_ expresado em milisegundos.  
 2. Si el pulsador se apreta, entonces el LED pasará a iluminar en forma estable, indicando que se empezó la secuencia _AutoConnect_
    - Se debe tomar un celular o una computadora y conectarla por WiFi a un Punto de Acceso (AP) que se denomina _OnDemandAP_ que no posee palabra clave
    - Una vez conectado a ese punto de acceso, se debe ir a través de un _browser_ (por ejemplo Chrome) a la págima de Web _192.168.4.1_ 
    - En dicha página de Web, aparecerá una lista de Puntos de Acceso para conectarse  
    - Elija aquel con el cual se quiere conectar y coloque la clave  
    - Si la conexión es aceptada, el LED se apagará y empezará a funcionar el resto del programa  
    - Si la conexión no es aceptada, el LED titilará rápidaente e indefinidamente.  
    - Para salir de esa situación, se debe hacer _RESET_ del _ESP32_ que generalmente esta indicado por _EN_  
 3. Si el pulsador no se apreta entonces pueden pasar 2 cosas:
    - Si hay conexiones de WiFi memorizadas,se intentará conectarse con alguna de ellas si están presentes y por orden decreciente de intensidad de señal; mientras se realiza este proceso, el LED permanece prendido en forma estable. Cuando se logra conectar, entonces se apagará el LED y se comenzará el funcionamiento del resto del programa
    - Si no hay conexiones de WiFi memorizadas o bien habiendo memorizadas, no existe ninguna de ellas, entonces se entrará en la fase denominada _AutoConnect_ con el LED prendido ya descripta
    
##  Videos

 [WiFiManager with ESP32 - Stop Hard-coding WiFi Credentials!](https://www.youtube.com/watch?v=VnfX9YJbaU8)


  **MUY IMPORTANTE:** recordar que el ESP32 es un microcomputador de reducidas proporciones y costo.  
  Por estas razones, no soporta conexiones de WiFi de 5.8 GHZ y solamente de 2.4 GHZ.  
  Este es uno de los errores más comunes por los cuales no se puede conectar al punto de acceso.  

