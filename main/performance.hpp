#ifndef _PN_PERFORMANCE_HPP_
#define _PN_PERFORMANCE_HPP_

#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FPS_Tracker final
	{
		f32			value{};
		f32			accum{};
		size_t		index{};
		Vector<f32>	times{};

		FPS_Tracker(size_t count = 120) noexcept { times.resize(count); }

		void update(Duration const & dt) noexcept
		{
			accum += (f32)dt - times[index];
			times[index] = dt;
			index = (index + 1) % times.size();
			value = (0.f < accum) ? (1.f / (accum / (f32)times.size())) : FLT_MAX;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PN_API Performance : public Object
	{
		DEFINE_CLASS(Performance, Object);

		static Performance * __singleton;

	public:
		Performance();
		virtual ~Performance() override;
		FORCE_INLINE static Performance * get_singleton() noexcept { return __singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_PERFORMANCE_HPP_
