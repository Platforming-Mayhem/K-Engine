#include "FollowPlayer.h"
#include "Editor.h"

K::FollowPlayer::FollowPlayer()
{
}

K::FollowPlayer::~FollowPlayer()
{
	this->player = nullptr;
}

void K::FollowPlayer::SetPlayer(K::Player* newPlayer) 
{
	this->player = newPlayer;
}

void K::FollowPlayer::Init()
{
	for (auto temp : K::Editor::GetCurrentScene()->GetGameObjects()) 
	{
		if (temp.second->GetComponentOfType(typeid(K::Player).name()) != nullptr)
		{
			this->player = (K::Player*)temp.second->GetComponentOfType(typeid(K::Player).name());
			break;
		}
	}
}

void K::FollowPlayer::Update()
{
	#if _DEBUG
	#else
		if (this->player != nullptr) 
		{
			K::Vector3 destination = K::Vector3(this->player->parent->GetTransform()->position->x, this->parent->GetTransform()->position->y, this->player->parent->GetTransform()->position->z);
			*this->parent->GetTransform()->position = K::Vector3::Lerp(*this->parent->GetTransform()->position, destination, K::Time::deltaTime() * 6.0f);
		}
	#endif
}

void K::FollowPlayer::UpdateEditor()
{
	if (ImGui::CollapsingHeader("Follow Player Settings"))
	{
		
	}
}

void K::FollowPlayer::Bind()
{
}

void K::FollowPlayer::Unbind()
{
}

void K::FollowPlayer::SetPropertyValues(const char* value, int valueIndex)
{
	std::string temp = value;
	switch (valueIndex)
	{
	case 0:
		break;
	}
}

const char* K::FollowPlayer::GetPropertyValues()
{
	return this->properties.c_str();
}

const char* K::FollowPlayer::GetName()
{
	return typeid(K::FollowPlayer).name();
}
