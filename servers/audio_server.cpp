#include <servers/audio_server.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(AudioServer, t) {}

	AudioServer * AudioServer::__singleton{};

	AudioServer::AudioServer() { SINGLETON_CTOR(); }

	AudioServer::~AudioServer() { SINGLETON_DTOR(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}