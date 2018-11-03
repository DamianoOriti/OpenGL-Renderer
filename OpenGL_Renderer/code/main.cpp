#include <iostream>
#include "graphics/renderer.h"
#include "GL/glut.h"

const int windowedModeWidth(1280);
const int windowedModeHeight(720);
const bool runFullscreen(false);
int windowWidth;
int windowHeight;
int startTime(0);
float deltaTime(0.0f);
std::vector<Font*> pFonts;
std::vector<Text2D*> pText2Ds;
Text2D* pText2DWithGlow(nullptr);
bool shouldIncreseGlow(false);
std::vector<ShadedModel3D*> pShadedModel3Ds;
std::vector<Billboard2D*> pBillboard2Ds;
PointLight* pPointLight(nullptr);
DirectionalLight* pDirLight(nullptr);
Camera* pCamera(nullptr);
bool moveForward(false);
bool moveBackward(false);
bool moveLeft(false);
bool moveRight(false);
bool moveUp(false);
bool moveDown(false);
float angleX(0.0f);
float angleY(-180.0f);

void releaseData()
{
	if (pDirLight != nullptr)
	{
		delete pDirLight;
		pDirLight = nullptr;
	}

	if (pPointLight != nullptr)
	{
		delete pPointLight;
		pPointLight = nullptr;
	}

	for (size_t i = 0; i < pBillboard2Ds.size(); i++)
	{
		delete pBillboard2Ds[i];
	}
	pBillboard2Ds.clear();

	for (size_t i = 0; i < pFonts.size(); i++)
	{
		delete pFonts[i];
	}
	pFonts.clear();

	for (size_t i = 0; i < pText2Ds.size(); i++)
	{
		delete pText2Ds[i];
	}
	pText2Ds.clear();

	for (size_t i = 0; i < pShadedModel3Ds.size(); i++)
	{
		delete pShadedModel3Ds[i];
	}
	pShadedModel3Ds.clear();
}

void loadData()
{
	Scene& scene = Renderer::getInstance().scenes_[0];
	pCamera = &scene.camera_;
	pCamera->position_ = Vector3f(0.0f, 0.5f, 0.0f);


	// fonts
	Font* pFont = new Font("../OpenGL_Renderer/data/fonts/candara.fnt");
	pFonts.push_back(pFont);


	// texts
	Text2D* pText2D = new Text2D("Hello world!", Text2D::Alignment::RIGHT, pFont);
	pText2D->color_ = Color(1.0f, 1.0f, 0.0f);
	pText2D->scale_ = 1.6f;
	pText2D->position_ = Vector2f(0.9f, -0.8f);
	pText2D->effectColor_ = Color(1.0f, 0.0f, 0.0f);
	pText2D->effectType_ = Text2D::EffectType::OUTLINE;
	pText2D->effectScale_ = 0.5f;
	pText2Ds.push_back(pText2D);
	scene.add(pText2D);

	pText2D = new Text2D("Hello world!", Text2D::Alignment::RIGHT, pFont);
	pText2D->color_ = Color(1.0f, 1.0f, 0.0f);
	pText2D->scale_ = 1.6f;
	pText2D->position_ = Vector2f(0.9f, -0.6f);
	pText2D->effectColor_ = Color(0.0f, 1.0f, 0.0f);
	pText2D->effectType_ = Text2D::EffectType::GLOW;
	pText2Ds.push_back(pText2D);
	pText2DWithGlow = pText2D;
	scene.add(pText2D);

	pText2D = new Text2D("Hello world!", Text2D::Alignment::RIGHT, pFont);
	pText2D->color_ = Color(0.25f, 0.25f, 1.0f);
	pText2D->scale_ = 1.6f;
	pText2D->position_ = Vector2f(0.9f, -0.4f);
	pText2D->effectColor_ = Color(0.0f, 0.0f, 0.0f);
	pText2D->effectType_ = Text2D::EffectType::DROP_SHADOWS;
	pText2Ds.push_back(pText2D);
	scene.add(pText2D);

	pText2D = new Text2D("Move with 'W' 'A' 'S' 'D' 'R' 'F' and mouse", Text2D::Alignment::LEFT, pFont);
	pText2D->color_ = Color(1.0f, 1.0f, 1.0f);
	pText2D->scale_ = 0.9f;
	pText2D->position_ = Vector2f(-0.98f, 0.98f);
	pText2Ds.push_back(pText2D);
	scene.add(pText2D);


	// billboards
	Billboard2D* pBillboard2D = new Billboard2D("../OpenGL_Renderer/data/textures/Sneaking-white-dog.png", 0.2f);
	pBillboard2D->position_ = Vector2f(0.6f, 0.25f);
	pBillboard2Ds.push_back(pBillboard2D);
	scene.add(pBillboard2D);


	// lights
	GLfloat diffuse[] = { 0.5f,0.5f,0.5f };
	GLfloat specular[] = { 0.7f,0.7f,0.7f };
	GLfloat position[] = { 1.0f,1.0f,0.0f };
	pPointLight = new PointLight(diffuse, specular, position);
	scene.pPointLight_ = pPointLight;

	GLfloat ambient[] = { 0.1f,0.1f,0.1f };
	GLfloat direction[] = { 0.7071f,0.0f,-0.7071f };
	pDirLight = new DirectionalLight(ambient, diffuse, specular, direction);
	scene.pDirLight_ = pDirLight;


	// models
	ShadedModel3D* pShadedModel3D = new ShadedModel3D("../OpenGL_Renderer/data/models/sofa.fbx");
	pShadedModel3D->position_ = Vector3f(0.0f, 0.0f, 2.0f);
	pShadedModel3Ds.push_back(pShadedModel3D);
	scene.add(pShadedModel3D);
}

void idle()
{
	int timeFromGlutInit = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (float)(timeFromGlutInit - startTime) * 0.001f;
	startTime = timeFromGlutInit;

	glutWarpPointer(windowWidth / 2, windowHeight / 2);

	// update text with glow
	if (shouldIncreseGlow)
	{
		pText2DWithGlow->effectScale_ += 2.0f * deltaTime;

		if (pText2DWithGlow->effectScale_ >= 1.0f)
		{
			shouldIncreseGlow = false;
		}
	}
	else
	{
		pText2DWithGlow->effectScale_ -= 2.0f * deltaTime;

		if (pText2DWithGlow->effectScale_ <= 0.2f)
		{
			shouldIncreseGlow = true;
		}
	}


	// move camera
	if (moveForward)
	{
		pCamera->position_ += pCamera->direction_ * 1.5f * deltaTime;
	}
	if (moveBackward)
	{
		pCamera->position_ -= pCamera->direction_ * 1.5f * deltaTime;
	}
	if (moveLeft)
	{
		Vector3f rightDir = pCamera->direction_.cross(Vector3f(0.0f, 1.0f, 0.0f));
		pCamera->position_ -= rightDir * 1.5f * deltaTime;
	}
	if (moveRight)
	{
		Vector3f rightDir = pCamera->direction_.cross(Vector3f(0.0f, 1.0f, 0.0f));
		pCamera->position_ += rightDir * 1.5f * deltaTime;
	}
	if (moveUp)
	{
		Vector3f upDir(0.0f, 1.0f, 0.0f);
		pCamera->position_ += upDir * 1.5f * deltaTime;
	}
	if (moveDown)
	{
		Vector3f upDir(0.0f, 1.0f, 0.0f);
		pCamera->position_ -= upDir * 1.5f * deltaTime;
	}


	glutPostRedisplay();
}

void display()
{
	Renderer& renderer = Renderer::getInstance();
	renderer.renderScene(0);

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	Renderer::getInstance().createProjectionMatrices(width, height);

	glutWarpPointer(width / 2, height / 2);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		releaseData();
		exit(0);
	case 'w':
		moveForward = true;
		break;
	case 's':
		moveBackward = true;
		break;
	case 'a':
		moveLeft = true;
		break;
	case 'd':
		moveRight = true;
		break;
	case 'r':
		moveUp = true;
		break;
	case 'f':
		moveDown = true;
		break;
	default:
		break;
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		moveForward = false;
		break;
	case 's':
		moveBackward = false;
		break;
	case 'a':
		moveLeft = false;
		break;
	case 'd':
		moveRight = false;
		break;
	case 'r':
		moveUp = false;
		break;
	case 'f':
		moveDown = false;
		break;
	default:
		break;
	}
}

void mouseMove(int x, int y)
{
	Vector2f delta = Vector2f(x - windowWidth / 2, y - windowHeight / 2) * deltaTime * 16.0f;

	angleX -= delta.y;
	if (angleX > 90.0f)
	{
		angleX = 90.0f;
	}
	else if (angleX < -90.0f)
	{
		angleX = -90.0f;
	}

	pCamera->direction_.y = sin(angleX * 0.017444444f);

	angleY += delta.x;
	if (angleY > 360.0f)
	{
		angleY -= 360.0f;
	}
	else if (angleY < 0.0f)
	{
		angleY += 360.0f;
	}

	float c = cos(angleX * 0.017444444f);
	pCamera->direction_.x = sin(angleY * 0.017444444f) * c;
	pCamera->direction_.z = -cos(angleY * 0.017444444f) * c;
}

int main(int argc, char** argv)
{
	try
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA /*| GLUT_MULTISAMPLE*/);

		if (runFullscreen)
		{
			glutCreateWindow("OpenGL Renderer");
			glutFullScreen();
			windowWidth = glutGet(GLUT_WINDOW_WIDTH);
			windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
		}
		else
		{
			windowWidth = windowedModeWidth;
			windowHeight = windowedModeHeight;
			Vector2i windowPosition;
			windowPosition.x = (glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2;
			windowPosition.y = (glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 2;
			glutInitWindowPosition(windowPosition.x, windowPosition.y);
			glutInitWindowSize(windowWidth, windowHeight);
			glutCreateWindow("OpenGL Renderer");
		}

		glutKeyboardFunc(keyboard);
		glutKeyboardUpFunc(keyboardUp);
		glutMotionFunc(mouseMove);
		glutPassiveMotionFunc(mouseMove);
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutIdleFunc(idle);

		glutWarpPointer(windowWidth / 2, windowHeight / 2);
		glutSetCursor(GLUT_CURSOR_NONE);

		glewInit();

		loadData();

		startTime = glutGet(GLUT_ELAPSED_TIME);

		glutMainLoop();
	}
	catch (std::exception exception)
	{
		releaseData();

		std::cout << exception.what() << std::endl;

		exit(-1);
	}
}
