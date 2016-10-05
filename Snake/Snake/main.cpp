#include<GL/glut.h>
#include<algorithm>
#include<Windows.h>
#include<vector>
using namespace std;

const GLint CEIL_SIZE = 30;

const GLint MATRIX_WIDTH = 600;

const GLint MATRIX_HEIGHT = 600;

const GLint MAX_WIDTH = MATRIX_WIDTH / CEIL_SIZE;

const GLint MAX_HEIGHT = MATRIX_HEIGHT / CEIL_SIZE;

const GLsizei daley = 200;

GLsizei directx = 1, directy = 0;

POINT food = { -1, -1 };

bool lose, pause;

class Snake {
public:
  vector<POINT> body;

  int startLength = 3;

  Snake() {
    body.push_back({ MAX_WIDTH / 2, MAX_HEIGHT / 2 });
    for (int i = 0; i < startLength - 1; i++) {
      body.push_back({ body.back().x - 1, body.back().y });
    }
  }

} snake;

bool checkIntersect(GLsizei x, GLsizei y) {
  for (int i = 0; i < snake.body.size(); i++) {
    if (snake.body[i].x == x && snake.body[i].y == y) {
      return 0;
    }
  }
  return 1;
}

void addFood() {
  GLsizei x, y;

  do {
    x = rand() % MAX_WIDTH;
    y = rand() % MAX_HEIGHT;
  } while (!checkIntersect(x, y));

  food = { x, y };

  glutPostRedisplay();
}

void up() {
  if (directy == 0) {
    directy = 1;
    directx = 0;
  }
}

void down() {
  if (directy == 0) {
    directy = -1;
    directx = 0;
  }
}

void right() {
  if (directx == 0) {
    directx = 1;
    directy = 0;
  }
}

void left() {
  if (directx == 0) {
    directx = -1;
    directy = 0;
  }
}

void drawRect(GLint x, GLint y) {
  x = x*CEIL_SIZE + CEIL_SIZE / 2;
  y = y*CEIL_SIZE + CEIL_SIZE / 2;
  glColor3f(0.18, 0.24, 0.24);
  glRectf(x - CEIL_SIZE / 2, y + CEIL_SIZE / 2, x + CEIL_SIZE / 2, y - CEIL_SIZE / 2);
  glColor3f(0.82, 0.86, 0.65);
  glRectf(x - CEIL_SIZE * 2 / 5, y + CEIL_SIZE * 2 / 5, x + CEIL_SIZE * 2 / 5, y - CEIL_SIZE * 2 / 5);
  glColor3f(0.18, 0.24, 0.24);
  glRectf(x - CEIL_SIZE / 4, y + CEIL_SIZE / 4, x + CEIL_SIZE / 4, y - CEIL_SIZE / 4);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  drawRect(food.x, food.y);
  glColor3f(1.0, 1.0, 1.0);
  for (int i = 0; i < snake.body.size(); i++) {
    drawRect(snake.body[i].x, snake.body[i].y);
  }
  glPopMatrix();
  Sleep(daley);
  glutSwapBuffers();
}

void move();

void adjustmentBorder() {
  if (snake.body[0].x >= MAX_WIDTH) {
    snake.body[0].x = 0;
  }
  if (snake.body[0].x < 0) {
    snake.body[0].x = MAX_WIDTH - 1;
  }
  if (snake.body[0].y >= MAX_HEIGHT) {
    snake.body[0].y = 0;
  }
  if (snake.body[0].y < 0) {
    snake.body[0].y = MAX_HEIGHT - 1;
  }
}

bool adjustmentFood(GLint dx, GLint dy) {
  if (snake.body[0].x + dx == food.x && snake.body[0].y + dy == food.y) {
    snake.body.insert(snake.body.begin(), { food.x, food.y });
    addFood();
    return 1;
  }
  return 0;
}

int isVisitedFunctionMove = 1;

void keyboard(unsigned char key, int x, int y) {
  
  if (!isVisitedFunctionMove) {
    return;
  }

  snake.body[0].x -= directx;
  snake.body[0].y -= directy;

  int prevDirectx = directx;

  int prevDirecty = directy;

  switch (tolower(key)) {
    case 'w':up(); break;
    case 's':down(); break;
    case 'a':left(); break;
    case 'd':right(); break;
    case ' ': if (pause) {
      glutIdleFunc(move);
      pause = 0;
    } else {
      glutIdleFunc(NULL);
      pause = 1;
    } break;
  }
  
  if (!checkIntersect(snake.body[0].x + directx, snake.body[0].y + directy)) {
    lose = 1;
  }

  snake.body[0].x += directx;
  snake.body[0].y += directy;

  adjustmentBorder();
  adjustmentFood(0, 0);

  isVisitedFunctionMove = 0;

  glutPostRedisplay();
}

void move(void) {
  if (lose) {
    return;
  }

  if (!checkIntersect(snake.body[0].x + directx, snake.body[0].y + directy)) {
    lose = 1;
    return;
  }

  if(!adjustmentFood(directx, directy)) {
    for (int i = snake.body.size() - 1; i > 0; i--) {
      snake.body[i] = snake.body[i - 1];
    }
    snake.body[0].x += directx;
    snake.body[0].y += directy;
  }

  adjustmentBorder();

  isVisitedFunctionMove = 1;

  glutPostRedisplay();
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, MATRIX_WIDTH, 0, MATRIX_HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char * argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  
  glutInitWindowPosition(250, 25);
  glutInitWindowSize(MATRIX_WIDTH, MATRIX_HEIGHT);
  glutCreateWindow("Snake");

  glClearColor(0.82, 0.86, 0.65, 0.0);

  glShadeModel(GL_FLAT);
  addFood();
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(move);
  glutMainLoop();

  return 0;
}
