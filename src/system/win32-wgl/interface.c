#include <GL/gl.h>
#include <GL/wgl.h>
#include <GL/wglext.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "breadbox.h"

#define NSEC_PER_TICK (1000000000 / BREADBOX_TICKRATE)

HWND window;
HDC devicecontext;
struct timespec EPOCH;

// find a better way to do this
RECT windowrect;
int width;
int height;

LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
	{
		GetClientRect(window, &windowrect);
		width = windowrect.right - windowrect.left;
		height = windowrect.bottom - windowrect.top;
		
		glViewport(0, 0, width, height);
	}
	break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int create_window(char *name, int width, int height) {
	WNDCLASS wndclass = {0};
	wndclass.lpszClassName = "breadboxwindow";
	wndclass.lpfnWndProc = WndProc;
	wndclass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wndclass.style = CS_OWNDC;

	RegisterClass(&wndclass);

	window =
		CreateWindow("breadboxwindow", name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
					 CW_USEDEFAULT, width, height, NULL, NULL, NULL, NULL);

	if(window == NULL)
		return 1;

	return 0;
}

float get_subtick() {
    float current = 0.0;
    struct timespec now;
    if(clock_gettime(CLOCK_MONOTONIC, &now)) {
        puts("breadbox_get_subtick: Failed to read monotonic clock!");
    } else {
        now.tv_sec -= EPOCH.tv_sec;
        if(EPOCH.tv_nsec > now.tv_nsec) {
            now.tv_nsec = 1000000000 + (now.tv_nsec - EPOCH.tv_nsec);
            now.tv_sec--;
        } else {
            now.tv_nsec -= EPOCH.tv_nsec;
        }
        current = (float)(now.tv_sec * BREADBOX_TICKRATE) + ((float)now.tv_nsec / (float)NSEC_PER_TICK);
    }
    return current;
}

int main(int argc, char* argv[]) {
	int alive = 1;
	MSG win32msg = {};
	HGLRC glrenderingcontext;

	int expected_tick;
	breadbox_t engine;
	breadbox_message_t msg;
	struct timespec now;

	breadbox_model_init(&engine.model);
	breadbox_subscription_init(&engine.subscriptions);
	breadbox_init(&engine);

	if(create_window("breadbox", 640, 480)) {
		puts("main: Unable to create window");
		return 1;
	}

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
		PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
		32,			   // Colordepth of the framebuffer.
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		24, // Number of bits for the depthbuffer
		8,	// Number of bits for the stencilbuffer
		0,	// Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0,
		0,
		0};

	devicecontext = GetDC(window);

	int pixelformat = ChoosePixelFormat(devicecontext, &pfd);
	SetPixelFormat(devicecontext, pixelformat, &pfd);

	glrenderingcontext = wglCreateContext(devicecontext);

	wglMakeCurrent(devicecontext, glrenderingcontext);
	glEnable(GL_DEPTH_TEST);
	
	int vsync = 1;
	if(vsync) {
		char* extensions = (char*)glGetString(GL_EXTENSIONS);
		if(strstr(extensions, "EXT_swap_control_tear") != NULL) {
			// Adaptive vsync
			wglGetProcAddress("wglSwapIntervalEXT")(-1);
		} else if (strstr(extensions, "WGL_EXT_swap_control") != NULL) {
			// Normal vsync
			wglGetProcAddress("wglSwapIntervalEXT")(1);
		}
	}

	ShowWindow(window, SW_NORMAL);

	GetClientRect(window, &windowrect);
	width = windowrect.right - windowrect.left;
	height = windowrect.bottom - windowrect.top;
	
	glViewport(0, 0, width, height);

	if(clock_gettime(CLOCK_MONOTONIC, &EPOCH)) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(glrenderingcontext);
		ReleaseDC(window, devicecontext);
		DestroyWindow(window);
		return 1;
	}

	while(alive) {
		if(clock_gettime(CLOCK_MONOTONIC, &now)) {
            puts("main: Failed to read the monotonic clock! Things might get weird!");
        // Breadbox treats BREADBOX_TICKRATE as the number of expected clock
        // ticks in a second. However, Unix likes to treat the time as a number
        // of seconds since a certain point. Nanoseconds are this awkward
        // secondary value we need to pay attention to. The easiest way to
        // measure time accurately would probably be to convert the time given
        // by clock_gettime into the number of expected ticks and comparing what
        // we believe the current tick is to that. Assuming the machine isn't
        // running too far behind, this also allows it to catch up in case one
        // tick was particularly taxing on the hardware. ~Alex
        } else {
            // We'll need to adjust the struct timespec to reflect time since
            // the epoch and not time since the system started. ~Alex
            now.tv_sec -= EPOCH.tv_sec;
            if(EPOCH.tv_nsec > now.tv_nsec) {
                now.tv_nsec = 1000000000 + (now.tv_nsec - EPOCH.tv_nsec);
                now.tv_sec--;
            } else {
                now.tv_nsec -= EPOCH.tv_nsec;
            }
            expected_tick = (now.tv_sec * BREADBOX_TICKRATE) + (now.tv_nsec / NSEC_PER_TICK);
            if(expected_tick > engine.model.tick) {
                if(!++engine.model.tick) {
                    // If anybody is crazy enough to run the engine for this long,
                    // let me know about it. They're my kind of crazy. ~Alex
                    puts(
                        "main: Did you seriously run this game for almost 7 years"
                        "straight? You have my respect. This message will now"
                        "self-destruct. ~Alex"
                    );
                    alive = 0;
                    break;
                } else {
                    if(expected_tick > engine.model.tick) {
                        printf(
                            "main: Running %u ticks behind! What's going on?\n",
                            expected_tick - engine.model.tick
                        );
                    }
                    msg.type = BBMSG_TICK;
                    breadbox_publish(&engine, &msg);
                }
            }
        }

		if(PeekMessage(&win32msg, NULL, 0, 0, PM_REMOVE)) {
			if(win32msg.message == WM_QUIT) {
				break;
			}

			switch(win32msg.message) {
				case WM_KEYDOWN:
					printf("KEY: +%u\n", win32msg.wParam);
					break;
				case WM_KEYUP:
					printf("KEY: -%u\n", win32msg.wParam);
					break;
				default:
					break;
			}

			// pass to WndProc
			TranslateMessage(&win32msg);
			DispatchMessage(&win32msg);
			continue;
		}

		if(width != 0 && height != 0) {
            view(&engine.model);
		}
	}
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glrenderingcontext);
	ReleaseDC(window, devicecontext);
	DestroyWindow(window);
	return 0;
}