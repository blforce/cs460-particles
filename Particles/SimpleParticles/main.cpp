#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>

#define MAX_PARTICLES 100
#define MAX_PARTICLE_AGE 300
#define MAX_BOUNCE_COUNT 5

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


const char className[] = "OpenGL";


// Function Forward Declarations

BOOL				CreateOpenGLWindow(	char* title, int width, int height, int bits, bool fullscreenflag);
GLvoid				ReSizeGLScene(GLsizei width, GLsizei height);
GLvoid				KillGLWindow(GLvoid);
int					InitGL(GLvoid);
GLvoid				InitialiseObjects();
GLvoid				MainProgramLoop();
GLvoid				PrepareGLScene(GLvoid);
GLvoid				DrawGLScene(GLvoid);
GLvoid				PerformKeyboardFunctions(GLvoid);
bool				LoadBitmapTexture(char * FileName, GLuint &texid);

void InitParticles();
void ActivateParticles();
void AdjustParticles();
void RenderParticles();


// Global Variable Definitions

char * szAppTitle = "Richard's OpenGL Particles Tutorial";

HDC					hDC=NULL;
HGLRC				hRC=NULL;
HWND				hWnd=NULL;
HINSTANCE			hInstance;

bool				keys[256];
bool				active=TRUE;
bool				fullscreen=TRUE;

float Size = 0.2f;

float R = 0.8f;
float G = 0.2f;
float B = 0.0f;
float cR = 0.001f;
float cG = 0.002f;
float cB = 0.003f;

GLuint txParticle;
GLuint txPlane;


struct PARTICLE {

	float X,Y,Z;		// Current position
	float sX,sY,sZ;		// Current Speed/Movement
	float tX,tY,tZ;		// Target Speed/Movement
	float R,B,G;		// Particle Colour
	bool Active;		// Is particle Active
	int Age;			// Age of the particle
	int MaxAge;			// Maximum Age before particle dies
	int BounceCount;

} Particles[MAX_PARTICLES];



LRESULT CALLBACK WndProc(HWND hWnd,	UINT Message, WPARAM wParam, LPARAM lParam){

	switch (Message){
		case WM_ACTIVATE:							// Watch For Window Activate Message
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
			break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?

			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
			break;
		case WM_KEYUP:								// Has A Key Been Released?

			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
			break;
		case WM_SIZE:								// Resize The OpenGL Window
	
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
			break;
		case WM_CLOSE:

			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd,Message,wParam,lParam);
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	MSG		msg;
	bool	done = false;	

	CreateOpenGLWindow(szAppTitle, 800, 600, 16, false);



	while(!done){

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			if (msg.message==WM_QUIT){
				done=TRUE;
			} else {
				TranslateMessage(&msg);	
				DispatchMessage(&msg);	
			}
		} else {
			if(active){
				// Our Application Processing goes here.
				MainProgramLoop();
			}
		}
	}

	KillGLWindow();
	// Exit
	return ((int)msg.wParam);
}







BOOL CreateOpenGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style

	// Comment out this if statement if you would rather have a specific
	// fullscreen res.  This statement will check the screen res and use
	// that instead of the width and height specified.
	if(fullscreenflag){
		width = GetDeviceCaps(GetDC(NULL), HORZRES);	// GetDC(NULL) returns a
		height = GetDeviceCaps(GetDC(NULL), VERTRES);	// HDC for the whole screen
	}


	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= className;							// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								className,							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}





GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}






GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}







int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	InitialiseObjects();

	return TRUE;										// Initialization Went OK
}


GLvoid InitialiseObjects(){


	InitParticles();



}




GLvoid MainProgramLoop(){

	PrepareGLScene();

	PerformKeyboardFunctions();

	DrawGLScene();

}




GLvoid DrawGLScene(GLvoid){

	// First we clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Then we reset the modelview matrix
	glLoadIdentity();


	// GL Drawing Code Goes Here!!!!

	glTranslatef(0.0f, -1.0f, -10.0f);

	glPushMatrix();
		glScalef(1.0f, -1.0f, 1.0f);
		RenderParticles();
	glPopMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

	glColor4f(0.0f, 0.2f, 0.2f, 0.5f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txPlane);

	glBegin(GL_QUADS);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(-10.0f, 0.0f, 10.0f);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f( 10.0f, 0.0f, 10.0f);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f( 10.0f, 0.0f, -10.0f);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(-10.0f, 0.0f, -10.0f);

	glEnd();



	RenderParticles();


	SwapBuffers(hDC);

}




GLvoid PrepareGLScene(GLvoid){


	ActivateParticles();

	AdjustParticles();



}

GLvoid PerformKeyboardFunctions(){

	if (keys[VK_ESCAPE]){	// Exit if ESC is pressed
		PostQuitMessage(0);
	}
	if (keys[VK_F1]){		// Switch between full/windowed on F1
		keys[VK_F1]=FALSE;
		KillGLWindow();
		fullscreen=!fullscreen;
		if (!CreateOpenGLWindow(szAppTitle,800,600,16,fullscreen)){
			fullscreen=!fullscreen; //Switch back and try again
			CreateOpenGLWindow(szAppTitle,800,600,16,fullscreen);
			MessageBox(hWnd,"Cannot Switch between full/windowed mode","ERROR",MB_OK | MB_ICONINFORMATION);
		}
	}



}




void InitParticles(){

	int p;

	srand((int)time(NULL));

	for(p=0; p<MAX_PARTICLES; p++){

		Particles[p].Active = false;
		Particles[p].tX = 0.0f;
		Particles[p].tY = -0.1f;
		Particles[p].tZ = 0.0f;

	}

	LoadBitmapTexture("./Particle.bmp", txParticle);
	LoadBitmapTexture("./Plane.bmp", txPlane);

}

void ActivateParticles(){

	int p;

	for(p=0; p<MAX_PARTICLES; p++){

		if(!Particles[p].Active){
			// Start the particle at 0,0,0 origin
			Particles[p].X = 0.0f;
			Particles[p].Y = 0.0f;
			Particles[p].Z = 0.0f;

			// The following lines set a random speed value
			Particles[p].sX = (((float)((rand() % 100) + 1)) / 1000.0f) - 0.05f;
			Particles[p].sY = (((float)((rand() % 100) + 50)) / 500.0f);
			Particles[p].sZ = (((float)((rand() % 100) + 1)) / 1000.0f) - 0.05f;

			// We also activate the particle
			Particles[p].Active = true;
			// Set it's Age to zero
			Particles[p].Age = 0;
			// We also assign a max age to the particles
			Particles[p].MaxAge = MAX_PARTICLE_AGE;
			// We Also reset the bouncecount to zero
			Particles[p].BounceCount = 0;

			Particles[p].R = R;
			Particles[p].G = G;
			Particles[p].B = B;

			R+=cR;
			G+=cG;
			B+=cB;

			if(R>1.0f){R=1.0f; cR=-cR;}if(R<0.0f){R=0.0f; cR=-cR;}
			if(G>1.0f){G=1.0f; cG=-cG;}if(G<0.0f){G=0.0f; cG=-cG;}
			if(B>1.0f){B=1.0f; cB=-cB;}if(B<0.0f){B=0.0f; cB=-cB;}

			return;
		}
	}
}

void AdjustParticles(){

	int p;

	for(p=0; p<MAX_PARTICLES; p++){

		// We move the speed towards the target speed by 1/20 (5%)
		Particles[p].sX+= (Particles[p].tX - Particles[p].sX) / 20.0f;
		Particles[p].sY+= (Particles[p].tY - Particles[p].sY) / 20.0f;
		Particles[p].sZ+= (Particles[p].tZ - Particles[p].sZ) / 20.0f;
		// Then we adjust the position of the particle by the new speed
		Particles[p].X+= Particles[p].sX;
		Particles[p].Y+= Particles[p].sY;
		Particles[p].Z+= Particles[p].sZ;

		// Now for the bounce code.
		if(Particles[p].Y < 0.0f){
			Particles[p].Y = 0.0f;
			Particles[p].sY = -Particles[p].sY;
			Particles[p].BounceCount++;
			if(Particles[p].BounceCount > MAX_BOUNCE_COUNT){
				Particles[p].Active = false;
			}

		}

		// And finally the age check
		Particles[p].Age++;
		if(Particles[p].Age > Particles[p].MaxAge){
			Particles[p].Active = false;
		}
	}
}


void RenderParticles(){

	int p;

	// Enable textures and bind our particle texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txParticle);

	// Disable Depth testing.
	glDisable(GL_DEPTH_TEST);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR,GL_ONE);

	for(p=0; p<MAX_PARTICLES; p++){

		if(Particles[p].Active){
			glColor4f(Particles[p].R, Particles[p].G, Particles[p].B, 1.0f);

			glPushMatrix();

				glTranslatef(Particles[p].X, Particles[p].Y, Particles[p].Z);
	//			glRotatef(-Rotation, 0.0f, 1.0f, 0.0f);

				glNormal3f(0.0f, 0.0f, 1.0f);

				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);		glVertex3f(-Size, -Size, 0.0f);
					glTexCoord2f(1.0f, 0.0f);		glVertex3f(Size, -Size, 0.0f);
					glTexCoord2f(1.0f, 1.0f);		glVertex3f(Size, Size, 0.0f);
					glTexCoord2f(0.0f, 1.0f);		glVertex3f(-Size, Size, 0.0f);
				glEnd();

			glPopMatrix();
		}
	}

	glEnable(GL_DEPTH_TEST);
}



bool LoadBitmapTexture(char * FileName, GLuint &texid)					// Creates Texture From A Bitmap File
{
	HBITMAP hBMP;														// Handle Of The Bitmap
	BITMAP	BMP;														// Bitmap Structure

	glGenTextures(1, &texid);   // Create The Texture


	hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL), FileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );

	if (!hBMP)															// Does The Bitmap Exist?
		return FALSE;													// If Not Return False

	GetObject(hBMP, sizeof(BMP), &BMP);									// Get The Object
																		// hBMP:        Handle To Graphics Object
																		// sizeof(BMP): Size Of Buffer For Object Information
																		// &BMP:        Buffer For Object Information

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);								// Pixel Storage Mode (Word Alignment / 4 Bytes)

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texid);								// Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);													// Delete The Object

	return TRUE;														// Loading Was Successful
}
