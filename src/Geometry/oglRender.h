#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "scene.h"

namespace byhj {
	class OGLRender : public OGLApp {
	public:
		OGLRender();
		~OGLRender();

		void v_init() override;
		void v_update() override;
		void v_render() override;
		void v_shutdown() override;

	private:
		Scene  m_scene;
	};
}
#endif