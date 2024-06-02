#include "Log.h"
#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"

#include "Video.h"

Video::Video(bool startEnabled) : Module(startEnabled)
{
	name.Create("video");
}

Video::~Video()
{
}

bool Video::Awake(pugi::xml_node&)
{
	return true;
}

bool Video::CleanUp()
{
	return true;
}

void Video::Initialize(char* path)
{
	
	OpenAVI(path);                  // Open The AVI File
}

void Video::OpenAVI(LPCSTR path)
{
	AVIFileInit(); // Inicializa la biblioteca AVIFile

	if (AVIStreamOpenFromFile(&pavi, path, streamtypeVIDEO, 0, OF_READ, NULL) != 0) // Abre el flujo de video AVI
	{
		LOG("Failed To Open The AVI Stream");
		return;
	}

	AVIStreamInfo(pavi, &psi, sizeof(psi)); // Lee la información del flujo en psi
	width = psi.rcFrame.right - psi.rcFrame.left; // Ancho del fotograma
	height = psi.rcFrame.bottom - psi.rcFrame.top; // Altura del fotograma
	lastFrame = AVIStreamLength(pavi); // Último fotograma del flujo

	// Intenta abrir el frame del video
	pgf = AVIStreamGetFrameOpen(pavi, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT); // Crear PGETFRAME usando nuestro modo de solicitud
	if (pgf == NULL)
	{
		LOG("Failed To Open The AVI Frame");
		return;
	}

	isVideoFinished = false;
}

bool Video::GrabAVIFrame()
{
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame); // Obtiene datos del flujo AVI

	if (lpbi == NULL) // Verifica si lpbi es nulo
	{
		LOG("Failed To Get The AVI Frame");
		return false;
	}

	pdata = (char*)lpbi + lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD); // Asigna puntero a los datos devueltos por AVIStreamGetFrame
	if (pdata == NULL) // Verifica si pdata es nulo
	{
		LOG("Failed To Get The AVI Frame Data");
		return false;
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pdata, width, height, lpbi->biBitCount, width * 3, 0, 0, 0, 0);
	if (surface == NULL)
	{
		LOG("Failed To Create Surface From Frame Data");
		return false;
	}

	SDL_Texture* texture = app->tex->LoadSurface(surface);
	if (texture == NULL)
	{
		LOG("Failed To Create Texture From Surface");
		SDL_FreeSurface(surface);
		return false;
	}

	app->render->DrawTexture(texture, 0, 0, NULL, SDL_FLIP_VERTICAL);

	if (i % 2 == 0)
	{
		frame++;
	}
	i++;
	if (frame >= lastFrame)
	{
		frame = 0;
		isVideoFinished = true;
	}

	app->tex->UnLoad(texture);
	SDL_FreeSurface(surface);

	char title[100];
	mpf = AVIStreamSampleToTime(pavi, lastFrame) / lastFrame;
	wsprintf(title, "AVI Player: Width: %d, Height: %d, Frames: %d, Miliseconds per frame: %d", width, height, lastFrame, mpf);
	app->win->SetTitle(title);

	return true;
}

void Video::CloseAVI()
{
	// TODO 3.2: Use AVIStreamGetFrameClose to deallocate the getframe resources.
	AVIStreamGetFrameClose(pgf);                // Deallocates The GetFrame Resources

	//TODO 2.2: Use AVIStreamRelease(...) to release the stream.
	AVIStreamRelease(pavi);                     // Release The Stream

	AVIFileExit();                              // Release The File
}


