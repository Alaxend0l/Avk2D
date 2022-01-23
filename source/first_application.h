#pragma once

#include "avk_window.h"
#include "avk_device.h"
#include "avk_entity.h"
#include "avk_renderer.h"

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
		void loadEntities();

		AvkWindow avkWindow{ WIDTH, HEIGHT, "Avk Hello World!" };
		AvkDevice avkDevice{ avkWindow };
		AvkRenderer avkRenderer {avkWindow, avkDevice};

		std::vector<AvkEntity> entities;
	};
}