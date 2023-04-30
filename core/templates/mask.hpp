#ifndef _PN_BITSET_HPP_
#define _PN_BITSET_HPP_

#include <core/templates/array.hpp>

#include <bitset>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define bit_read(m_value, m_bit) \
		(((m_value) >> (m_bit)) & 1)

#define bit_set(m_value, m_bit) \
		((m_value) |= 1 << (m_bit))

#define bit_clear(m_value, m_bit) \
		((m_value) &= ~(1 << (m_bit)))

#define bit_write(m_value, m_bit, m_bitvalue) \
		((m_bitvalue) ? bit_set((m_value), (m_bit)) : bit_clear((m_value), (m_bit)))

#define bit_map(m_dst, m_dst_index, m_src, m_src_index) \
		bit_write(m_dst, m_dst_index, bit_read(m_src, m_src_index))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define flag_read(m_value, m_flag) \
		(((m_flag) & (m_value)) == (m_flag))

#define flag_set(m_value, m_flag) \
		((m_value) |= (m_flag))

#define flag_clear(m_value, m_flag) \
		((m_value) &= ~(m_flag))

#define flag_write(m_value, m_flag, m_flagvalue) \
		((m_flagvalue) ? flag_set(m_value, m_flag) : flag_clear(m_value, m_flag))

#define flag_map(m_dst, m_dst_index, m_src, m_src_index) \
		flag_write(m_dst, m_dst_index, flag_read(m_src, m_src_index))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace pn
{
	// mask base
	template <size_t _Bits
	> using _MaskBase = std::bitset<_Bits>;

	// mask
	template <size_t _Bits
	> class Mask : public _MaskBase<_Bits>
	{
	public:
		using base_type = _MaskBase<_Bits>;
		using base_type::base_type;
		using base_type::operator=;
		using base_type::operator&=;
		using base_type::operator|=;
		using base_type::operator^=;
		using base_type::operator<<=;
		using base_type::operator>>=;
		using base_type::operator~;
		using base_type::operator==;
		using base_type::operator!=;
		using base_type::operator<<;
		using base_type::operator>>;
		using base_type::set;
		using base_type::reset;
		using base_type::flip;
		using base_type::to_ulong;
		using base_type::to_ullong;
		using base_type::to_string;
		using base_type::count;
		using base_type::size;
		using base_type::m_test;
		using base_type::any;
		using base_type::none;
		using base_type::all;
		using base_type::_Getword;
		using base_type::_Validate;
		using base_type::_Subscript;
	};

	using Mask32 = Mask<32>;
	using Mask64 = Mask<64>;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_PN_BITSET_HPP_
