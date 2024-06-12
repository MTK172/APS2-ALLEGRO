// APS2 - Programação Avançada 5ESBN
// Dragon x Flappy - C / C++ Allegro 5
// Matheus Jitkoski Comparin (2022100287)
// Everton Schnitzler (2022101757)

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "object.h"
#include "globals.h"

#define SCREEN_W 800
#define SCREEN_H 600

bool keys[5] = { false, false, false, false, false };

// Functions - Funções
void initDragon(Dragon& dragon, int lives);
void displayDragon(Dragon& dragon);
void moveDragon(Dragon& dragon);
void initFireballs(Fireball fireballs[], int number_of_fireballs);
void displayFireballs(Fireball fireballs[], int number_of_fireballs);
void fireFireballs(Fireball fireballs[], int number_of_fireballs, Dragon& dragon);
void updateFireballs(Fireball fireballs[], int number_of_fireballs);
void initFlappy(Flappy flappy[], int number_of_flappy);
void displayFlappy(Flappy flappy[], int number_of_flappy);
void startFlappy(Flappy flappy[], int number_of_flappy);
void updateFlappy(Flappy flappy[], int number_of_flappy);
void collideFireball(Fireball fireballs[], int number_of_fireballs, Flappy flappy[], int number_of_flappy, Dragon& dragon);
void collideFlappy(Flappy flappy[], int number_of_flappy, Dragon& dragon);
void initBackground(float x, float y, float dirx, float diry, float velx, float vely, int background_width, int background_height, ALLEGRO_BITMAP* image, Background& bg);
void updateBackground(Background& bg);
void displayBackground(Background& bg, ALLEGRO_BITMAP* Background_image);
int showMenu(ALLEGRO_FONT* font);
void resetGame(Dragon& dragon, Fireball fireballs[], int number_of_fireballs, Flappy flappy[], int number_of_flappy, Background& bg1, Background& bg2, ALLEGRO_BITMAP* background_image1, ALLEGRO_BITMAP* background_image2, int dragon_lives);


// Define images globally - Define as imagens globalmente
ALLEGRO_BITMAP* dragon_image;
ALLEGRO_BITMAP* flappy_image;

int main()
{
    // Initializing Allegro - Inicia o Allegro
    if (!al_init())
    {
        al_show_native_message_box(nullptr, "Error", "Failed to initialize Allegro!", nullptr, nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // Create display - Cria o display
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display)
    {
        al_show_native_message_box(nullptr, "Error", "Failed to create display!", nullptr, nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // Add-ons
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_init_image_addon();
    al_install_keyboard();

    //  Load fonts - Carrega as fontes
    ALLEGRO_FONT* catholic_girls = al_load_font("bloodlust.ttf", 30, 0);
    ALLEGRO_FONT* font = al_load_font("RobotoSlab.ttf", 30, 0);

    // Load images - Carrega as imagens
    ALLEGRO_BITMAP* background_image1 = al_load_bitmap("img/starBG.png");
    ALLEGRO_BITMAP* background_image2 = al_load_bitmap("img/starFG.png");
    dragon_image = al_load_bitmap("img/dragon.png");
    flappy_image = al_load_bitmap("img/flappy.png");

    // Create event queue and timer - Cria fila de eventos e timer
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

    // Register event sources - Registra a fila de eventos
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Variables - Variaveis
    bool workdone = false;
    bool redraw = true;
    bool is_game_over = false;
    bool paused = false;
    bool restart = false;
    bool quit = false;
    bool show_pause_menu = false;

    const int number_of_fireballs = 10;
    const int DIALOG_WIDTH = 400;
    const int DIALOG_HEIGHT = 300;

    int number_of_flappy;
    int dragon_lives;
    int dialog_x = (SCREEN_W - DIALOG_WIDTH) / 2;
    int dialog_y = (SCREEN_H - DIALOG_HEIGHT) / 2;

    // Show menu - Mostra o menu inicial
    int difficulty = showMenu(font);

    // Difficulty - Dificuldade
    switch (difficulty) {
    case 1: // Easy
        number_of_flappy = 10;
        dragon_lives = 3;
        break;
    case 2: // Medium
        number_of_flappy = 15;
        dragon_lives = 2;
        break;
    case 3: // Hard
        number_of_flappy = 20;
        dragon_lives = 1;
        break;
    default: // Default
        number_of_flappy = 10;
        dragon_lives = 3;
        break;
    }

    // Objects - Objetos
    Dragon dragon;
    Fireball fireballs[number_of_fireballs];
    Flappy* flappy = new Flappy[number_of_flappy];
    Background bg1, bg2;

    // Initialization - Inicialização
    initDragon(dragon, dragon_lives);
    initFireballs(fireballs, number_of_fireballs);
    initFlappy(flappy, number_of_flappy);
    initBackground(0, 0, -1, 0, 1, 0, SCREEN_W, SCREEN_H, background_image1, bg1);
    initBackground(0, 0, -1, 0, 1, 0, SCREEN_W, SCREEN_H, background_image2, bg2);

    // Hide mouse cursor - Esconde cursor do mouse
    al_hide_mouse_cursor(display);

    // Start timer - Inicia timer
    al_start_timer(timer);

    // Game loop - Loop do jogo
    while (!workdone)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        // Press key - Pressiona uma tecla
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (!paused)
            {
                switch (ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_UP:
                    keys[UP] = true;
                    break;
                case ALLEGRO_KEY_DOWN:
                    keys[DOWN] = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[LEFT] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[RIGHT] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = true;
                    fireFireballs(fireballs, number_of_fireballs, dragon);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    show_pause_menu = true;
                    paused = true;
                    break;
                }
            }
            else if (paused)
            {
                // Paused 
                switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE: // Pause
                    paused = !paused;
                    if (!paused) {
                        show_pause_menu = false;
                    }
                    break;
                case ALLEGRO_KEY_R: // Reset
                    if (paused) {
                        resetGame(dragon, fireballs, number_of_fireballs, flappy, number_of_flappy, bg1, bg2, background_image1, background_image2, dragon_lives);
                        paused = false;
                        show_pause_menu = false;
                        is_game_over = false;
                    }
                    break;
                case ALLEGRO_KEY_Q: // Exit
                    if (paused) {
                        workdone = true;
                    }
                    break;
                }
            }
        }

        // Press key - Pressiona tecla
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (!paused)
            {
                switch (ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_UP:
                    keys[UP] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    keys[DOWN] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[RIGHT] = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = false;
                    break;
                }
            }
        }

        // Close window - fecha a janela
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            workdone = true;
        }

        // Timer
        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (!paused)
            {
                redraw = true;
                moveDragon(dragon);
                updateBackground(bg1);
                updateBackground(bg2);

                if (!is_game_over && dragon.lives != 0)
                {
                    // Start Game
                    updateFireballs(fireballs, number_of_fireballs);
                    startFlappy(flappy, number_of_flappy);
                    updateFlappy(flappy, number_of_flappy);
                    collideFireball(fireballs, number_of_fireballs, flappy, number_of_flappy, dragon);
                    collideFlappy(flappy, number_of_flappy, dragon);
                }
                else
                {
                    is_game_over = true;
                }
            }
        }

        // Redraw event queue - Redesenha a fila de eventos 
        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            if (!is_game_over)
            {
                // Draw game - Desenha o jogo
                displayBackground(bg1, background_image1);
                displayBackground(bg2, background_image2);
                displayDragon(dragon);
                displayFireballs(fireballs, number_of_fireballs);
                displayFlappy(flappy, number_of_flappy);
                al_draw_textf(catholic_girls, al_map_rgb(255, 255, 255), 10, 10, 0, "Lives: %i   Score: %i", dragon.lives, dragon.score);
            }
            else
            {
                // Game Over
                al_draw_text(catholic_girls, al_map_rgb(255, 255, 255), 400, 280, ALLEGRO_ALIGN_CENTER, "GAME OVER!");
                al_draw_textf(catholic_girls, al_map_rgb(255, 255, 255), 400, 320, ALLEGRO_ALIGN_CENTER, "Score: %i", dragon.score);
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }

        // Pause menu
        if (paused && show_pause_menu)
        {
            al_draw_filled_rectangle(dialog_x, dialog_y, dialog_x + DIALOG_WIDTH, dialog_y + DIALOG_HEIGHT, al_map_rgb(255, 255, 255));
            al_draw_text(catholic_girls, al_map_rgb(0, 0, 0), dialog_x + DIALOG_WIDTH / 2, dialog_y + 50, ALLEGRO_ALIGN_CENTER, "Paused");
            al_draw_text(catholic_girls, al_map_rgb(0, 0, 0), dialog_x + DIALOG_WIDTH / 2, dialog_y + 100, ALLEGRO_ALIGN_CENTER, "Press ESC to Resume");
            al_draw_text(catholic_girls, al_map_rgb(0, 0, 0), dialog_x + DIALOG_WIDTH / 2, dialog_y + 140, ALLEGRO_ALIGN_CENTER, "Press R to Restart");
            al_draw_text(catholic_girls, al_map_rgb(0, 0, 0), dialog_x + DIALOG_WIDTH / 2, dialog_y + 180, ALLEGRO_ALIGN_CENTER, "Press Q to Quit");
            al_flip_display();
        }

    }

    // Clear - Limpeza
    al_destroy_font(font);
    al_destroy_font(catholic_girls);
    al_destroy_bitmap(background_image1);
    al_destroy_bitmap(background_image2);
    al_destroy_bitmap(dragon_image);
    al_destroy_bitmap(flappy_image);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
