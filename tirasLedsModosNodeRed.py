import network
from umqtt.simple import MQTTClient
from machine import Pin, PWM
import neopixel
from time import sleep
import random
from servo import Servo

# Configuración de LEDs
LED_PIN = 15  # Pin de datos de los LEDs
NUM_LEDS = 100  # Número total de LEDs en la tira
np = neopixel.NeoPixel(Pin(LED_PIN), NUM_LEDS)

#Declaro el servo
servo = Servo(Pin(13))
girar_servo = False


# Función para realizar una vuelta completa del servo
def vuelta_completa():
    print("Servo realizando una vuelta completa")
    servo.move(0)
    sleep(1)
    servo.move(90)
    sleep(1)
    servo.move(135)
    sleep(1)
    servo.move(180)
    sleep(1)
    servo.move(135)
    sleep(1)
    servo.move(90)
    sleep(1)
    servo.move(0)
    print("Vuelta completa terminada")

# Colores para los 10 niveles
colores = [
    (255, 0, 0),    # Rojo
    (255, 128, 0),  # Naranja
    (255, 255, 0),  # Amarillo
    (128, 255, 0),  # Verde lima
    (0, 255, 0),    # Verde
    (0, 255, 128),  # Verde agua
    (0, 255, 255),  # Cian
    (0, 128, 255),  # Azul claro
    (0, 0, 255),    # Azul
    (128, 0, 255)   # Violeta
]

# Variables globales
modo = 1  # Modo inicial (1 = Normal, 2 = Secuencial, etc.)

# Función para controlar los LEDs en modo normal
def modo_normal(nivel):
    np.fill((0, 0, 0))  # Limpia la tira de LEDs
    leds_encendidos = int((nivel / 100) * NUM_LEDS)  # Calcula LEDs a encender según el nivel
    for i in range(leds_encendidos):
        color_actual = colores[i % len(colores)]  # Ciclo de colores
        np[i] = color_actual
    np.write()  # Actualiza la tira de LEDs

# Función para controlar los LEDs en modo secuencial
def modo_secuencial():
    np.fill((0, 0, 0))  # Limpia la tira de LEDs
    for i in range(NUM_LEDS):
        color_actual = colores[i % len(colores)]  # Ciclo de colores secuenciales
        np[i] = color_actual
        np.write()
        sleep(0.05)  # Retardo para animación secuencial

# Función para controlar los LEDs en modo aleatorio
def modo_aleatorio():
    np.fill((0, 0, 0))  # Limpia la tira de LEDs
    for i in range(NUM_LEDS):
        color_actual = random.choice(colores)  # Elige un color aleatorio
        np[i] = color_actual
    np.write()

# Función para controlar los LEDs en modo barrido
def modo_barrido():
    np.fill((0, 0, 0))  # Limpia la tira de LEDs
    for i in range(NUM_LEDS):
        color_actual = colores[i % len(colores)]  # Ciclo de colores
        np[i] = color_actual
        np.write()
        sleep(0.1)  # Retardo para barrido
    for i in range(NUM_LEDS - 1, -1, -1):
        np[i] = (0, 0, 0)  # Apaga los LEDs
        np.write()
        sleep(0.1)  # Retardo para barrido inverso

# Función para controlar los LEDs en modo respirar
def modo_respirar():
    for brightness in range(0, 256, 5):
        for i in range(NUM_LEDS):
            np[i] = (brightness, brightness, brightness)  # Aplica un brillo uniforme
        np.write()
        sleep(0.05)
    for brightness in range(255, -1, -5):
        for i in range(NUM_LEDS):
            np[i] = (brightness, brightness, brightness)  # Aplica un brillo uniforme
        np.write()
        sleep(0.05)

# Función para controlar los LEDs en modo parpadeo
def modo_parpadeo():
    np.fill((0, 0, 0))  # Apaga los LEDs
    np.write()
    sleep(random.uniform(0.1, 0.5))  # Espera aleatoria
    np.fill(random.choice(colores))  # Enciende todos los LEDs con un color aleatorio
    np.write()
    sleep(random.uniform(0.1, 0.5))  # Espera aleatoria

# Función para conectar a WiFi
def conectar_wifi():
    print("Conectando a WiFi...")
    sta_if = network.WLAN(network.STA_IF)
    sta_if.active(True)
    sta_if.connect("UTNG_GUEST", "R3d1nv1t4d0s#UT")
    while not sta_if.isconnected():
        sleep(0.3)
    print("WiFi conectada")

# Función para manejar los mensajes MQTT
def llegada_mensaje(topic, msg):
    global modo, girar_servo
    print(f"Mensaje recibido: {msg}")
    if msg == b'1':
        modo = 1  # Cambiar a modo normal
    elif msg == b'2':
        modo = 2  # Cambiar a modo secuencial
    elif msg == b'3':
        modo = 3  # Cambiar a modo aleatorio
    elif msg == b'4':
        modo = 4  # Cambiar a modo barrido
    elif msg == b'5':
        modo = 5  # Cambiar a modo respirar
    elif msg == b'6':
        modo = 6  # Cambiar a modo parpadeo
    elif msg == b'servo':
        girar_servo = True

# Función para suscribirse al broker MQTT
def subscribir():
    client = MQTTClient("client_id", "broker.emqx.io", port=1883)
    client.set_callback(llegada_mensaje)
    client.connect()
    client.subscribe("LEDs/mode")
    client.subscribe("Servo/action")
    print("Conectado a broker MQTT")
    return client

# Conectar a WiFi
conectar_wifi()

# Suscripción al broker MQTT
client = subscribir()

# Ciclo principal
while True:
    client.check_msg()  # Verifica los mensajes MQTT entrantes
    
    if girar_servo:
        vuelta_completa()
        girar_servo = False

    if modo == 1:  # Modo Normal
        nivel = 100  # El valor puede cambiar dependiendo de los valores enviados
        print(f"Modo Normal - Nivel: {nivel}")
        modo_normal(nivel)
    elif modo == 2:  # Modo Secuencial
        print("Modo Secuencial")
        modo_secuencial()
    elif modo == 3:  # Modo Aleatorio
        print("Modo Aleatorio")
        modo_aleatorio()
    elif modo == 4:  # Modo Barrido
        print("Modo Barrido")
        modo_barrido()
    elif modo == 5:  # Modo Respirar
        print("Modo Respirar")
        modo_respirar()
    elif modo == 6:  # Modo Parpadeo
        print("Modo Parpadeo")
        modo_parpadeo()
    
    sleep(1)  # Pausa para estabilidad
