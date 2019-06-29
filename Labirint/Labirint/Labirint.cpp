// Labirint.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Labirint.h"
#include <windows.h>        									
#include <glut.h>          
#include <glaux.h>          
#include <SOIL.h>
#include <math.h>
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib,"SOIL.lib")
#define CDS_FULLSCREEN 4

HGLRC  hRC = NULL;              // Постоянный контекст рендеринга
HDC  hDC = NULL;              // Приватный контекст устройства GDI
HWND  hWnd = NULL;              // Здесь будет хранится дескриптор окна
HINSTANCE  hInstance;              // Здесь будет хранится дескриптор приложения

bool  keys[256];                // Массив, используемый для операций с клавиатурой

BOOL light, camera = false;      // Свет ВКЛ / ВЫКЛ
BOOL lp, gp, mp;        
GLfloat rotx = 0, roty = 0, posx = 0, posz = 0;
GLfloat crotx = 0, croty = 0, cposx = 0, cposz = 0;
GLuint  texture[9];
GLuint kust;     // Память для текстур
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Значения фонового света 
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения диффузного света 
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };     // Позиция света

ISoundEngine  *SoundMusic = createIrrKlangDevice();
bool  active = true;                // Флаг активности окна, установленный в true по умолчанию
bool  fullscreen = true;              // Флаг режима окна, установленный в полноэкранный по умолчанию

LRESULT  CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // Прототип функции WndProc

GLvoid BuildLists()     //создаем список отображения кустов
{
	kust = glGenLists(1);
	glNewList(kust, GL_COMPILE);      // Новый откомпилированный список отображения box
	glBegin(GL_QUADS);      // Начинаем рисование четырехугольников (quads)
							// Нижняя поверхность
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.1f, -0.1f, -0.1f);        // Верхний правый угол текстуры и четырехугольник
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.1f, -0.1f, -0.1f);        // Верхний левый угол текстуры и четырехугольник
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.1f);        // Нижний левый угол текстуры и четырехугольник
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.1f);        // Нижний правый угол текстуры и четырехугольник
										   // Передняя поверхность
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.1f);        // Нижний левый угол текстуры и четырехугольник
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.1f);        // Нижний правый угол текстуры и четырехугольник
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.1f, 0.1f, 0.1f);        // Верхний правый угол текстуры и четырехугольник
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.1f, 0.1f, 0.1f);        // Верхний левый угол текстуры и четырехугольник
										  // Задняя поверхность
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, -0.1f, -0.1f);        // Нижний правый угол текстуры и четырехугольник
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.1f, 0.1f, -0.1f);        // Верхний правый угол текстуры и четырехугольник
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.1f, 0.1f, -0.1f);        // Верхний левый угол текстуры и четырехугольник
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.1f, -0.1f, -0.1f);        // Нижний левый угол текстуры и четырехугольник
										   // Правая поверхность
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, -0.1f, -0.1f);        // Нижний правый угол текстуры и четырехугольник
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.1f, 0.1f, -0.1f);        // Верхний правый угол текстуры и четырехугольник
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.1f, 0.1f, 0.1f);        // Верхний левый угол текстуры и четырехугольник
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.1f);        // Нижний левый угол текстуры и четырехугольник
										  // Левая поверхность
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.1f, -0.1f, -0.1f);        // Нижний левый угол текстуры и четырехугольник
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.1f);        // Нижний правый угол текстуры и четырехугольник
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.1f, 0.1f, 0.1f);        // Верхний правый угол текстуры и четырехугольник
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.1f, 0.1f, -0.1f);        // Верхний левый угол текстуры и четырехугольник
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.1f, 0.1f, -0.1f);        // Верхний левый угол текстуры и четырехугольник
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.1f, 0.1f, 0.1f);        // Нижний левый угол текстуры и четырехугольник
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.1f, 0.1f, 0.1f);        // Нижний правый угол текстуры и четырехугольник
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.1f, 0.1f, -0.1f);        // Верхний правый угол текстуры и четырехугольник
	glEnd();                // Заканчиваем рисование четырехугольника
	glEndList();            // Закончили создание списка отображения 'top'
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)        // Изменить размер и инициализировать окно GL
{
	if (height == 0)              // Предотвращение деления на ноль
	{
		height = 1;
	}
	glViewport(0, 0, width, height);          // Сброс текущей области вывода
	glMatrixMode(GL_PROJECTION);            // Выбор матрицы проекций
	glLoadIdentity();              // Сброс матрицы проекции
								   // Вычисление соотношения геометрических размеров для окна
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);            // Выбор матрицы вида модели
	glLoadIdentity();              // Сброс матрицы вида модели
}

GLvoid LoadGLTextures()
{
	// Загрузка картинки
	int twidth, theight, tw,th;
	unsigned char* text[9]; 
	text[0] = SOIL_load_image("negx.jpg", &twidth, &theight, 0, SOIL_LOAD_RGB);
	text[1] = SOIL_load_image("posx.jpg", &twidth, &theight, 0, SOIL_LOAD_RGB);
	text[2] = SOIL_load_image("negy.jpg", &twidth, &theight, 0, SOIL_LOAD_RGB);
	text[3] = SOIL_load_image("posy.jpg", &twidth, &theight, 0, SOIL_LOAD_RGB);
	text[4] = SOIL_load_image("negz.jpg", &twidth, &theight, 0, SOIL_LOAD_RGB);
	text[5] = SOIL_load_image("posz.jpg", &twidth, &theight, 0, SOIL_LOAD_RGB);
	text[6] = SOIL_load_image("kust4.jpg", &tw, &th, 0, SOIL_LOAD_RGB); //куст
	text[7] = SOIL_load_image("rock.jpg", &tw, &th, 0, SOIL_LOAD_RGB);
	text[8] = SOIL_load_image("tea.jpg", &tw, &th, 0, SOIL_LOAD_RGB);
	// Создание текстуры
	glGenTextures(9, &texture[0]);
	for (int i = 0; i < 9; i++) 
	{
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (i ==6 || i == 7 || i ==8)
			glTexImage2D(GL_TEXTURE_2D, 0, 3, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE, text[i]);
		else glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE,text[i]);
	}
	
}

GLint InitGL(GLsizei Width, GLsizei Height)
{
	LoadGLTextures();			// Загрузка текстур
	BuildLists(); // для кустов
	//SoundMusic->play2D("crazy.mp3", GL_TRUE);
	glEnable(GL_TEXTURE_2D);      // Разрешить наложение текстуры
	glShadeModel(GL_SMOOTH);      // Разрешение сглаженного закрашивания
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Черный фон
	glClearDepth(1.0f);           // Установка буфера глубины
	glEnable(GL_DEPTH_TEST);      // Разрешить тест глубины
	glDepthFunc(GL_LEQUAL);       // Тип теста глубины

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // Установка Фонового Света
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);    // Установка Диффузного Света
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   // Позиция света
	glEnable(GL_LIGHT0); // Быстрое простое освещение
						 // (устанавливает в качестве источника освещения Light0)
	glEnable(GL_LIGHTING);  // Включает освещение
	glEnable(GL_COLOR_MATERIAL);    // Включает раскрашивание материала
	glEnable(GL_LIGHT1); // Разрешение источника света номер один
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Улучшенные вычисления перспективы
	return 1;
}

GLvoid DrawGLScene(GLvoid)                // Здесь прорисовка
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//glLoadIdentity();
	glTranslated(cposx, 0, cposz);
	glTranslated(-0.25, 0, -1);
	glRotatef(180, 1.0f, 0.0f, 0.0f);
	glRotatef(65, 1.0f, 0.0f, 0.0f);
	glRotatef(crotx, 1.0f, 0.0f, 0.0f);
	glRotatef(croty, 0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	// Передняя грань
	glNormal3f(0.0f, 0.0f, 1.0f);     // Нормаль указывает на наблюдателя
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Точка 1 (Перед)
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Точка 2 (Перед)
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Точка 3 (Перед)
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Точка 4 (Перед)
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[4]);											 // Задняя грань
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);     // Нормаль указывает от наблюдателя
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Точка 1 (Зад)
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Точка 2 (Зад)
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Точка 3 (Зад)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Точка 4 (Зад)
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[2]);											  // Верхняя грань
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);     // Нормаль указывает вверх
	glTexCoord2f(0.0f, 0.0f);   glVertex3f(-1.0f, 1.0f, 1.0f); // Точка 2 (Верх)
	glTexCoord2f(1.0f, 0.0f); 	glVertex3f(1.0f, 1.0f, 1.0f); // Точка 3 (Верх)
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Точка 4 (Верх)
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-1.0f, 1.0f, -1.0f); // Точка 1 (Верх)
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);// Нижняя грань
	glNormal3f(0.0f, -1.0f, 0.0f);     // Нормаль указывает вниз
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Точка 3 (Низ)
	glTexCoord2f(0.0f, 1.0f);		glVertex3f(-1.0f, -1.0f, 1.0f); // Точка 4 (Низ)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Точка 1 (Низ)
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(1.0f, -1.0f, -1.0f); // Точка 2 (Низ)
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[1]);												  // Правая грань
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);     // Нормаль указывает вправо
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f); // Точка 1 (Право)
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f); // Точка 2 (Право)
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f); // Точка 3 (Право)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f); // Точка 4 (Право)
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);// Левая грань
	glNormal3f(-1.0f, 0.0f, 0.0f);     // Нормаль указывает влево
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Точка 1 (Лево)
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); // Точка 2 (Лево)
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f); // Точка 3 (Лево)
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f); // Точка 4 (Лево)
	glEnd();        // Кончили рисовать четырехугольник
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	glScaled(0.3, 0.3, 0.3);
	glTranslated(1.5, 0.75, -0.75);
	for (int i = 0; i < 58; i++)
	{
		if (i == 51) { glTranslated(0, 0, 0.2); continue; }
		else {
			if (i == 17 || i == 30 || i == 46)
			{
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			}
			glTranslated(0, 0, 0.2);
			glCallList(kust);
		}
	}
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glTranslated(0, 0, 0.4);
	for (int i = 0; i < 95; i++)
	{
		if (i == 1) { glTranslated(0, 0, 1.6); continue;}
		if (i == 5) { glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); glTranslated(0, 0, 0.2); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);  glTranslated(0, 0, 0.6); continue; }
		if (i == 10 || i == 17 || i == 75) { glTranslated(0, 0, 0.2); continue;}
		if (i == 15) { glRotatef(90.0f, 0.0f, 1.0f, 0.0f); glTranslated(0, 0, 0.2); glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  glTranslated(0, 0, 1.4); continue; }
		if (i == 21 || i == 24 || i == 31 || i == 42 || i == 51 || i == 56 || i == 58 || i == 65 || i == 71 || i == 73 || i == 77 || i == 85 || i == 79 || i == 89) { glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); }
		if (i == 26 || i == 29 || i == 37 || i == 41 || i == 43 || i == 47 || i == 50 || i == 54 || i == 61 || i == 63 || i == 69 || i == 72 || i == 90 || i == 93) { glRotatef(90.0f, 0.0f, 1.0f, 0.0f); }
		if (i == 27 || i == 32 || i == 39 || i == 57 || i == 81) { glRotatef(180.0f, 0.0f, 1.0f, 0.0f); }
		if (i == 44) { glTranslated(0, 0, 0.2); glRotatef(90.0f, 0.0f, 1.0f, 0.0f); glTranslated(0, 0, 0.2); continue;}
		if (i == 48) { glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); glTranslated(0, 0, 0.2); continue;}
		if (i == 52) { glRotatef(90.0f, 0.0f, 1.0f, 0.0f); glTranslated(0, 0, 0.2); continue;}
		if (i == 55) { glTranslated(0, 0, 1); }
		if (i == 67) { glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); glTranslated(0, 0, 0.4); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); continue; }
		if (i == 91) { glRotatef(180.0f, 0.0f, 1.0f, 0.0f); glTranslated(0, 0, 1.4); glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); continue; }
		if (i == 94) { glRotatef(90.0f, 0.0f, 1.0f, 0.0f); glTranslated(0, 0, 1.4); }
			glTranslated(0, 0, 0.2);
			glCallList(kust);
	}
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 
	glTranslated(0, 0, 1);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslated(0, 0, 1.2);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glCallList(kust);
	glRotatef(180, 1.0f, 0.0f, 0.0f);
	glTranslated(0, 0.15, 0);

	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glRotated(rotx, 1.0, 0.0, 0.0);
	glRotated(roty, 0.0, 1.0, 0.0);
	glTranslated(posx, 0, posz);

	if (gp) {
		glLoadIdentity();
		for (int i = 0; i < 20; i++) {
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glRotated((double)0.03 + i*0.05, 0.0, 1.0, 0.0);
			glTranslated(0.0, 0.0001, 0.0);
			glutSolidTeapot(0.075);
		}
	}
	else { glutSolidTeapot(0.075); }

}

GLvoid KillGLWindow(GLvoid)              // Корректное разрушение окна
{
	if (fullscreen)              // Мы в полноэкранном режиме?
	{
		ChangeDisplaySettings(NULL, 0);        // Если да, то переключаемся обратно в оконный режим
		ShowCursor(true);            // Показать курсор мышки
	}
	if (hRC)                // Существует ли Контекст Рендеринга?
	{
		if (!wglMakeCurrent(NULL, NULL))        // Возможно ли освободить RC и DC?
		{
			MessageBox(NULL, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC))        // Возможно ли удалить RC?
		{
			MessageBox(NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;              // Установить RC в NULL
	}
	if (hDC && !ReleaseDC(hWnd, hDC))          // Возможно ли уничтожить DC?
	{
		MessageBox(NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;                // Установить DC в NULL
	}
	if (hWnd && !DestroyWindow(hWnd))            // Возможно ли уничтожить окно?
	{
		MessageBox(NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;                // Установить hWnd в NULL
	}
	if (!UnregisterClass(L"OpenGL", hInstance))        // Возможно ли разрегистрировать класс
	{
		MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;                // Установить hInstance в NULL
	}
}
BOOL CreateGLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint    PixelFormat;              // Хранит результат после поиска
	WNDCLASS  wc;                // Структура класса окна
	DWORD    dwExStyle;              // Расширенный стиль окна
	DWORD    dwStyle;              // Обычный стиль окна
	RECT WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;              // Установить левую составляющую в 0
	WindowRect.right = (long)width;              // Установить правую составляющую в Width
	WindowRect.top = (long)0;                // Установить верхнюю составляющую в 0
	WindowRect.bottom = (long)height;              // Установить нижнюю составляющую в Height
	fullscreen = fullscreenflag;              // Устанавливаем значение глобальной переменной fullscreen
	hInstance = GetModuleHandle(NULL);        // Считаем дескриптор нашего приложения
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // Перерисуем при перемещении и создаём скрытый DC
	wc.lpfnWndProc = (WNDPROC)WndProc;          // Процедура обработки сообщений
	wc.cbClsExtra = 0;              // Нет дополнительной информации для окна
	wc.cbWndExtra = 0;              // Нет дополнительной информации для окна
	wc.hInstance = hInstance;            // Устанавливаем дескриптор
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);        // Загружаем иконку по умолчанию
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);        // Загружаем указатель мышки
	wc.hbrBackground = NULL;              // Фон не требуется для GL
	wc.lpszMenuName = NULL;              // Меню в окне не будет
	wc.lpszClassName = L"OpenGL";            // Устанавливаем имя классу
	if (!RegisterClass(&wc))              // Пытаемся зарегистрировать класс окна
	{
		MessageBox(NULL, L"Failed To Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;                // Выход и возвращение функцией значения false
	}
	if (fullscreen)                // Полноэкранный режим?
	{
		DEVMODE dmScreenSettings;            // Режим устройства
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));    // Очистка для хранения установок
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);      // Размер структуры Devmode
		dmScreenSettings.dmPelsWidth = width;        // Ширина экрана
		dmScreenSettings.dmPelsHeight = height;        // Высота экрана
		dmScreenSettings.dmBitsPerPel = bits;        // Глубина цвета
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;// Режим Пикселя
																				 // Пытаемся установить выбранный режим и получить результат.  Примечание: CDS_FULLSCREEN убирает панель управления.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// Если переключение в полноэкранный режим невозможно, будет предложено два варианта: оконный режим или выход.

			if (MessageBox(NULL, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",

				L"NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)

			{
				fullscreen = false;          // Выбор оконного режима (fullscreen = false)
			}
			else
			{
				// Выскакивающее окно, сообщающее пользователю о закрытие окна.
				MessageBox(NULL, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP);
				return false;            // Выход и возвращение функцией false
			}
		}
	}
	if (fullscreen)                  // Мы остались в полноэкранном режиме?
	{
		dwExStyle = WS_EX_APPWINDOW;          // Расширенный стиль окна
		dwStyle = WS_POPUP;            // Обычный стиль окна
		ShowCursor(false);              // Скрыть указатель мышки
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // Расширенный стиль окна
		dwStyle = WS_OVERLAPPEDWINDOW;        // Обычный стиль окна
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);      // Подбирает окну подходящие размеры

	if (!(hWnd = CreateWindowEx(dwExStyle,          // Расширенный стиль для окна
		_T("OpenGL"),          // Имя класса
		title,            // Заголовок окна
		WS_CLIPSIBLINGS |        // Требуемый стиль для окна
		WS_CLIPCHILDREN |        // Требуемый стиль для окна
		dwStyle,          // Выбираемые стили для окна
		0, 0,            // Позиция окна
		WindowRect.right - WindowRect.left,    // Вычисление подходящей ширины
		WindowRect.bottom - WindowRect.top,    // Вычисление подходящей высоты
		NULL,            // Нет родительского
		NULL,            // Нет меню
		hInstance,          // Дескриптор приложения
		NULL)))          // Не передаём ничего до WM_CREATE (???)
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}
	static  PIXELFORMATDESCRIPTOR pfd =            // pfd сообщает Windows каким будет вывод на экран каждого пикселя
	{
		sizeof(PIXELFORMATDESCRIPTOR),            // Размер дескриптора данного формата пикселей
		1,                  // Номер версии
		PFD_DRAW_TO_WINDOW |              // Формат для Окна
		PFD_SUPPORT_OPENGL |              // Формат для OpenGL
		PFD_DOUBLEBUFFER,              // Формат для двойного буфера
		PFD_TYPE_RGBA,                // Требуется RGBA формат
		bits,                  // Выбирается бит глубины цвета
		0, 0, 0, 0, 0, 0,              // Игнорирование цветовых битов
		0,                  // Нет буфера прозрачности
		0,                  // Сдвиговый бит игнорируется
		0,                  // Нет буфера накопления
		0, 0, 0, 0,                // Биты накопления игнорируются
		32,                  // 32 битный Z-буфер (буфер глубины)
		0,                  // Нет буфера трафарета
		0,                  // Нет вспомогательных буферов
		PFD_MAIN_PLANE,                // Главный слой рисования
		0,                  // Зарезервировано
		0, 0, 0                  // Маски слоя игнорируются
	};
	if (!(hDC = GetDC(hWnd)))              // Можем ли мы получить Контекст Устройства?
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))        // Найден ли подходящий формат пикселя?
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))          // Возможно ли установить Формат Пикселя?
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}
	if (!(hRC = wglCreateContext(hDC)))          // Возможно ли установить Контекст Рендеринга?
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}
	if (!wglMakeCurrent(hDC, hRC))            // Попробовать активировать Контекст Рендеринга
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}
	ShowWindow(hWnd, SW_SHOW);              // Показать окно
	SetForegroundWindow(hWnd);              // Слегка повысим приоритет
	SetFocus(hWnd);                // Установить фокус клавиатуры на наше окно
	ReSizeGLScene(width, height);              // Настроим перспективу для нашего OpenGL экрана.
	if (!InitGL(width, height))                  // Инициализация только что созданного окна
	{
		KillGLWindow();                // Восстановить экран
		MessageBox(NULL, _T("Initialization Failed."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return false;                // Вернуть false
	}
	return true;                  // Всё в порядке!
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam)            // Дополнительная информация
{
	switch (uMsg)                // Проверка сообщения для окна
	{
	case WM_ACTIVATE:            // Проверка сообщения активности окна
	{
		if (!HIWORD(wParam))          // Проверить состояние минимизации
		{
			active = true;          // Программа активна
		}
		else
		{
			active = false;          // Программа теперь не активна
		}
		return 0;            // Возвращаемся в цикл обработки сообщений
	}
	case WM_SYSCOMMAND:            // Перехватываем системную команду
	{
		switch (wParam)            // Останавливаем системный вызов
		{
		case SC_SCREENSAVE:        // Пытается ли запустится скринсейвер?
		case SC_MONITORPOWER:        // Пытается ли монитор перейти в режим сбережения энергии?
			return 0;          // Предотвращаем это
		}
		break;              // Выход
	}
	case WM_CLOSE:              // Мы получили сообщение о закрытие?
	{
		PostQuitMessage(0);          // Отправить сообщение о выходе
		return 0;            // Вернуться назад
	}
	case WM_KEYDOWN:            // Была ли нажата кнопка?
	{
		keys[wParam] = true;          // Если так, мы присваиваем этой ячейке true
		return 0;            // Возвращаемся
	}
	case WM_KEYUP:              // Была ли отпущена клавиша?
	{
		keys[wParam] = false;          //  Если так, мы присваиваем этой ячейке false
		return 0;            // Возвращаемся
	}
	case WM_SIZE:              // Изменены размеры OpenGL окна
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // Младшее слово=Width, старшее слово=Height
		return 0;            // Возвращаемся
	}
	}
	// пересылаем все необработанные сообщения DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)       
{
	MSG  msg;              // Структура для хранения сообщения Windows
	BOOL  done = false;            // Логическая переменная для выхода из цикла
								   // Спрашивает пользователя, какой режим экрана он предпочитает
	if (MessageBox(NULL, L"Do you want to set fullscreen mode?", L"Full?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		fullscreen = false;          // Оконный режим
	}
	// Создать наше OpenGL окно
	if (!CreateGLWindow(L"OpenGL", 1920, 1080, 32, fullscreen))
	{
		return 0;              // Выйти, если окно не может быть создано
	}
	while (!done)                // Цикл продолжается, пока done не равно true
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))    // Есть ли в очереди какое-нибудь сообщение?
		{
			if (msg.message == WM_QUIT)        // Мы получили сообщение о выходе?
			{
				done = true;          // Если так, done=true
			}
			else              // Если нет, обрабатывает сообщения
			{
				TranslateMessage(&msg);        // Переводим сообщение
				DispatchMessage(&msg);        // Отсылаем сообщение
			}
		}
		else                // Если нет сообщений
		{
			// Прорисовываем сцену.
			if (active)          // Активна ли программа?
			{
				if (keys[VK_ESCAPE])        // Было ли нажата клавиша ESC?
				{
					done = true;      // ESC говорит об останове выполнения программы
				}
				else            // Не время для выхода, обновим экран.
				{
					DrawGLScene();        // Рисуем сцену
					SwapBuffers(hDC);    // Меняем буфер (двойная буферизация)

					if (keys['L'] && !lp) // Клавиша 'L' нажата и не удерживается?
					{
						lp = TRUE;      // lp присвоили TRUE
						light = !light; // Переключение света TRUE/FALSE
					}
					if (!light)               // Если не свет
					{
						glDisable(GL_LIGHTING);  // Запрет освещения
					}
					else                      // В противном случае
					{
						glEnable(GL_LIGHTING);   // Разрешить освещение
					}
					if (!keys['L']) // Клавиша 'L' Отжата?
					{
						lp = FALSE;      // Если так, то lp равно FALSE
					}
					if (keys['C']) 
					{
						camera = !camera; 
					}	
					if (keys['G'] )
					{
						gp = true;
					}
					else {
						gp = false;
					}
					if (keys['M'] && !mp )
					{
						mp = true;
						SoundMusic->play2D("crazy.mp3", GL_TRUE);
					}
					if (!keys['M']) 
					{
						mp = FALSE;      
						SoundMusic->removeAllSoundSources();
					}
					if (!camera) {
						if (keys[VK_UP])     // Клавиша стрелка вверх нажата?
						{
							rotx -= 0.5f;
						}
						if (keys[VK_DOWN])   // Клавиша стрелка вниз нажата?
						{
							rotx += 0.5f;
						}
						if (keys[VK_RIGHT])  // Клавиша стрелка вправо нажата?
						{
							roty -= 0.5f;
						}
						if (keys[VK_LEFT])   // Клавиша стрелка влево нажата?
						{
							roty += 0.5f;
						}
						if (keys['R'])
						{
							roty = 0; rotx = 0; posz = 0; posx = 0;
						}
						if (keys['A'])
						{
							posz += 0.01f;
						}
						if (keys['D'])
						{
							posz -= 0.01f;
						}
						if (keys['S'])
						{
							posx += 0.01f;
						}
						if (keys['W'])
						{
							posx -= 0.01f;
						}
					}
					if (camera) {
						if (keys[VK_UP])     // Клавиша стрелка вверх нажата?
						{
							crotx -= 0.5f;
						}
						if (keys[VK_DOWN])   // Клавиша стрелка вниз нажата?
						{
							crotx += 0.5f;
						}
						if (keys[VK_RIGHT])  // Клавиша стрелка вправо нажата?
						{
							croty -= 0.5f;
						}
						if (keys[VK_LEFT])   // Клавиша стрелка влево нажата?
						{
							croty += 0.5f;
						}
						if (keys['R'])
						{
							croty = 0; crotx = 0; cposz = 0; cposx = 0;
						}
						if (keys['W'])
						{
							cposz += 0.01f;
						}
						if (keys['S'])
						{
							cposz -= 0.01f;
						}
						if (keys['A'])
						{
							cposx += 0.01f;
						}
						if (keys['D'])
						{
							cposx -= 0.01f;
						}
					}
					if (keys[VK_F1])          // Была ли нажата F1?
					{
						keys[VK_F1] = false;        // Если так, меняем значение ячейки массива на false
						KillGLWindow();          // Разрушаем текущее окно
						fullscreen = !fullscreen;      // Переключаем режим
													   // Пересоздаём наше OpenGL окно
						if (!CreateGLWindow(_T("OpenGL struct"), 1920, 1080, 32, fullscreen))
						{
							return 0;        // Выходим, если это невозможно
						}
					}
				}
			}
		}
	}
	// Shutdown
	KillGLWindow();                // Разрушаем окно
	return (msg.wParam);              // Выходим из программы
}