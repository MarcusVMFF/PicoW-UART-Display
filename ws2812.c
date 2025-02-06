#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Definições
#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7

const uint ledRed_pin = 13;
const uint ledBlue_pin = 12;
const uint ledGreen_pin = 11;
const uint button_A = 5;
const uint button_B = 6;
ssd1306_t ssd; // Inicializa a estrutura do display

static volatile uint32_t last_time_A = 0;
static volatile uint32_t last_time_B = 0; 


// Paineis
double desenhonada[25] = { 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0 };
double desenho0[25] = { 0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0.3, 0, 
                        0, 0.3, 0, 0.3, 0,
                        0, 0.3, 0, 0.3, 0,
                        0, 0.3, 0.3, 0.3, 0 };

double desenho1[25] = { 0, 0, 0.3, 0, 0,
                        0, 0, 0.3, 0.3, 0, 
                        0, 0, 0.3, 0, 0,
                        0, 0, 0.3, 0, 0,
                        0, 0.3, 0.3, 0.3, 0 };

double desenho2[25] = { 0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0, 0,
                        0, 0, 0.3, 0, 0,
                        0, 0, 0, 0.3, 0,
                        0, 0.3, 0.3, 0.3, 0 };

double desenho3[25] = { 0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0, 0,
                        0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0, 0,
                        0, 0.3, 0.3, 0.3, 0 };

double desenho4[25] = { 0, 0.3, 0, 0.3, 0,
                        0, 0.3, 0, 0.3, 0,
                        0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0, 0,
                        0, 0, 0, 0.3, 0 };

double desenho5[25] = { 0, 0.3, 0.3, 0.3, 0,
                        0, 0, 0, 0.3, 0,
                        0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0, 0,
                        0, 0.3, 0.3, 0.3, 0 };

double desenho6[25] = { 0, 0.3, 0.3, 0.3, 0,
                        0, 0, 0, 0.3, 0,
                        0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0.3, 0,
                        0, 0.3, 0.3, 0.3, 0 };

double desenho7[25] = { 0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0.3, 0,
                        0, 0, 0, 0.3, 0,
                        0, 0.3, 0, 0, 0,
                        0, 0, 0, 0.3, 0 };

double desenho8[25] = { 0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0.3, 0,
                        0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0.3, 0,
                        0, 0.3, 0.3, 0.3, 0 };

double desenho9[25] = { 0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0.3, 0,
                        0, 0.3, 0.3, 0.3, 0,
                        0, 0.3, 0, 0, 0,
                        0, 0, 0, 0.3, 0 };


//imprimir valor binário 
void imprimir_binario(int num) {
 int i;
 for (i = 31; i >= 0; i--) {
  (num & (1 << i)) ? printf("1") : printf("0");
 }
}

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
    ssd1306_fill(&ssd, false); 
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
            valor_led = matrix_rgb(b=0.0, desenho[24-i], g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
}

// Função de interrupção para quando os botões forem pressionados, aumentam o decrescem o contador que define qual painel aparecerá
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Verificação e debouncing de 300ms
    if (gpio == button_A && (current_time - last_time_A > 300000)) {
        last_time_A = current_time;
        gpio_put(ledBlue_pin, false);
        gpio_put(ledGreen_pin, !gpio_get(ledGreen_pin));
        printf("LED verde alternado!\n");

        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "LED VERDE", 30, 30);
        ssd1306_draw_string(&ssd, "ALTERADO", 30, 50);
        ssd1306_send_data(&ssd);
    }
    else if 
        (gpio == button_B && (current_time - last_time_B > 300000)) {
        last_time_B = current_time;
        gpio_put(ledGreen_pin, false);
        gpio_put(ledBlue_pin, !gpio_get(ledBlue_pin));
        printf("LED azul alternado!\n");
        
        ssd1306_fill(&ssd, false); 
        ssd1306_draw_string(&ssd, "LED AZUL", 30, 30);
        ssd1306_draw_string(&ssd, "ALTERADO", 30, 50);
        ssd1306_send_data(&ssd);
    }
}

int main() {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    double r = 0.0, b = 0.0 , g = 0.0;
    uint32_t valor_led;
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    stdio_init_all();
    
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);


    gpio_init(ledRed_pin);
    gpio_init(ledBlue_pin);
    gpio_init(ledGreen_pin);
    gpio_init(button_A);
    gpio_init(button_B);

    gpio_set_dir(ledRed_pin, GPIO_OUT);
    gpio_set_dir(ledBlue_pin, GPIO_OUT);
    gpio_set_dir(ledGreen_pin, GPIO_OUT);

    gpio_put(ledRed_pin, 0); 
    gpio_put(ledBlue_pin, 0);
    gpio_put(ledGreen_pin, 0);

    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);
    gpio_set_dir(button_B, GPIO_IN);
    gpio_pull_up(button_B);

    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    int last_cont = -1;

    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    while (true) {
      if(stdio_usb_connected()){
        char c;
        if(scanf("%c", &c) == 1){
            printf("Recebido: '%c'\n", c);

    switch (c) {
        case 'A': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "A", 60, 30); break;
        case 'B': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "B", 60, 30); break;
        case 'C': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "C", 60, 30); break;
        case 'D': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "D", 60, 30); break;
        case 'E': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "E", 60, 30); break;
        case 'F': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "F", 60, 30); break;
        case 'G': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "G", 60, 30); break;
        case 'H': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "H", 60, 30); break;
        case 'I': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "I", 60, 30); break;
        case 'J': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "J", 60, 30); break;
        case 'K': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "K", 60, 30); break;
        case 'L': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "L", 60, 30); break;
        case 'M': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "M", 60, 30); break;
        case 'N': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "N", 60, 30); break;
        case 'O': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "O", 60, 30); break;
        case 'P': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "P", 60, 30); break;
        case 'Q': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "Q", 60, 30); break;
        case 'R': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "R", 60, 30); break;
        case 'S': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "S", 60, 30); break;
        case 'T': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "T", 60, 30); break;
        case 'U': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "U", 60, 30); break;
        case 'V': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "V", 60, 30); break;
        case 'W': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "W", 60, 30); break;
        case 'X': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "X", 60, 30); break;
        case 'Y': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "Y", 60, 30); break;
        case 'Z': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "Z", 60, 30); break;
        case 'a': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "a", 60, 30); break;
        case 'b': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "b", 60, 30); break;
        case 'c': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "c", 60, 30); break;
        case 'd': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "d", 60, 30); break;
        case 'e': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "e", 60, 30); break;
        case 'f': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "f", 60, 30); break;
        case 'g': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "g", 60, 30); break;
        case 'h': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "h", 60, 30); break;
        case 'i': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "i", 60, 30); break;
        case 'j': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "j", 60, 30); break;
        case 'k': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "k", 60, 30); break;
        case 'l': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "l", 60, 30); break;
        case 'm': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "m", 60, 30); break;
        case 'n': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "n", 60, 30); break;
        case 'o': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "o", 60, 30); break;
        case 'p': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "p", 60, 30); break;
        case 'q': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "q", 60, 30); break;
        case 'r': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "r", 60, 30); break;
        case 's': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "s", 60, 30); break;
        case 't': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "t", 60, 30); break;
        case 'u': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "u", 60, 30); break;
        case 'v': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "v", 60, 30); break;
        case 'w': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "w", 60, 30); break;
        case 'x': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "x", 60, 30); break;
        case 'y': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "y", 60, 30); break;
        case 'z': desenho_pio(desenhonada, valor_led, pio, sm, r, g, b); ssd1306_fill(&ssd, false); ssd1306_draw_string(&ssd, "z", 60, 30); break;
        case '1': desenho_pio(desenho1, valor_led, pio, sm, r, g, b); break;
        case '2': desenho_pio(desenho2, valor_led, pio, sm, r, g, b); break;
        case '3': desenho_pio(desenho3, valor_led, pio, sm, r, g, b); break;
        case '4': desenho_pio(desenho4, valor_led, pio, sm, r, g, b); break;
        case '5': desenho_pio(desenho5, valor_led, pio, sm, r, g, b); break;
        case '6': desenho_pio(desenho6, valor_led, pio, sm, r, g, b); break;
        case '7': desenho_pio(desenho7, valor_led, pio, sm, r, g, b); break;
        case '8': desenho_pio(desenho8, valor_led, pio, sm, r, g, b); break;
        case '9': desenho_pio(desenho9, valor_led, pio, sm, r, g, b); break;
        case '0': desenho_pio(desenho0, valor_led, pio, sm, r, g, b); break;
    }
    ssd1306_send_data(&ssd);
        }
      }
    }
}
