#include <cpm.h>
#include <rand.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tms.h>

#define GRID_W 64
#define GRID_H 48
#define GRID_CELLS (GRID_W * GRID_H)
#define BUFFERLEN (GRID_CELLS)

#define STARTING_LEN 5
#define GROWLEN 5
#define MAX_APPLES ((GRID_CELLS - STARTING_LEN) / GROWLEN)
#define GAME_SPEED 4

enum e_dir { NORTH, EAST, SOUTH, WEST };

typedef struct {
  int8_t x;
  int8_t y;
} Vec2;

typedef struct {
  Vec2 *head;
  Vec2 *neck;
  Vec2 *body;
  Vec2 *tail;
  uint8_t grow;
} Snake;

Vec2 buffer[BUFFERLEN];

#define RING_NEXT(p) ((p) == &buffer[BUFFERLEN - 1] ? &buffer[0] : (p) + 1)

static Snake snake_buf;
static Vec2 apple_buf;

Snake *snake;
Vec2 *apple;
bool drawflag = false;
bool running = false;
bool won = false;
uint16_t score = 0;
uint8_t dir, k;
uint8_t gamespeed = 4;
uint16_t seed = 0;

void crlf() { puts("\r\n"); }

void delay(uint8_t t) {
  do {
    tms_wait();
  } while (--t > 0);
}

void fatal(char *msg) {
  tms_set_reg(7,0x16);
  puts(msg);
  crlf();
  printf("SCORE: %d", score);
  crlf();
  delay(8);
  tms_set_reg(7,0x1A);
  exit(0);
}

void quit() { fatal("Quitter!"); }

void victory() {
  puts("YOU WIN!");
  crlf();
  exit(0);
}

Vec2 *new_apple() {
  bool taken = true;
  while (taken) {
    apple_buf.x = (rand() & 0xFF) % GRID_W;
    apple_buf.y = (rand() & 0xFF) % GRID_H;
    if (!tms_plot_mc(apple_buf.x, apple_buf.y, LIGHT_GREEN))
      taken = false;
  }
  return &apple_buf;
}

Snake *new_snake() {
  Snake *s = &snake_buf;
  Vec2 v;
  uint8_t i;
  memset(&buffer, 0, sizeof(buffer));

  for (i = 0; i < STARTING_LEN; ++i) {
    v.x = 27 + i;
    v.y = 24;
    buffer[i].x = v.x;
    buffer[i].y = v.y;
  }
  s->head = &buffer[i - 1];
  s->neck = &buffer[i - 2];
  s->body = &buffer[i - 3];
  s->tail = &buffer[0];
  s->grow = 0;
  return s;
}

void move_snake(Snake *s, uint8_t dir) {
  Vec2 newseg;
  newseg.x = s->head->x;
  newseg.y = s->head->y;
  switch (dir) {
  case NORTH:
    newseg.y--;
    break;
  case EAST:
    newseg.x++;
    break;
  case SOUTH:
    newseg.y++;
    break;
  case WEST:
    newseg.x--;
    break;
  default:
    break;
  }

  if ((newseg.x > GRID_W-1) || (newseg.x < 0) || (newseg.y > GRID_H-1) || (newseg.y < 0)) {
    fatal("CRASHED INTO WALL");
  }

  s->head = RING_NEXT(s->head);
  s->neck = RING_NEXT(s->neck);
  s->body = RING_NEXT(s->body);
  if (s->grow == 0) {
    s->tail = RING_NEXT(s->tail);
  } else {
    s->grow--;
  }
  s->head->x = newseg.x;
  s->head->y = newseg.y;
}

bool draw_snake(Snake *s) {
  tms_plot_mc(s->tail->x, s->tail->y, BLACK);
  tms_plot_mc(s->neck->x, s->neck->y, MAGENTA);
  tms_plot_mc(s->body->x, s->body->y, DARK_RED);
  return tms_plot_mc(s->head->x, s->head->y, CYAN);
}

void main() {
  tms_init_mc(BLACK, DARK_YELLOW, false, false);
  running = true;
  srand(seed);
  won = false;
  snake = new_snake();
  dir = EAST;
  draw_snake(snake);
  apple = new_apple();
  tms_mcflush(tms_buf);

  crlf();
  puts("Press SPACE to play...\r\n");
  while (cpm_dc_in() != ' ') {
    ++seed;
  }

  while (running) {
    delay(1);
    --gamespeed;
    k = cpm_dc_in();
    switch (k) {
    case 'a':
    case 'A':
      if (dir != EAST)
        dir = WEST;
      break;
    case 's':
    case 'S':
      if (dir != NORTH)
        dir = SOUTH;
      break;
    case 'd':
    case 'D':
      if (dir != WEST)
        dir = EAST;
      break;
    case 'w':
    case 'W':
      if (dir != SOUTH)
        dir = NORTH;
      break;
    default:
      break;
    }
    if (gamespeed == 0) {
      move_snake(snake, dir);
      if (draw_snake(snake)) {
        if ((snake->head->x == apple->x) && (snake->head->y == apple->y)) {
          snake->grow = GROWLEN;
          score++;
          if (score >= MAX_APPLES)
            won = true;
          else
            apple = new_apple();
        } else {
          fatal("CRASHED INTO TAIL");
        }
      }
      tms_mcflush(tms_buf);
      gamespeed = GAME_SPEED;
      if (won && snake->grow == 0)
        victory();
    }
  }
}
