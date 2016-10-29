#pragma once
#include <memory>
#include <list>
#include <d3d11.h>
#include <SimpleMath.h>
#include "MeshID.h"
#include "TextureID.h"
#include "RenderType.h"
//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;

namespace Graphic {
	namespace NScene {
		class Object {
		private:
			bool m_isDirty;
		protected:
		public:
			Vector3 m_scale;
			Vector3 m_pos;
			Quaternion m_rotation;
			Matrix m_matModel;
			
			//Information required for rendering process
			RENDER_TYPE				m_renderType;
			MESH_TYPE				m_meshType;
			std::list<TextureID>	m_textures; // textures in order in which the shader processes the textures

			Object();
			virtual void setScale(Vector3 scale);
			virtual void setPos(Vector3 pos);
			virtual void setRotation(Quaternion quaternion);
			Matrix getModelMatrix();
		};
	}
}