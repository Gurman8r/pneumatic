#include <servers/audio_server.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(AudioServer, t) {}

	AudioServer * AudioServer::__singleton{};

	AudioServer::AudioServer() { SINGLETON_CTOR(); }

	AudioServer::~AudioServer() { SINGLETON_DTOR(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}