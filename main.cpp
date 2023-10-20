#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <deque>
#include <algorithm>

#define DEFAULT_SNAKE_SIZE 30
#define SCREEN_W 800
#define SCREEN_H 800
#define OFFSET_X 40
#define OFFSET_Y 40
#define SHW 10

int SPEED = 2;
enum Direction {
    DOWN,
    UP,
    LEFT,
    RIGHT
};
enum Direction direction;
int snake_size = DEFAULT_SNAKE_SIZE;

static bool running = true;
SDL_Rect src = {0, 0, SCREEN_H / 32, SCREEN_W / 32};
SDL_Rect des = {20, 20, SCREEN_H - OFFSET_Y, SCREEN_W - OFFSET_X};
SDL_Texture *texture;
SDL_Rect snake_head{SCREEN_H / 2 - SHW, SCREEN_W / 2 - SHW, SHW, SHW};
std::deque<SDL_Rect> snake_tail;
SDL_Rect apple;

void events() {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            running = false;
        } else {
            switch (event.key.keysym.sym)
            {
            case SDLK_DOWN: case SDLK_s:
                if (direction != UP){
                    direction = DOWN;
                }
                break;
            case SDLK_UP: case SDLK_w:
                if (direction != DOWN){
                    direction = UP;
                }
                break;
            case SDLK_LEFT: case SDLK_a:
                if (direction != RIGHT){
                    direction = LEFT;
                }
                break;
            case SDLK_RIGHT: case SDLK_d:
                if (direction != LEFT) {
                    direction = RIGHT;
                }
                break;
            case SDLK_ESCAPE:
                running = false;
            }
        }
    }
    
}

void logic(SDL_Renderer *renderer) {
    switch (direction)
    {
    case DOWN:
        snake_head.y += SPEED;
        break;
    case UP:
        snake_head.y -= SPEED;
        break;
    case LEFT:
        snake_head.x -= SPEED;
        break;
    case RIGHT:
        snake_head.x += SPEED;
        break;
    }

    if (((snake_head.x >= apple.x && snake_head.x <= apple.x + apple.w) &&
        (snake_head.y >= apple.y && snake_head.y <= apple.y + apple.h)) ||
        ((snake_head.x + snake_head.w >= apple.x && snake_head.x + snake_head.w <= apple.x + apple.w) && 
        (snake_head.y + snake_head.h >= apple.y && snake_head.y + snake_head.h <= apple.y + apple.h)))
    {
        snake_size += 10;
        apple.x = rand() % (des.h - apple.h)+ des.y;
        apple.y = rand() % (des.w - apple.w) + des.x;
    }
    
    if ((snake_head.x >= des.w + snake_head.w) || 
        (snake_head.x <= des.x) ||
        (snake_head.y >= des.h + snake_head.h) ||
        (snake_head.y <= des.y)) {
            snake_size = DEFAULT_SNAKE_SIZE;
    }

    std::for_each(snake_tail.begin(), snake_tail.end(), [&](auto &tail) {
        if (snake_head.x == tail.x && snake_head.y == tail.y) {
            snake_size = DEFAULT_SNAKE_SIZE;
        }
    });

    if (snake_head.x >= des.w + snake_head.w) {
        snake_head.x = des.x;
    } else if (snake_head.x <= des.x) {
        snake_head.x = des.w + snake_head.w;
    } else if (snake_head.y >= des.h + snake_head.h) {
        snake_head.y = des.y;
    } else if (snake_head.y <= des.y) {
        snake_head.y = des.h + snake_head.h;
    }
}

void draw(SDL_Renderer *renderer) {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &src, &des);

    SDL_SetRenderDrawColor(renderer, 238, 158, 131, 255);
    SDL_RenderFillRect(renderer, &apple);

    SDL_SetRenderDrawColor(renderer, 239, 186, 101, 255);
    SDL_RenderFillRect(renderer, &snake_head);
    std::for_each(snake_tail.begin(), snake_tail.end(), [&](auto& snake_segment){
        SDL_RenderFillRect(renderer, &snake_segment);      
    });

    SDL_RenderPresent(renderer);
    snake_tail.push_front(snake_head);
    while ((int)snake_tail.size() > snake_size) {
        snake_tail.pop_back();
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        perror("SDL Initialization failed!\n");
    }
    if ((window = SDL_CreateWindow(
    "Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN)) == NULL)
    {
        perror("SDL Window Initialization failed!\n");
    }

    if ((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL) {
        perror("SDL Renderer Initialization failed!\n");
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        SCREEN_W * 2, SCREEN_H * 2
    );
    apple = {
        rand() % (des.h - apple.h) + des.y,
        rand() % (des.w - apple.w) + des.x,
        SHW,
        SHW
        };

    while (running)
    {
        events();
        draw(renderer);
        logic(renderer);
        SDL_Delay(10);
    }
    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}