#pragma once
#include <stdlib.h>
#include <functional>

enum CallingConvention { CDECLR, STDCALL, FASTCALL, THISCALL };
template<CallingConvention> struct CCHelper;

template<> struct CCHelper<CDECLR>
{
	template<typename RT, typename... Args>
	using PFunc = RT(__cdecl *)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncM = RT(__cdecl O::*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncMC = RT(__cdecl O::*)(Args...) const;
};
template<> struct CCHelper<STDCALL>
{
	template<typename RT, typename... Args>
	using PFunc = RT(__stdcall *)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncM = RT(__stdcall O::*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncMC = RT(__stdcall O::*)(Args...) const;
};
template<> struct CCHelper<FASTCALL>
{
	template<typename RT, typename... Args>
	using PFunc = RT(__fastcall *)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncM = RT(__fastcall O::*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncMC = RT(__fastcall O::*)(Args...) const;
};
template<> struct CCHelper<THISCALL>
{
	template<typename RT, typename... Args>
	using PFunc = RT(__thiscall *)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncM = RT(__thiscall O::*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncMC = RT(__thiscall O::*)(Args...) const;
};

template<CallingConvention CC, typename RT, typename... Args>
using PFunc = typename CCHelper<CC>::template PFunc<RT, Args...>;

template<CallingConvention CC, typename RT, typename O, typename... Args>
using PFuncM = typename CCHelper<CC>::template PFuncM<RT, O, Args...>;

template<CallingConvention CC, typename RT, typename O, typename... Args>
using PFuncMC = typename CCHelper<CC>::template PFuncMC<RT, O, Args...>;

template <typename FUNC>
struct is_function_ptr
	:
	std::integral_constant<bool, std::is_pointer<FUNC>::value &&
	std::is_function<std::remove_pointer_t<FUNC>>::value>
{};

template<typename RT, typename... Args>
class Function
{
public:
	template<typename FUNC>
	Function(FUNC&& func, typename std::enable_if_t<!is_function_ptr<FUNC>::value>* = nullptr)
		: action([func](Args&&... args)->RT {return func(std::forward<Args>(args)...); })
	{}

	template<typename FUNC>
	Function(FUNC&& func, typename std::enable_if_t<is_function_ptr<FUNC>::value>* = nullptr)
		: action([func](Args&&... args)->RT {return (*func)(std::forward<Args>(args)...); })
	{}

	template<typename FUNC, typename O>
	Function(FUNC&& func, O* o, typename std::enable_if_t<std::is_member_function_pointer<FUNC>::value>* = nullptr)
		: action([func, o](Args&&... args)->RT {return (o->*func)(std::forward<Args>(args)...); })
	{}

	template<typename FUNC, typename O>
	Function(FUNC&& func, O& o, typename std::enable_if_t<std::is_member_function_pointer<FUNC>::value>* = nullptr)
		: action([func, &o](Args&&... args)->RT {return (o.*func)(std::forward<Args>(args)...); })
	{}

	operator bool() const
	{
		return action.operator bool();
	}

	auto operator()(Args... args) const -> RT
	{
		return action(std::forward<Args>(args)...);
	}

private:
	std::function<RT(Args...)> action;
};

template<typename RT>
class FunctionS
{
public:
	template<typename FUNC, typename... Args>
	FunctionS(FUNC&& func, Args&&... args, typename std::enable_if_t<!is_function_ptr<FUNC>::value>* = nullptr)
		: action([func, args...]()->RT {return func(std::forward<Args>(args)...); })
	{}

	template<typename FUNC, typename... Args>
	FunctionS(FUNC&& func, Args&&... args, typename std::enable_if_t<is_function_ptr<FUNC>::value>* = nullptr)
		: action([func, args...]()->RT {return (*func)(std::forward<Args>(args)...); })
	{}

	template<typename FUNC, typename O, typename... Args>
	FunctionS(FUNC&& func, O* o, Args&&... args, typename std::enable_if_t<std::is_member_function_pointer<FUNC>::value>* = nullptr)
		: action([func, o, args...]()->RT {return (o->*func)(std::forward<Args>(args)...); })
	{}

	template<typename FUNC, typename O, typename... Args>
	FunctionS(FUNC&& func, O& o, Args&&... args, typename std::enable_if_t<std::is_member_function_pointer<FUNC>::value>* = nullptr)
		: action([func, &o, args...]()->RT {return (o.*func)(std::forward<Args>(args)...); })
	{}

	operator bool() const
	{
		return action.operator bool();
	}

	auto operator()() const -> RT
	{
		return action();
	}

private:
	std::function<RT()> action;
};