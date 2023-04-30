#ifndef _PN_BATCH_HPP_
#define _PN_BATCH_HPP_

#include <core/templates/mpl.hpp>
#include <core/templates/vector.hpp>

namespace pn
{
	// tuple<vector<T>...>
	template <class ... _Types
	> class Batch
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename Batch<_Types...>;
		using allocator_type	= typename PolymorphicAllocator<u8>;
		using value_types		= typename mpl::type_list<_Types...>;
		using value_tuple		= typename mpl::tuple<value_types>;
		using vector_types		= typename mpl::remap<pn::Vector, value_types>;
		using vector_tuple		= typename mpl::tuple<vector_types>;
		using init_type			= typename std::initializer_list<value_tuple>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t npos
		{
			static_cast<size_t>(-1)
		};

		static constexpr size_t tuple_size
		{
			std::tuple_size_v<value_tuple>
		};

		static constexpr auto indices
		{
			std::make_index_sequence<tuple_size>{}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I>	using element_i			= mpl::nth<I, value_types>;
		template <class  T> using element_t			= T;
		template <size_t I>	using vector_i			= mpl::nth<I, vector_types>;
		template <class  T> using vector_t			= pn::Vector<T>;
		template <size_t I>	using iterator_i		= typename vector_i<I>::iterator;
		template <class  T>	using iterator_t		= typename vector_t<T>::iterator;
		template <size_t I>	using const_iterator_i	= typename vector_i<I>::const_iterator;
		template <class  T>	using const_iterator_t	= typename vector_t<T>::const_iterator;

		using iterator_types		= typename mpl::remap<iterator_t, value_types>;
		using iterator_tuple		= typename mpl::tuple<iterator_types>;

		using const_iterator_types	= typename mpl::remap<const_iterator_t, value_types>;
		using const_iterator_tuple	= typename mpl::tuple<const_iterator_types>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Batch() noexcept
			: m_data{}
		{
		}

		Batch(std::allocator_arg_t, allocator_type alloc) noexcept
			: m_data{ std::allocator_arg, alloc }
		{
		}

		Batch(allocator_type alloc) noexcept
			: m_data{ std::allocator_arg, alloc }
		{
		}

		Batch(size_t nitems, allocator_type alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc }
		{
			this->reserve(nitems);
		}

		Batch(init_type value, allocator_type alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc }
		{
			this->reserve(value.size());

			for (auto const & elem : value)
			{
				this->emplace_back(elem);
			}
		}

		explicit Batch(vector_tuple const & value, allocator_type alloc = {})
			: m_data{ std::allocator_arg, alloc, value }
		{
		}

		explicit Batch(vector_tuple && value, allocator_type alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc, std::move(value) }
		{
		}

		Batch(self_type const & value, allocator_type alloc = {})
			: m_data{ std::allocator_arg, alloc, value.m_data }
		{
		}

		Batch(self_type && value, allocator_type alloc = {}) noexcept
			: m_data{ std::allocator_arg, alloc, std::move(value.m_data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & operator=(init_type value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(vector_tuple const & value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(vector_tuple && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		self_type & operator=(self_type const & value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(self_type && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		decltype(auto) data() & noexcept { return m_data; }

		decltype(auto) data() const & noexcept { return m_data; }

		decltype(auto) data() && noexcept { return std::move(m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> decltype(auto) get() & noexcept
		{
			return std::get<I>(m_data);
		}

		template <size_t I> decltype(auto) get() const & noexcept
		{
			return std::get<I>(m_data);
		}

		template <size_t I> decltype(auto) get() && noexcept
		{
			return std::move(std::get<I>(m_data));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> decltype(auto) get() & noexcept
		{
			return std::get<vector_t<T>>(m_data);
		}

		template <class T> decltype(auto) get() const & noexcept
		{
			return std::get<vector_t<T>>(m_data);
		}

		template <class T> decltype(auto) get() && noexcept
		{
			return std::move(std::get<vector_t<T>>(m_data));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is
		> decltype(auto) get(size_t const i, std::index_sequence<Is...>) noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>()[i]...);
			}
		}

		template <size_t ... Is
		> decltype(auto) get(size_t const i, std::index_sequence<Is...>) const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>()[i]...);
			}
		}

		template <size_t ... Is
		> decltype(auto) get(size_t const i) noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>()[i]...);
			}
		}

		template <size_t ... Is
		> decltype(auto) get(size_t const i) const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>()[i]...);
			}
		}

		template <class ... Ts
		> decltype(auto) get(size_t const i) noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>()[i]...);
			}
		}

		template <class ... Ts
		> decltype(auto) get(size_t const i) const noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>()[i];
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>()[i]...);
			}
		}

		decltype(auto) get(size_t const i) noexcept
		{
			return this->get(i, indices);
		}

		decltype(auto) get(size_t const i) const noexcept
		{
			return this->get(i, indices);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is
		> decltype(auto) back(std::index_sequence<Is...>) noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().back()...);
			}
		}

		template <size_t ... Is
		> decltype(auto) back(std::index_sequence<Is...>) const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().back()...);
			}
		}

		template <size_t ... Is
		> decltype(auto) back() noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().back()...);
			}
		}

		template <size_t ... Is
		> decltype(auto) back() const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().back()...);
			}
		}

		template <class ... Ts
		> decltype(auto) back() noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>().back()...);
			}
		}

		template <class ... Ts
		> decltype(auto) back() const noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>().back();
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>().back()...);
			}
		}

		decltype(auto) back() noexcept
		{
			return this->back(indices);
		}

		decltype(auto) back() const noexcept
		{
			return this->back(indices);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is
		> decltype(auto) front(std::index_sequence<Is...>) noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().front()...);
			}
		}

		template <size_t ... Is
		> decltype(auto) front(std::index_sequence<Is...>) const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().front()...);
			}
		}

		template <size_t ... Is
		> decltype(auto) front() noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().front()...);
			}
		}

		template <size_t ... Is
		> decltype(auto) front() const noexcept
		{
			if constexpr (1 == sizeof...(Is))
			{
				return this->get<Is...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Is>().front()...);
			}
		}

		template <class ... Ts
		> decltype(auto) front() noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>().front()...);
			}
		}

		template <class ... Ts
		> decltype(auto) front() const noexcept
		{
			if constexpr (1 == sizeof...(Ts))
			{
				return this->get<Ts...>().front();
			}
			else
			{
				return std::forward_as_tuple(this->get<Ts>().front()...);
			}
		}

		decltype(auto) front() noexcept
		{
			return this->front(indices);
		}

		decltype(auto) front() const noexcept
		{
			return this->front(indices);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn, size_t ... Is
		> void expand(Fn && fn, std::index_sequence<Is...>) noexcept
		{
			std::invoke(FWD(fn), this->get<Is>()...);
		}

		template <class Fn, size_t ... Is
		> void expand(Fn && fn, std::index_sequence<Is...>) const noexcept
		{
			std::invoke(FWD(fn), this->get<Is>()...);
		}

		template <size_t ... Is, class Fn
		> void expand(Fn && fn) noexcept
		{
			std::invoke(FWD(fn), this->get<Is>()...);
		}

		template <size_t ... Is, class Fn
		> void expand(Fn && fn) const noexcept
		{
			std::invoke(FWD(fn), this->get<Is>()...);
		}

		template <class ... Ts, class Fn
		> void expand(Fn && fn) noexcept
		{
			std::invoke(FWD(fn), this->get<Ts>()...);
		}

		template <class ... Ts, class Fn
		> void expand(Fn && fn) const noexcept
		{
			std::invoke(FWD(fn), this->get<Ts>()...);
		}

		template <class Fn> void expand_all(Fn && fn) noexcept
		{
			this->expand(FWD(fn), indices);
		}

		template <class Fn> void expand_all(Fn && fn) const noexcept
		{
			this->expand(FWD(fn), indices);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn, size_t ... Is
		> void expand(size_t const i, Fn && fn, std::index_sequence<Is...>) noexcept
		{
			std::invoke(FWD(fn), this->get<Is>(i)...);
		}

		template <class Fn, size_t ... Is
		> void expand(size_t const i, Fn && fn, std::index_sequence<Is...>) const noexcept
		{
			std::invoke(FWD(fn), this->get<Is>(i)...);
		}

		template <size_t ... Is, class Fn
		> void expand(size_t const i, Fn && fn) noexcept
		{
			std::invoke(FWD(fn), this->get<Is>(i)...);
		}

		template <size_t ... Is, class Fn
		> void expand(size_t const i, Fn && fn) const noexcept
		{
			std::invoke(FWD(fn), this->get<Is>(i)...);
		}

		template <class ... Ts, class Fn
		> void expand(size_t const i, Fn && fn) noexcept
		{
			std::invoke(FWD(fn), this->get<Ts>(i)...);
		}

		template <class ... Ts, class Fn
		> void expand(size_t const i, Fn && fn) const noexcept
		{
			std::invoke(FWD(fn), this->get<Ts>(i)...);
		}

		template <class Fn> void expand_all(size_t const i, Fn && fn) noexcept
		{
			this->expand(i, FWD(fn), indices);
		}

		template <class Fn> void expand_all(size_t const i, Fn && fn) const noexcept
		{
			this->expand(i, FWD(fn), indices);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> void for_tuple(Fn && fn) noexcept
		{
			mpl::for_tuple(m_data, FWD(fn));
		}

		template <class Fn
		> void for_tuple(Fn && fn) const noexcept
		{
			mpl::for_tuple(m_data, FWD(fn));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t ... Is, class Fn
		> void for_indices(Fn && fn) noexcept
		{
			this->expand<Is...>([&](auto && ... vs) noexcept
			{
				mpl::for_args([&](auto & v) noexcept
				{
					std::invoke(FWD(fn), v);
				}
				, FWD(vs)...);
			});
		}

		template <class ... Ts, class Fn
		> void for_types(Fn && fn) noexcept
		{
			this->expand<Ts...>([&](auto && ... vs) noexcept
			{
				mpl::for_args([&](auto & v) noexcept
				{
					std::invoke(FWD(fn), v);
				}
				, FWD(vs)...);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I = 0> size_t capacity() const noexcept
		{
			return this->get<I>().capacity();
		}

		template <class T> size_t capacity() const noexcept
		{
			return this->get<T>().capacity();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I = 0> bool empty() const noexcept
		{
			return this->get<I>().empty();
		}

		template <class T> bool empty() const noexcept
		{
			return this->get<T>().empty();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I = 0> size_t max_size() const noexcept
		{
			return this->get<I>().max_size();
		}

		template <class T> size_t max_size() const noexcept
		{
			return this->get<T>().max_size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I = 0> size_t size() const noexcept
		{
			return this->get<I>().size();
		}

		template <class T> size_t size() const noexcept
		{
			return this->get<T>().size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept
		{
			this->for_tuple([&](auto & v) noexcept { v.clear(); });
		}

		template <size_t ... Is> void clear() noexcept
		{
			this->for_indices<Is...>([&](auto & v) noexcept { v.clear(); });
		}

		template <class ... Ts> void clear() noexcept
		{
			this->for_types<Ts...>([&](auto & v) noexcept { v.clear(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void pop_back() noexcept
		{
			this->for_tuple([&](auto & v) noexcept { v.pop_back(); });
		}

		template <size_t ... Is> void pop_back() noexcept
		{
			this->for_indices<Is...>([&](auto & v) noexcept { v.pop_back(); });
		}

		template <class ... Ts> void pop_back() noexcept
		{
			this->for_types<Ts...>([&](auto & v) noexcept { v.pop_back(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void pop_front() noexcept
		{
			this->for_tuple([&](auto & v) noexcept { v.erase(v.begin()); });
		}

		template <size_t ... Is> void pop_front() noexcept
		{
			this->for_indices<Is...>([&](auto & v) noexcept { v.erase(v.begin()); });
		}

		template <class ... Ts> void pop_front() noexcept
		{
			this->for_types<Ts...>([&](auto & v) noexcept { v.erase(v.begin()); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void reserve(size_t const value)
		{
			this->for_tuple([&](auto & v) { v.reserve(value); });
		}

		template <size_t ... Is> void reserve(size_t const value)
		{
			this->for_indices<Is...>([&](auto & v) { v.reserve(value); });
		}

		template <class ... Ts> void reserve(size_t const value)
		{
			this->for_types<Ts...>([&](auto & v) { v.reserve(value); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void resize(size_t const value)
		{
			this->for_tuple([&](auto & v) { v.resize(value); });
		}

		template <size_t ... Is> void resize(size_t const value)
		{
			this->for_indices<Is...>([&](auto & v) { v.resize(value); });
		}

		template <class ... Ts> void resize(size_t const value)
		{
			this->for_types<Ts...>([&](auto & v) { v.resize(value); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void shrink_to_fit() noexcept
		{
			this->for_tuple([&](auto & v) noexcept { v.shrink_to_fit(); });
		}

		template <size_t ... Is> void shrink_to_fit() noexcept
		{
			this->for_indices<Is...>([&](auto & v) noexcept { v.shrink_to_fit(); });
		}

		template <class ... Ts> void shrink_to_fit() noexcept
		{
			this->for_types<Ts...>([&](auto & v) noexcept { v.shrink_to_fit(); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void erase(size_t const i)
		{
			this->for_tuple([&](auto & v)
			{
				v.erase(v.begin() + i);
			});
		}

		void erase(size_t const first, size_t const last)
		{
			this->for_tuple([&](auto & v)
			{
				v.erase(v.begin() + first, v.begin() + last);
			});
		}

		template <size_t ... Is> void erase(size_t const i)
		{
			this->for_indices<Is...>([&](auto & v)
			{
				v.erase(v.begin() + i);
			});
		}

		template <size_t ... Is> void erase(size_t const first, size_t const last)
		{
			this->for_indices<Is...>([&](auto & v)
			{
				v.erase(v.begin() + first, v.begin() + last);
			});
		}

		template <class ... Ts> void erase(size_t const i)
		{
			this->for_types<Ts...>([&](auto & v)
			{
				v.erase(v.begin() + i);
			});
		}

		template <class ... Ts> void erase(size_t const first, size_t const last)
		{
			this->for_types<Ts...>([&](auto & v)
			{
				v.erase(v.begin() + first, v.begin() + last);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = element_i<I>
		> auto find(U && value) noexcept -> iterator_i<I>
		{
			return std::find(this->begin<I>(), this->end<I>(), FWD(value));
		}

		template <size_t I, class U = element_i<I>
		> auto find(U && value) const noexcept -> const_iterator_i<I>
		{
			return std::find(this->cbegin<I>(), this->cend<I>(), FWD(value));
		}

		template <class T, class U = element_t<T>
		> auto find(U && value) noexcept -> iterator_t<T>
		{
			return std::find(this->begin<T>(), this->end<T>(), FWD(value));
		}

		template <class T, class U = element_t<T>
		> auto find(U && value) const noexcept -> const_iterator_t<T>
		{
			return std::find(this->cbegin<T>(), this->cend<T>(), FWD(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class Pr
		> auto find_if(Pr && pr) noexcept -> iterator_i<I>
		{
			return std::find_if(this->begin<I>(), this->end<I>(), FWD(pr));
		}

		template <size_t I, class Pr
		> auto find_if(Pr && pr) const noexcept -> const_iterator_i<I>
		{
			return std::find_if(this->cbegin<I>(), this->cend<I>(), FWD(pr));
		}

		template <class T, class Pr
		> auto find_if(Pr && pr) noexcept -> iterator_t<T>
		{
			return std::find_if(this->begin<T>(), this->end<T>(), FWD(pr));
		}

		template <class T, class Pr
		> auto find_if(Pr && pr) const noexcept -> const_iterator_t<T>
		{
			return std::find_if(this->cbegin<T>(), this->cend<T>(), FWD(pr));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = element_i<I>
		> bool binary_search(U && value) const noexcept
		{
			return std::binary_search(this->cbegin<I>(), this->cend<I>(), FWD(value));
		}

		template <class T, class U = element_t<T>
		> bool binary_search(U && value) const noexcept
		{
			return std::binary_search(this->cbegin<T>(), this->cend<T>(), FWD(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = element_i<I>
		> bool contains(U && value) const noexcept
		{
			return this->end<I>() != this->find<I>(FWD(value));
		}

		template <class T, class U = element_t<T>
		> bool contains(U && value) const noexcept
		{
			return this->end<T>() != this->find<T>(FWD(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> size_t distance(const_iterator_i<I> it) const noexcept
		{
			return (size_t)std::distance(this->cbegin<I>(), it);
		}

		template <class T> size_t distance(const_iterator_t<T> it) const noexcept
		{
			return (size_t)std::distance(this->cbegin<T>(), it);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = element_i<I>
		> size_t index_of(U && value) const noexcept
		{
			if (auto const it{ this->find<I>(FWD(value)) }; it == this->end<I>()) { return npos; }
			else { return this->distance<I>(it); }
		}

		template <class T, class U = element_t<T>
		> size_t index_of(U && value) const noexcept
		{
			if (auto const it{ this->find<T>(FWD(value)) }; it == this->end<T>()) { return npos; }
			else { return this->distance<T>(it); }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class Pr
		> size_t index_of_if(Pr && pr) const noexcept
		{
			if (auto const it{ this->find_if<I>(FWD(pr)) }; it == this->end<I>()) { return npos; }
			else { return this->distance<I>(it); }
		}

		template <class T, class Pr
		> size_t index_of_if(Pr && pr) const noexcept
		{
			if (auto const it{ this->find_if<T>(FWD(pr)) }; it == this->end<T>()) { return npos; }
			else { return this->distance<T>(it); }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class It = const_iterator_i<I>, class ... Args
		> auto emplace(It && it, Args && ... args) noexcept -> iterator_i<I>
		{
			return this->get<I>().emplace
			(
				this->get_iterator<I>(FWD(it)), FWD(args)...
			);
		}

		template <class T, class It = const_iterator_t<T>, class ... Args
		> auto emplace(It && it, Args && ... args) noexcept -> iterator_t<T>
		{
			return this->get<T>().emplace
			(
				this->get_iterator<T>(FWD(it)), FWD(args)...
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class It = const_iterator_i<I>, class U = element_i<I>
		> auto insert(It && it, U && value) noexcept -> iterator_i<I>
		{
			return this->get<I>().emplace
			(
				this->get_iterator<I>(FWD(it)), FWD(value)
			);
		}

		template <class T, class It = const_iterator_t<T>, class U = element_t<T>
		> auto insert(It && it, U && value) noexcept -> iterator_t<T>
		{
			return this->get<T>().emplace
			(
				this->get_iterator<T>(FWD(it)), FWD(value)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class U = element_i<I>
		> decltype(auto) push_back(U && value) noexcept
		{
			return this->get<I>().emplace_back(FWD(value));
		}

		template <class T, class U = element_t<T>
		> decltype(auto) push_back(U && value) noexcept
		{
			return this->get<T>().emplace_back(FWD(value));
		}

		template <class Arg0, class ... Args
		> decltype(auto) push_back(Arg0 && arg0, Args && ... args) noexcept
		{
			return this->impl_push_back<0, value_tuple, tuple_size>
			(
				value_tuple{ FWD(arg0), FWD(args)... }
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_data.swap(value.m_data);
			}
		}

		void swap(vector_tuple & value) noexcept
		{
			if (std::addressof(m_data) != std::addressof(value))
			{
				m_data.swap(value);
			}
		}

		template <size_t I, class U = vector_i<I>
		> void swap(U & value) noexcept
		{
			this->get<I>().swap(value);
		}

		template <class T, class U = element_t<T>
		> void swap(U & value) noexcept
		{
			this->get<T>().swap(value);
		}

		void swap(size_t const lhs, size_t const rhs) noexcept
		{
			this->for_tuple([&](auto & v) noexcept { util::swap(v[lhs], v[rhs]); });
		}

		template <size_t ... Is
		> void swap(size_t const lhs, size_t const rhs) noexcept
		{
			this->for_indices<Is...>([&](auto & v) noexcept { util::swap(v[lhs], v[rhs]); });
		}

		template <class ... Ts
		> void swap(size_t const lhs, size_t const rhs) noexcept
		{
			this->for_types<Ts...>([&](auto & v) noexcept { util::swap(v[lhs], v[rhs]); });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t From, size_t To
		> auto map_unchecked(element_i<From> const & from) -> element_i<To> &
		{
			return this->get<To>(this->index_of<From>(from));
		}

		template <size_t From, size_t To
		> auto map_unchecked(element_i<From> const & from) const -> element_i<To> const &
		{
			return this->get<To>(this->index_of<From>(from));
		}

		template <size_t From, class To
		> auto map_unchecked(element_i<From> const & from) -> To &
		{
			return this->get<To>(this->index_of<From>(from));
		}

		template <size_t From, class To
		> auto map_unchecked(element_i<From> const & from) const -> To const &
		{
			return this->get<To>(this->index_of<From>(from));
		}

		template <class From, size_t To
		> auto map_unchecked(From const & from) -> element_i<To> &
		{
			return this->get<To>(this->index_of<From>(from));
		}

		template <class From, size_t To
		> auto map_unchecked(From const & from) const -> element_i<To> const &
		{
			return this->get<To>(this->index_of<From>(from));
		}

		template <class From, class To
		> auto map_unchecked(From const & from) -> To &
		{
			return this->get<To>(this->index_of<From>(from));
		}

		template <class From, class To
		> auto map_unchecked(From const & from) const -> To const &
		{
			return this->get<To>(this->index_of<From>(from));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t From, size_t To
		> auto map(element_i<From> const & from) -> element_i<To> *
		{
			if (size_t const i{ this->index_of<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <size_t From, size_t To
		> auto map(element_i<From> const & from) const -> element_i<To> const *
		{
			if (size_t const i{ this->index_of<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <size_t From, class To
		> auto map(element_i<From> const & from) -> To *
		{
			if (size_t const i{ this->index_of<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <size_t From, class To
		> auto map(element_i<From> const & from) const -> To const *
		{
			if (size_t const i{ this->index_of<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <class From, size_t To
		> auto map(From const & from) -> element_i<To> *
		{
			if (size_t const i{ this->index_of<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <class From, size_t To
		> auto map(From const & from) const -> element_i<To> const *
		{
			if (size_t const i{ this->index_of<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <class From, class To
		> auto map(From const & from) -> To *
		{
			if (size_t const i{ this->index_of<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		template <class From, class To
		> auto map(From const & from) const -> To const *
		{
			if (size_t const i{ this->index_of<From>(from) }; i != npos)
			{
				return std::addressof(this->get<To>(i));
			}
			else
			{
				return nullptr;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I, class It = iterator_i<I>
		> auto get_iterator(It && it) const noexcept -> iterator_i<I>
		{
			if constexpr (std::is_scalar_v<std::decay_t<decltype(it)>>)
			{
				return this->begin<I>() + it;
			}
			else
			{
				return it;
			}
		}

		template <class T, class It = iterator_t<T>
		> auto get_iterator(It && it) const noexcept -> iterator_t<T>
		{
			if constexpr (std::is_scalar_v<std::decay_t<decltype(it)>>)
			{
				return this->begin<T>() + it;
			}
			else
			{
				return it;
			}
		}

		template <size_t I, class It = const_iterator_i<I>
		> auto get_const_iterator(It && it) const noexcept -> const_iterator_i<I>
		{
			if constexpr (std::is_scalar_v<std::decay_t<decltype(it)>>)
			{
				return this->cbegin<I>() + it;
			}
			else
			{
				return it;
			}
		}

		template <class T, class It = const_iterator_t<T>
		> auto get_const_iterator(It && it) const noexcept -> const_iterator_t<T>
		{
			if constexpr (std::is_scalar_v<std::decay_t<decltype(it)>>)
			{
				return this->cbegin<T>() + it;
			}
			else
			{
				return it;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> auto begin() noexcept { return this->get<I>().begin(); }

		template <size_t I> auto begin() const noexcept { return this->get<I>().begin(); }

		template <size_t I> auto cbegin() const noexcept { return this->get<I>().cbegin(); }

		template <class T> auto begin() noexcept { return this->get<T>().begin(); }

		template <class T> auto begin() const noexcept { return this->get<T>().begin(); }

		template <class T> auto cbegin() const noexcept { return this->get<T>().cbegin(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> auto end() noexcept { return this->get<I>().end(); }

		template <size_t I> auto end() const noexcept { return this->get<I>().end(); }

		template <size_t I> auto cend() const noexcept { return this->get<I>().cend(); }

		template <class T> auto end() noexcept { return this->get<T>().end(); }

		template <class T> auto end() const noexcept { return this->get<T>().end(); }

		template <class T> auto cend() const noexcept { return this->get<T>().cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> auto rbegin() noexcept { return this->get<I>().rbegin(); }

		template <size_t I> auto rbegin() const noexcept { return this->get<I>().rbegin(); }

		template <size_t I> auto crbegin() const noexcept { return this->get<I>().crbegin(); }

		template <class T> auto rbegin() noexcept { return this->get<T>().rbegin(); }

		template <class T> auto rbegin() const noexcept { return this->get<T>().rbegin(); }

		template <class T> auto crbegin() const noexcept { return this->get<T>().crbegin(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> auto rend() noexcept { return this->get<I>().rend(); }

		template <size_t I> auto rend() const noexcept { return this->get<I>().rend(); }

		template <size_t I> auto crend() const noexcept { return this->get<I>().crend(); }

		template <class T> auto rend() noexcept { return this->get<T>().rend(); }

		template <class T> auto rend() const noexcept { return this->get<T>().rend(); }

		template <class T> auto crend() const noexcept { return this->get<T>().crend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> auto begin_move_iterator() noexcept { return std::make_move_iterator(this->begin<I>()); }

		template <size_t I> auto begin_move_iterator() const noexcept { return std::make_move_iterator(this->begin<I>()); }

		template <size_t I> auto cbegin_move_iterator() const noexcept { return std::make_move_iterator(this->cbegin<I>()); }

		template <size_t I> auto end_move_iterator() noexcept { return std::make_move_iterator(this->end<I>()); }

		template <size_t I> auto end_move_iterator() const noexcept { return std::make_move_iterator(this->end<I>()); }

		template <size_t I> auto cend_move_iterator() const noexcept { return std::make_move_iterator(this->cend<I>()); }

		template <class T> auto begin_move_iterator() noexcept { return std::make_move_iterator(this->begin<T>()); }

		template <class T> auto begin_move_iterator() const noexcept { return std::make_move_iterator(this->begin<T>()); }

		template <class T> auto cbegin_move_iterator() const noexcept { return std::make_move_iterator(this->cbegin<T>()); }

		template <class T> auto end_move_iterator() noexcept { return std::make_move_iterator(this->end<T>()); }

		template <class T> auto end_move_iterator() const noexcept { return std::make_move_iterator(this->end<T>()); }

		template <class T> auto cend_move_iterator() const noexcept { return std::make_move_iterator(this->cend<T>()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = self_type
		> auto compare(U const & value) const noexcept
		{
			if constexpr (std::is_same_v<U, self_type>)
			{
				return this->compare(value.m_data);
			}
			else
			{
				static_assert(std::is_same_v<U, vector_tuple>);
				return ML_compare(m_data, value);
			}
		}

		template <class U = self_type
		> bool operator==(U const & value) const noexcept
		{
			return this->compare(value) == 0;
		}

		template <class U = self_type
		> bool operator!=(U const & value) const noexcept
		{
			return this->compare(value) != 0;
		}

		template <class U = self_type
		> bool operator<(U const & value) const noexcept
		{
			return this->compare(value) < 0;
		}

		template <class U = self_type
		> bool operator>(U const & value) const noexcept
		{
			return this->compare(value) > 0;
		}

		template <class U = self_type
		> bool operator<=(U const & value) const noexcept
		{
			return this->compare(value) <= 0;
		}

		template <class U = self_type
		> bool operator>=(U const & value) const noexcept
		{
			return this->compare(value) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
	protected:
		// push_back implementation
		template <size_t Idx, class Tpl, size_t Max
		> decltype(auto) impl_push_back(Tpl && value) noexcept
		{
			static_assert(self_type::tuple_size <= Max);

			if constexpr (Idx < Max)
			{
				this->push_back<Idx>(std::get<Idx>(value));

				return this->impl_push_back<Idx + 1, Tpl, Max>(FWD(value));
			}
			else
			{
				return this->back();
			}
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vector_tuple m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_PN_BATCH_HPP_
