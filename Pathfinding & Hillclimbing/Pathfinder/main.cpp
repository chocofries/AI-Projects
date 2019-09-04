#include <iostream>
#include "game.h"
#include "ShaderLoader.h"
#include "inputmanager.h"
#include "utils.h"

Game* globalGame;

//enum InputState
//{
//	KEY_UP,
//	KEY_DOWN,
//	KEY_UP_BEGIN,
//	KEY_DOWN_BEGIN
//};
//
//enum MouseButton
//{
//	MOUSE_LEFT,
//	MOUSE_MIDDLE,
//	MOUSE_RIGHT
//};
//
//InputState KeyState[255];
//InputState MouseState[3];

void init()
{
	globalGame = &Game::GetGameInstance();
	globalGame->Initialise(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void update()
{
	globalGame->Update();
}

void render()
{
	globalGame->Draw();
}

void resize(int x, int y)
{
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void KeyboardDown(unsigned char _key, int x, int y)
{
	globalGame->GetInputManager()->KeyboardDown(_key, x, y);
}

void KeyboardUp(unsigned char _key, int x, int y)
{
	globalGame->GetInputManager()->KeyboardUp(_key, x, y);
}

void MouseClick(int button, int state, int x, int y)
{
	globalGame->GetInputManager()->MouseClick(button, state, x, y);
	globalGame->SetScreenMouseX(x);
	globalGame->SetScreenMouseY(y);
}

void MousePassiveMove(int x, int y)
{
	globalGame->SetScreenMouseX(x);
	globalGame->SetScreenMouseY(y);
}

void MouseMove(int x, int y)
{
	globalGame->SetScreenMouseX(x);
	globalGame->SetScreenMouseY(y);
}

void MouseWheel(int button, int dir, int x, int y)
{
	globalGame->GetInputManager()->MouseWheel(button, dir, x, y);
}

void SpecialDown(int _key, int x, int y)
{
	globalGame->GetInputManager()->SpecialKeyDown(_key, x, y);
}

void SpecialUp(int _key, int x, int y)
{
	globalGame->GetInputManager()->SpecialKeyUp(_key, x, y);
}

void ShutDown()
{
	delete globalGame;
	globalGame = nullptr;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(20, 20);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("A* Pathfinder - Zheng Yu Bu");
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //clear
	
	//Init Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glewInit();
	init();

	// Register callbacks
	glutReshapeFunc(resize);

	// Input callbacks
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(SpecialDown);
	glutSpecialUpFunc(SpecialUp);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);
	glutMouseWheelFunc(MouseWheel);
	glutPassiveMotionFunc(MousePassiveMove);

	// Loop callbacks
	glutIdleFunc(update);
	glutDisplayFunc(render);
	glutCloseFunc(ShutDown);
	glutMainLoop();

	return 0;
}