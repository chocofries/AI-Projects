#include "ShaderLoader.h"
#include "game.h"
#include <iostream>

Game* game;

void init()
{
	game = new Game();
	game->Initialise();
}

void update()
{
	game->Update();
}

void render(void)
{
	game->Draw();
}

void resize(int _x, int _y)
{
	int winW = glutGet(GLUT_WINDOW_WIDTH);
	int winH = glutGet(GLUT_WINDOW_HEIGHT);
}

void mousePosition(int _x, int _y)
{
	game->MouseMove(_x, _y);
}

void mouseClick(int _button, int _state, int _x, int _y)
{
	game->MouseClick(_button,_state,_x,_y);
}

void keyPress(unsigned char _key, int _x, int _y)
{
	game->KeyPress(_key, _x, _y);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Tic Tac Toe - AI Summative 1 - John Bu");

	glewInit();
	init();

	// register callbacks
	glutReshapeFunc(resize);
	glutPassiveMotionFunc(mousePosition);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyPress);
	glutIdleFunc(update);
	glutDisplayFunc(render);
	glutMainLoop();

	return 0;
}

