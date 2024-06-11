#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "object.h"
#include "globals.h"

extern ALLEGRO_BITMAP* dragon_image;
extern ALLEGRO_BITMAP* flappy_image;

const int SPRITE_WIDTH = 191;
const int SPRITE_HEIGHT = 161;

void initDragon(Dragon& dragon, int lives) {
    dragon.lives = lives;
    dragon.score = 0;
    dragon.ID = PLAYER;
    dragon.x = 20;
    dragon.y = 20;
    dragon.speed = 5;
    dragon.xbound = 15;
    dragon.ybound = 12;
    dragon.dragonSprite = dragon_image;
}


void displayDragon(Dragon& dragon) {
    al_draw_bitmap_region(dragon.dragonSprite, SPRITE_WIDTH * 0, SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_HEIGHT, dragon.x - 10, dragon.y - 10, 0);
}


void moveDragon(Dragon& dragon) {
    if (keys[UP])
        dragon.y -= dragon.speed;
    if (keys[DOWN])
        dragon.y += dragon.speed;
    if (keys[LEFT])
        dragon.x -= dragon.speed;
    if (keys[RIGHT])
        dragon.x += dragon.speed;

    if (dragon.x < 0)
        dragon.x = 0;
    if (dragon.y < 0)
        dragon.y = 0;
    if (dragon.x > 800)
        dragon.x = 800;
    if (dragon.y > 600)
        dragon.y = 600;
}

void initFireballs(Fireball fireballs[], int number_of_fireballs) {
    for (int i = 0; i < number_of_fireballs; i++) {
        fireballs[i].ID = FIREBALL;
        fireballs[i].speed = 10;
        fireballs[i].alive = false;
    }
}

void displayFireballs(Fireball fireballs[], int number_of_fireballs) {
    for (int i = 0; i < number_of_fireballs; i++) {
        if (fireballs[i].alive) {
            al_draw_filled_circle(fireballs[i].x, fireballs[i].y, 7, al_map_rgb(255, 165, 0));
        }
    }
}

void fireFireballs(Fireball fireballs[], int number_of_fireballs, Dragon& dragon) {
    for (int i = 0; i < number_of_fireballs; i++) {
        if (!fireballs[i].alive) {
            fireballs[i].x = dragon.x + 120;
            fireballs[i].y = dragon.y + 40;
            fireballs[i].alive = true;
            break;
        }
    }
}

void updateFireballs(Fireball fireballs[], int number_of_fireballs) {
    for (int i = 0; i < number_of_fireballs; i++) {
        if (fireballs[i].alive) {
            fireballs[i].x += fireballs[i].speed;
            if (fireballs[i].x > 800) {
                fireballs[i].alive = false;
            }
        }
    }
}

void initFlappy(Flappy flappy[], int number_of_flappy) {
    for (int i = 0; i < number_of_flappy; i++) {
        flappy[i].ID = ENEMY;
        flappy[i].alive = false;
        flappy[i].speed = 5;
        flappy[i].x = 0;
        flappy[i].y = 0;
        flappy[i].xbound = 18;
        flappy[i].ybound = 18;
        flappy[i].flappySprite = flappy_image;
    }
}

void displayFlappy(Flappy flappy[], int number_of_flappy) {
    for (int i = 0; i < number_of_flappy; i++) {
        if (flappy[i].alive) {
            const int FLAPPY_WIDTH = 34;
            const int FLAPPY_HEIGHT = 24;

            al_draw_scaled_bitmap(flappy[i].flappySprite, 0, 0, al_get_bitmap_width(flappy[i].flappySprite), al_get_bitmap_height(flappy[i].flappySprite),
                flappy[i].x, flappy[i].y, FLAPPY_WIDTH, FLAPPY_HEIGHT, 0);
        }
    }
}

void startFlappy(Flappy flappy[], int number_of_flappy) {
    for (int i = 0; i < number_of_flappy; i++) {
        if (!flappy[i].alive) {
            if (rand() % 500 == 0) {
                flappy[i].alive = true;
                flappy[i].x = 800;
                flappy[i].y = rand() % 600;
                break;
            }
        }
    }
}

void updateFlappy(Flappy flappy[], int number_of_flappy) {
    for (int i = 0; i < number_of_flappy; i++) {
        if (flappy[i].alive) {
            flappy[i].x -= flappy[i].speed;
            if (flappy[i].x < 0) {
                flappy[i].alive = false;
            }
        }
    }
}

void collideFireball(Fireball fireballs[], int number_of_fireballs, Flappy flappy[], int number_of_flappy, Dragon& dragon) {
    for (int i = 0; i < number_of_fireballs; i++) {
        if (fireballs[i].alive) {
            for (int j = 0; j < number_of_flappy; j++) {
                if (flappy[j].alive) {
                    if (fireballs[i].x > (flappy[j].x - flappy[j].xbound) &&
                        fireballs[i].x < (flappy[j].x + flappy[j].xbound) &&
                        fireballs[i].y >(flappy[j].y - flappy[j].ybound) &&
                        fireballs[i].y < (flappy[j].y + flappy[j].ybound)) {
                        fireballs[i].alive = false;
                        flappy[j].alive = false;
                        dragon.score++;
                    }
                }
            }
        }
    }
}

void collideFlappy(Flappy flappy[], int number_of_flappy, Dragon& dragon) {
    for (int i = 0; i < number_of_flappy; i++) {
        if (flappy[i].alive) {
            if (flappy[i].x - flappy[i].xbound < dragon.x + 120 + dragon.xbound &&
                flappy[i].x + flappy[i].xbound > dragon.x + 120 - dragon.xbound &&
                flappy[i].y - flappy[i].ybound < dragon.y + 40 + dragon.ybound &&
                flappy[i].y + flappy[i].ybound > dragon.y + 40 - dragon.ybound) {
                dragon.lives--;
                flappy[i].alive = false;
            }
            else if (flappy[i].x < 0) {
                dragon.lives--;
                flappy[i].alive = false;
            }
        }
    }
}

void initBackground(float x, float y, float dirx, float diry, float velx, float vely, int background_width, int background_height, ALLEGRO_BITMAP* image, Background& bg) {
    bg.x = x;
    bg.y = y;
    bg.dirx = dirx;
    bg.diry = diry;
    bg.x_vel = velx;
    bg.y_vel = vely;
    bg.background_width = background_width;
    bg.background_height = background_height;
    bg.image = image;
}

void updateBackground(Background& bg) {
    bg.x += bg.x_vel * bg.dirx;
    if (bg.x + bg.background_width <= 0) {
        bg.x = 0;
    }
}

void displayBackground(Background& bg, ALLEGRO_BITMAP* Background_image) {
    al_draw_bitmap(bg.image, bg.x, bg.y, 0);
    al_draw_bitmap(bg.image, bg.x + bg.background_width, bg.y, 0);
}

int showMenu(ALLEGRO_FONT* font) {
    ALLEGRO_DISPLAY* display = al_get_current_display();
    bool done = false;
    int option = 0;

    while (!done) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "Escolha a dificuldade:");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "1. Facil");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "2. Medio");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTER, "3. Dificil");
        al_flip_display();

        ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_keyboard_event_source());

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_1:
                option = 1;
                done = true;
                break;
            case ALLEGRO_KEY_2:
                option = 2;
                done = true;
                break;
            case ALLEGRO_KEY_3:
                option = 3;
                done = true;
                break;
            }
        }
        al_destroy_event_queue(event_queue);
    }

    return option;
}

void resetGame(Dragon& dragon, Fireball fireballs[], int number_of_fireballs, Flappy flappy[], int number_of_flappy, Background& bg1, Background& bg2, ALLEGRO_BITMAP* background_image1, ALLEGRO_BITMAP* background_image2) {
    initDragon(dragon, dragon.lives); 
    initFireballs(fireballs, number_of_fireballs);
    initFlappy(flappy, number_of_flappy);
    initBackground(0, 0, -1, 0, 1, 0, 800, 600, background_image1, bg1);
    initBackground(800, 0, -1, 0, 1, 0, 800, 600, background_image2, bg2); 
}

