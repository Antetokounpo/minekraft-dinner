#include<iostream>
#include<vector>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<GL/glew.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#include "engine/model.hpp"
#include "engine/texture.hpp"
#include "engine/shader.hpp"
#include "engine/light.hpp"
#include "engine/renderer/camera.hpp"
#include "engine/renderer/renderer.hpp"
#include "engine/loaders/objloader.hpp"

#include "terrain/chunk.hpp"
#include "terrain/terrain.hpp"

#include "game/world.hpp"

void opengl_debug_cb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    std::cout << "OpenGL error: ";
    std::cout << message << '\n';
}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window* window = SDL_CreateWindow("KraftDinner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    Uint32 t = SDL_GetTicks();

    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        std::cout << glewGetErrorString(err) << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    IMG_Init(IMG_INIT_PNG);

    Renderer renderer(window);
    renderer.load_texture("res/tex/atlas.png");
    renderer.load_shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    renderer.load_skybox_shader("shaders/skybox_vertex.glsl", "shaders/skybox_fragment.glsl");
    World world(window);
    world.get_terrain().set_seed(1234);
    world.get_skybox().load({"res/tex/right.png", "res/tex/left.png", "res/tex/top.png", "res/tex/bottom.png", "res/tex/front.png", "res/tex/back.png"});

    SDL_GL_SetSwapInterval(1);
    SDL_ShowCursor(SDL_DISABLE);

    glEnable(GL_DEPTH_TEST); // So triangles don't "overlap"
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    #ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(opengl_debug_cb, 0);
    #endif

    /* Init ImGui */
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark();

    bool quit = false;
    while(!quit)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                quit = true;
            else
                world.handle_events(e);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1, 1, 1, 1);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        world.update();
        renderer.render_world(world);

        ImGui::Begin("Coords");
        ImGui::TextColored(ImVec4(0, 0.5, 1, 1), ("X : " + std::to_string(world.get_player().get_position().x)).c_str());
        ImGui::TextColored(ImVec4(0, 1, 0, 1), ("Y : " + std::to_string(world.get_player().get_position().y)).c_str());
        ImGui::TextColored(ImVec4(1, 0, 0, 1), ("Z : " + std::to_string(world.get_player().get_position().z)).c_str());
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
        //std::cout << 1000.0f/(SDL_GetTicks() - t) << " fps\n";
        t = SDL_GetTicks();
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
