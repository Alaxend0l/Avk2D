#pragma once

#include "core/avk_window.h"
#include "core/avk_device.h"
#include "core/avk_renderer.h"
#include "core/avk_model.h"

#include "scene/avk_entity.h"


// std

#include <memory>
#include <vector>

namespace avk
{
	class FirstApplication
	{
	public:
		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;

		FirstApplication();
		~FirstApplication();

		FirstApplication(const FirstApplication&) = delete;
		FirstApplication& operator=(const FirstApplication&) = delete;

		void run();

	private:
		void LoadEntities(std::shared_ptr<AvkScene>& scene);

		AvkWindow avkWindow{ WIDTH, HEIGHT, "Avk Hello World!" };
		AvkDevice avkDevice{ avkWindow };
		AvkRenderer avkRenderer {avkWindow, avkDevice};
		std::shared_ptr<AvkScene> avkScene;
	};
}