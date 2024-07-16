#pragma once
#include "Core.h"

namespace Ivory {
	class IVORY_API Application
	{
	public:
		Application() {}
		virtual ~Application() {}

		void run();
	};

	Application* create_application();
}
