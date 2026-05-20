# REQUIRED

CMAKE 3.11 or Higher
C++23 or Higher

# Included libaries

[GLEW](https://github.com/nigels-com/glew)

[GLFW](https://github.com/glfw/glfw)

[IMGUI docking branch](https://github.com/ocornut/imgui/tree/docking)

[MiniAudio](https://github.com/mackron/miniaudio)

[NativeFileDialogue-Extended](https://github.com/btzy/nativefiledialog-extended)

[Modified version of STB_Image](https://github.com/nothings/stb/blob/master/stb_image.h)

# Overview

The design of this engine is highly inspired by Unity.
A project created by K-Engine has its own set of components that are built by the engine at runtime.
The engine also has the capability to build the game out as a separate standalone executable, however currently only Windows is 100% supported.
It does work in Linux but you need to manually copy the libK-Engine.so to the same directory as the executable, also debug logs are still printed in the console.
This engine supports PNGs, JPEGs and Animated GIFs.

# Compiling

Just run either the "Recompile-and-start.bat"(for Windows) or the "Recompile-and-start.sh"(for Linux).

When creating a project with K-Engine, choose an empty folder and the file directory will automatically be setup.
A CMakeLists.txt file will be created within said directory and any .h or .cpp file will be automatically built and linked to the K-Engine solution.

# Examples

[WIP: Save The Baby 2](https://github.com/Platforming-Mayhem/SaveTheBaby2)

# Component Script Example

Header

	#pragma once
	#include <K_Engine.h>

	namespace K 
	{
		class KC_API $safeitemname$ : public K::Component
		{
		private:
			std::string properties;
		public:
			$safeitemname$();

			~$safeitemname$();

			void Update() override;

			void UpdateEditor() override;

			void SetPropertyValues(const char* value, int valueIndex) override;

			const char* GetPropertyValues() override;
		};
	}

CPP

	#include "$safeitemname$.h"

	namespace K 
	{
		REGISTER($safeitemname$);

		$safeitemname$::$safeitemname$()
		{
		}

		$safeitemname$::~$safeitemname$()
		{
		}

		void $safeitemname$::Update()
		{

		}

		void $safeitemname$::UpdateEditor()
		{
			if (ImGui::CollapsingHeader(this->GetName()))
			{

			}
		}

		void $safeitemname$::SetPropertyValues(const char* value, int valueIndex)
		{
			if (value[0] != '\0' && value != nullptr)
			{
				std::string temp = value;
				switch (valueIndex)
				{
				case 0:
					break;
				}
			}
		}

		const char* $safeitemname$::GetPropertyValues()
		{
			return this->properties.c_str();
		}
	}
