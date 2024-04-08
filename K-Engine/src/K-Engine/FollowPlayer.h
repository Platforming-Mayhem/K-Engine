#pragma once
#include "build.h"
#include "Component.h"
#include "Player.h"

namespace K 
{
	class K_API FollowPlayer : public K::Component 
	{
	private:
		std::string properties;
		K::Player* player = nullptr;
	public:
		FollowPlayer();

		~FollowPlayer();

		void SetPlayer(K::Player* newPlayer = nullptr);

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;

		const char* GetName() override;
	};
}