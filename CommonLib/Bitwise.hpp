#pragma once
#if 0
template <class Enum, class Underlying = std::underlying_type_t<Enum>>
class Bitwise
{
public:
	using enum_type = Enum;
	using underlying_type = Underlying;

	static_assert(std::is_enum_v<enum_type>, "enum_type must be an enum");
	static_assert(std::is_integral_v<underlying_type>, "underlying_type must be an integral");

	constexpr Bitwise() noexcept = default;

	constexpr Bitwise(const Bitwise&) noexcept = default;

	constexpr Bitwise(Bitwise&&) noexcept = default;

	template <class U2>  // NOLINTNEXTLINE(google-explicit-constructor)
	constexpr Bitwise(Bitwise<Enum, U2> a_rhs) noexcept :
		_impl(static_cast<underlying_type>(a_rhs.get()))
	{
	}

	template <class... Args>
	constexpr Bitwise(Args... a_values) noexcept  //
		requires(std::same_as<Args, enum_type>&&...) :
		_impl((static_cast<underlying_type>(a_values) | ...))
	{
	}

	~Bitwise() noexcept = default;

	constexpr Bitwise& operator=(const Bitwise&) noexcept = default;
	constexpr Bitwise& operator=(Bitwise&&) noexcept = default;

	template <class U2>
	constexpr Bitwise& operator=(Bitwise<Enum, U2> a_rhs) noexcept
	{
		_impl = static_cast<underlying_type>(a_rhs.get());
		return *this;
	}

	constexpr Bitwise& operator=(enum_type a_value) noexcept
	{
		_impl = static_cast<underlying_type>(a_value);
		return *this;
	}

	template <class U2>
	constexpr Bitwise& operator&=(Bitwise<Enum, U2> a_rhs) noexcept
	{
		_impl &= static_cast<underlying_type>(a_rhs.get());
		return *this;
	}

	constexpr Bitwise& operator&=(enum_type a_value) noexcept
	{
		_impl &= static_cast<underlying_type>(a_value);
		return *this;
	}

	template <class U2>
	constexpr Bitwise& operator|=(Bitwise<Enum, U2> a_rhs) noexcept
	{
		_impl |= static_cast<underlying_type>(a_rhs.get());
		return *this;
	}

	constexpr Bitwise& operator|=(enum_type a_value) noexcept
	{
		_impl |= static_cast<underlying_type>(a_value);
		return *this;
	}

	template <class U2>
	constexpr enum_type operator&(Bitwise<Enum, U2> a_rhs) const noexcept
	{
		return _impl & static_cast<underlying_type>(a_rhs.get());
	}

	constexpr enum_type operator&(enum_type a_value) const noexcept
	{
		return _impl & static_cast<underlying_type>(a_value);
	}

	template <class U2>
	constexpr enum_type operator|(Bitwise<Enum, U2> a_rhs) const noexcept
	{
		return _impl | static_cast<underlying_type>(a_rhs.get());
	}

	constexpr enum_type operator|(enum_type a_value) const noexcept
	{
		return _impl | static_cast<underlying_type>(a_value);
	}


	[[nodiscard]] explicit constexpr operator bool() const noexcept { return _impl != static_cast<underlying_type>(0); }

	[[nodiscard]] constexpr enum_type	   operator*() const noexcept { return get(); }
	[[nodiscard]] constexpr enum_type	   get() const noexcept { return static_cast<enum_type>(_impl); }
	[[nodiscard]] constexpr underlying_type underlying() const noexcept { return _impl; }

	template <class... Args>
	constexpr Bitwise& set(Args... a_args) noexcept  //
		requires(std::same_as<Args, enum_type>&&...)
	{
		_impl |= (static_cast<underlying_type>(a_args) | ...);
		return *this;
	}

	template <class... Args>
	constexpr Bitwise& reset(Args... a_args) noexcept  //
		requires(std::same_as<Args, enum_type>&&...)
	{
		_impl &= ~(static_cast<underlying_type>(a_args) | ...);
		return *this;
	}

	template <class... Args>
	[[nodiscard]] constexpr bool any(Args... a_args) const noexcept  //
		requires(std::same_as<Args, enum_type>&&...)
	{
		return (_impl & (static_cast<underlying_type>(a_args) | ...)) != static_cast<underlying_type>(0);
	}

	template <class... Args>
	[[nodiscard]] constexpr bool all(Args... a_args) const noexcept  //
		requires(std::same_as<Args, enum_type>&&...)
	{
		return (_impl & (static_cast<underlying_type>(a_args) | ...)) == (static_cast<underlying_type>(a_args) | ...);
	}

	template <class... Args>
	[[nodiscard]] constexpr bool none(Args... a_args) const noexcept  //
		requires(std::same_as<Args, enum_type>&&...)
	{
		return (_impl & (static_cast<underlying_type>(a_args) | ...)) == static_cast<underlying_type>(0);
	}

private:
	underlying_type _impl{ 0 };
};

template <class... Args>
Bitwise(Args...) -> Bitwise<
	std::common_type_t<Args...>,
	std::underlying_type_t<
	std::common_type_t<Args...>>>;

#endif