#ifndef __ENGINE_BLOCK_HPP_
#define __ENGINE_BLOCK_HPP_

#include <engine/foundation/GL.hpp>

#include <thread>
#include <iostream>
#include <string>
#include <vector>

#include "EngineWorker.hpp"

#include <engine/core/Window.hpp>

#include <engine/core/impl/EntityCamera.hpp>
#include <engine/core/impl/FreeCamera.hpp>

#include <engine/misc/Properties.hpp>
#include <engine/core/ObjectOfType.hpp>

#include <engine/shadow/ShadowManager.hpp>
#include <engine/light/LightManager.hpp>

#include <engine/glmodel/buffer/FrameBuffer.hpp>

#include <engine/config/MetaConfig.hpp>

#include <engine/culling/FrustumCullingManager.hpp>

namespace vd {
	class Engine {
	public:
	    typedef std::function<bool()> FramebufferPreconditionFunc;

	    enum CameraMode {
	        eFreeCamera = 0,
	        e3rdPersonCamera
	    };

		Engine();
		
		void setup(int windowWidth, int windowHeight, const char* windowTitle);
		void init(core::CameraInitParametersPtr cameraInitParametersPtr);
		void start();

		// Get the number of frames per second of the engine
		[[nodiscard]] int getFramesPerSecond() const;

		// Get the time necessary by a frame to be completely rendered
		[[nodiscard]] float getFrameTime() const;

		kernel::EngineWorkerPtr& getWorker();
		[[nodiscard]] const kernel::EngineWorkerPtr& getWorker() const;

		core::InputHandlerPtr& getInputHandler();
		[[nodiscard]] const core::InputHandlerPtr& getInputHandler() const;

		core::WindowPtr& getWindow();
		[[nodiscard]] const core::WindowPtr& getWindow() const;

		[[nodiscard]] const CameraMode& getCameraMode() const;

		core::CameraPtr& getCamera();
		[[nodiscard]] const core::CameraPtr& getCamera() const;

		core::CameraPtr& getFreeCamera();
		[[nodiscard]] const core::CameraPtr& getFreeCamera() const;

		core::CameraPtr& getEntityCamera();
		[[nodiscard]] const core::CameraPtr& getEntityCamera() const;

        shadow::ShadowManagerPtr& getShadowManager();
        [[nodiscard]] const shadow::ShadowManagerPtr& getShadowManager() const;

		[[nodiscard]] const glm::vec4& getClipPlane() const;
		void setClipPlane(const glm::vec4& clipPlane);

		void addRenderingFramebuffer(const buffer::FrameBufferPtr& frameBufferPtr,
                               const FramebufferPreconditionFunc& preconditionCheck,
                               const config::MetaConfigPtr& configPtr,
                               const kernel::RenderingPass& renderingPass);
	private:
		void run();
		void stop();
		void update();
		void render();
		void cleanUp();

		int fps; // frames per second
		float frameTimeInSeconds; // frame time (in seconds)

		float frameTime;
		bool isRunning;

		core::InputHandlerPtr inputHandlerPtr;
		core::WindowPtr windowPtr;

		CameraMode cameraMode;
		core::CameraPtr entityCameraPtr;
		core::CameraPtr freeCameraPtr;

		shadow::ShadowManagerPtr shadowManagerPtr;
		light::LightManagerPtr m_LightManagerPtr;

		kernel::EngineWorkerPtr engineWorkerPtr;

        culling::FrustumCullingManagerPtr m_FrustumCullingManagerPtr;

		glm::vec4 clipPlane;

		struct RenderingFrameBuffer {
		    buffer::FrameBufferPtr frameBufferPtr;
            FramebufferPreconditionFunc preconditionFunc;
		    config::MetaConfigPtr configPtr;
            kernel::RenderingPass renderingPass;
		};

		std::vector<RenderingFrameBuffer> renderingFrameBuffers;

	};
	typedef std::shared_ptr<Engine>	EnginePtr;
}

#endif // !__ENGINE_BLOCK_HPP_