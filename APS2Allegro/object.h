#pragma once

enum IDS { PLAYER, FIREBALL, ENEMY };

struct Dragon {

    int ID;
    int lives;
    int score;
    int speed;
    float x, y;
    int xbound, ybound;
    int currentFrame;

    ALLEGRO_BITMAP* dragonSprite;

};

struct Fireball {

    int ID;
    bool alive;
    float speed;
    float x, y;

};

struct Flappy {

    int ID;
    bool alive;
    float speed;
    float x, y;
    int xbound, ybound;

    ALLEGRO_BITMAP* flappySprite;

};

struct Background {

    float x, y;
    float dirx, diry;
    float x_vel, y_vel;
    int background_width, background_height;

    ALLEGRO_BITMAP* image;

};
