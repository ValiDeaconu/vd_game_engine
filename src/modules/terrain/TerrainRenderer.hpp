#ifndef __TERRAIN_RENDERER_HPP_
#define __TERRAIN_RENDERER_HPP_

#include <string>

#include <engine/component/Renderer.hpp>
#include <engine/shader/Shader.hpp>

#include <engine/kernel/EngineWorker.hpp>

#include "Terrain.hpp"

namespace mod
{
	namespace terrain
	{
		class TerrainRenderer : public vd::component::Renderer
		{
		public:
			TerrainRenderer();
			~TerrainRenderer();

			void init();
			void update();
			void cleanUp();

			TerrainPtr& getTerrain();
			const TerrainPtr& getTerrain() const;
			void setTerrain(const TerrainPtr& terrainPtr);

		private:
			bool isReady();

			TerrainPtr terrainPtr;
		};
		typedef std::shared_ptr<TerrainRenderer>	TerrainRendererPtr;
	}
}

#endif // !__TERRAIN_RENDERER_HPP_