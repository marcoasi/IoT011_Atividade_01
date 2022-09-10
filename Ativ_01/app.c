/*Atividade 1

Vence 10/09/2022 às 23:59
Instruções

Construa um projeto onde:- O LED verde deverá ficar piscando, com período de 500ms (250ms aceso, 250ms apagado)- O LED vermelho deverá piscar (acender por 100ms e apagar) a cada vez que o usuário digitar um número;- Caso o usuário pressione a tecla “*”, o LED vermelho deverá ficar aceso e o LED verde deve parar de piscar (apagado);- Voltar ao funcionamento normal quando pressionado o “+”!


Essa atividade é individual.
O código deverá ser disponibilizado no github e o aluno deverá publicar aqui o link do projeto.

Obs: O repositório deve ser de acesso de visualização pública.
Obs.2: Compartilhe com os colegas no grupo, veja a solução dos seus colegas e aprendam juntos!
*/

/*

Aluno: Marco Antonio da Silva
Curso: POS-IoT - Turma 215

*/



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

#define TASK1_PRIORITY 0  // Led verde piscando
#define TASK2_PRIORITY 0  // Led vermelho acendendo sempre que dig numero
#define TASK3_PRIORITY 0  // led vermelho aceso
#define TASK4_PRIORITY 1  // leitura teclado

#define BLACK "\033[30m" /* Black */
#define RED "\033[31m"   /* Red */
#define GREEN "\033[32m" /* Green */
#define DISABLE_CURSOR() printf("\e[?25l")
#define ENABLE_CURSOR() printf("\e[?25h")

#define clear() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

typedef struct
{
    int pos;
    char *color;
    int period_ms;
} st_led_param_t;

st_led_param_t green = {  // led verde piscando em 500ms (250 on / 250 off)
    6,
    GREEN,
    250};
st_led_param_t red = {    // led vermelho piscando em 100ms quando dig numero
    13,
    RED,
    100};

TaskHandle_t greenTask_hdlr, redTask_hdlr, redTaskEdit_hdlr; //, greenTask_edit_hdlr;  //executor task 1, 2 e 3

#include <termios.h>
static void prvTask_getChar(void *pvParameters)
{
    char key;
    int n;

    /* I need to change  the keyboard behavior to
    enable nonblock getchar */
    struct termios initial_settings,
        new_settings;

    tcgetattr(0, &initial_settings);

    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 1;

    tcsetattr(0, TCSANOW, &new_settings);
    /* End of keyboard configuration */
    for (;;)
    {
        int stop = 0;
        key = getchar();   // leitura teclado quando nr pisca o led vermelho piscando em 200ms
                           // se '*' led vermelho aceso e led verde apagado
                           // se '+' tudo roda conforme inicio
                           // se 'z' para o programa
        switch (key)
        {
        case '1':
            vTaskResume(redTask_hdlr);
            break;
        case '2':
            vTaskResume(redTask_hdlr);
            break;
        case '3':
            vTaskResume(redTask_hdlr);
            break;
        case '4':
            vTaskResume(redTask_hdlr);
            break;
        case '5':
            vTaskResume(redTask_hdlr);
            break;
        case '6':
            vTaskResume(redTask_hdlr);
            break;
        case '7':
            vTaskResume(redTask_hdlr);
            break;
        case '8':
            vTaskResume(redTask_hdlr);
            break;
        case '9':
            vTaskResume(redTask_hdlr);
            break;
        case '0':
            vTaskResume(redTask_hdlr);
            break;
        case '*':
            vTaskSuspend(greenTask_hdlr);
            vTaskResume(redTaskEdit_hdlr);
            break;
        case '+':
            vTaskResume(greenTask_hdlr);
            vTaskSuspend(redTaskEdit_hdlr);
            break;
        case 'z':
            vTaskSuspend(greenTask_hdlr);
            vTaskSuspend(redTaskEdit_hdlr);
            vTaskSuspend(redTask_hdlr);
            stop = 1;         
            break;
            //break;
        }
        if (stop)
        {
            break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    tcsetattr(0, TCSANOW, &initial_settings);
    //ENABLE_CURSOR();
    exit(0);
    vTaskDelete(NULL);
}
static void prvTask_ledEdit(void *pvParameters)  // task led vermelho aceso
{
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        // console_print("@");
        gotoxy(led->pos, 2);
        printf("%s⬤", led->color);
        //printf("%s⬤", led->color);
        fflush(stdout);
        xTaskNotify(redTask_hdlr, 1UL, eSetValueWithOverwrite);
        //vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);

        //gotoxy(led->pos, 2);
        //printf("%s ", BLACK);
        //fflush(stdout);
        //vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
static void prvTask_led(void *pvParameters)  // task led verde piscando
{
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        // console_print("@");
        gotoxy(led->pos, 2);
        printf("%s⬤", led->color);
        //printf("%s⬤", led->color);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);

        gotoxy(led->pos, 2);
        printf("%s ", BLACK);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
static void prvTask_led_(void *pvParameters)  // task led vermelho acende quando tecla numero
{
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        // console_print("@");
        gotoxy(led->pos, 2);
        printf("%s⬤", led->color);
        //printf("%s⬤", led->color);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);

        gotoxy(led->pos, 2);
        printf("%s ", BLACK);
        fflush(stdout);
        //vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
void app_run(void)
{

    clear();
    DISABLE_CURSOR();
    printf(
        "╔═════════════════╗\n"
        "║                 ║\n"
        "╚═════════════════╝\n");

    xTaskCreate(prvTask_led, "LED_green", configMINIMAL_STACK_SIZE, &green, TASK1_PRIORITY, &greenTask_hdlr);
    xTaskCreate(prvTask_led, "LED_red", configMINIMAL_STACK_SIZE, &red, TASK2_PRIORITY, &redTask_hdlr);
    vTaskSuspend(redTask_hdlr);
    xTaskCreate(prvTask_led_, "LED_red", configMINIMAL_STACK_SIZE, &red, TASK3_PRIORITY, &redTaskEdit_hdlr);
    vTaskSuspend(redTaskEdit_hdlr);
    xTaskCreate(prvTask_getChar, "Get_key", configMINIMAL_STACK_SIZE, NULL, TASK4_PRIORITY, NULL);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks      to be created.  See the memory management section on the
     * FreeRTOS web site for more details. */
    for (;;)
    {
    }
}