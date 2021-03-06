//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_WINDOW_HPP
#define VD_GAME_ENGINE_WINDOW_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

#include <engine/defines/Types.hpp>

#include <engine/logger/Logger.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/event/EventHandler.hpp>

#include <engine/component/IManager.hpp>

#include <GLFW/glfw3.h>

namespace vd::window {
    class WindowManager;

    class Window {
    public:
        Window(uint32_t width, uint32_t height, std::string title);

        void Build();
        void Dispose();
        void Resize(uint32_t width, uint32_t height);

        [[nodiscard]] uint32_t Width() const;
        [[nodiscard]] uint32_t Height() const;

        float& NearPlane();
        float& FarPlane();
        float& FieldOfView();

        [[nodiscard]] float AspectRatio() const;

        [[nodiscard]] bool PerspectiveChanged() const;
        [[nodiscard]] bool CloseRequested() const;

        [[nodiscard]] glm::mat4 ProjectionMatrix() const;
        [[nodiscard]] glm::mat4 OrthoProjectionMatrix() const;
    private:
        friend class WindowManager;

        static Key::Code ToKey(int key);
        static Action::Code ToAction(int action);
        static Button::Code ToButton(int button);

        static void WindowResizeCallback(GLFWwindow* window, int32_t width, int32_t height);
        static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
        static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
        static void MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);

        bool  m_CloseRequested;
        float m_NearPlane;
        float m_FarPlane;
        float m_FieldOfView;

        GLFWwindow* m_Window;

        vd::Dimension   m_Dimension;
        std::string     m_Title;

        bool m_Changed;
    };
    typedef std::shared_ptr<Window>	WindowPtr;

    class WindowManager : public vd::component::IManager, public vd::injector::Injectable {
    public:
        WindowManager();

        void Link() override;

        void Init() override;
        void Update() override;
        void Render(const vd::datastruct::Observer::params_t& params) override;
        void CleanUp() override;
    private:
        WindowPtr               m_pWindow;
        event::EventHandlerPtr  m_pEventHandler;
    };
    typedef std::shared_ptr<WindowManager>  WindowManagerPtr;
}

#endif //VD_GAME_ENGINE_WINDOWMANAGER_HPP