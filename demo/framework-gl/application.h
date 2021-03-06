/*
 * Copyright (c) 2014 Roman Kuznetsov 
 *
 * Thanks to Graham Sellers (and his book "OpenGL SuperBible, 6th Edition") for inspiration and some code
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

namespace framework
{

class Application
{
	friend class Destroyable;

public:
	Application();
	virtual ~Application(){}

	virtual void init(const std::map<std::string, int>& params){}
	virtual void startup(gui::WidgetPtr_T root){}
	virtual void render(double elapsedTime){}
	virtual void shutdown(){}
	virtual void onResize(int width, int height){}
	virtual void onKeyButton(int key, int scancode, bool pressed){}
	virtual void onMouseButton(double xpos, double ypos, int button, bool pressed){}
	virtual void onMouseMove(double xpos, double ypos){}

	static Application* instance();
	int run(Application* self, const std::string& commandLine);
	void exit();
	void resize();
	bool isDebugEnabled() const;
	vector2 getScreenSize();

protected:
	struct AppInfo
    {
        std::string title;
        int windowWidth;
        int windowHeight;
        int samples;
        union
        {
            struct
            {
                unsigned int fullscreen  : 1;
                unsigned int vsync       : 1;
                unsigned int cursor      : 1;
                unsigned int debug       : 1;
				unsigned int useStencil  : 1;
            };
            unsigned int	 all;
        } flags;
		
		AppInfo();
    };

	AppInfo m_info;
	LightManager m_lightManager;
	gui::LabelPtr_T m_fpsLabel;
	gui::LabelPtr_T m_legendLabel;

	void renderGui(double elapsedTime);
	void renderAxes(const matrix44& viewProjection);
	void renderSkybox(Camera& camera, std::shared_ptr<Texture> texture);

	void useDefaultRenderTarget();
	void clearDefaultRenderTarget(const vector4& color = vector4(0, 0, 0, 0), float depth = 1.0f);

	void applyStandardParams(const std::map<std::string, int>& params);
	void setLegend(const std::string& legend);

private:
	static Application* m_self;
	OpenGLContext m_context;
	utils::Timer m_timer;
	bool m_isRunning;
	double m_lastTime;	
	std::string m_legend;
	utils::FpsCounter m_fpsCounter;

	std::list<std::weak_ptr<Destroyable> > m_destroyableList;
	std::shared_ptr<Line3D> m_axisX;
	std::shared_ptr<Line3D> m_axisY;
	std::shared_ptr<Line3D> m_axisZ;

	void registerDestroyable(std::weak_ptr<Destroyable> ptr);
	void destroyAllDestroyable();

	bool initGui();
	void destroyGui();

	void initAxes();
	void initInput();

	void mainLoop();

	static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);
};

}
#if defined _WIN32
#define DECLARE_MAIN(A)                             \
int CALLBACK WinMain(HINSTANCE hInstance,           \
                     HINSTANCE hPrevInstance,       \
                     LPSTR lpCmdLine,               \
                     int nCmdShow)                  \
{                                                   \
	utils::Utils::init();							\
	utils::Logger::start(utils::Logger::IDE_OUTPUT |\
						 utils::Logger::FILE);		\
    A *app = new A();                               \
    int result = app->run(app, lpCmdLine);          \
    delete app;                                     \
	utils::Logger::finish();					    \
    return result;                                  \
}
#elif defined __APPLE__
#define DECLARE_MAIN(a)                             \
int main(int argc, const char ** argv)              \
{                                                   \
	utils::Utils::init();							\
	utils::Logger::start(utils::Logger::IDE_OUTPUT |\
						 utils::Logger::FILE);		\
    A *app = new A();                               \
    int result = app->run(app);                     \
    delete app;                                     \
	utils::Logger::finish();					    \
    return result;                                  \
}
#else
#error Undefined platform!
#endif

#endif // __APPLICATION_H__