#pragma once
#include <stdlib.h>
#include <functional>

template<typename RT, typename... Args>
using PFunc = RT(*)(Args...);

template<typename RT, typename O, typename... Args>
using PFuncM = RT(O::*)(Args...);

template<typename RT, typename O, typename... Args>
using PFuncMC = RT(O::*)(Args...) const;

template<typename RT, typename... Args>
class Function
{
public:
	using FUNC = std::function<RT(Args...)>;

	template<typename O>
	using FUNCM = std::function<RT(O&, Args...)>;

	//using PFUNC = PFunc<RT, Args...>;
	//template<typename O> using PFUNCM = PFuncM<RT, O, Args...>;
	//template<typename O> using PFUNCMC = PFuncMC<RT, O, Args...>;

	Function(FUNC func)
		:
		action([func](Args&&... args)->RT {return func(std::forward<Args>(args)...); })
	{}
	template<typename O>
	Function(FUNCM<O> func, O& o)
		:
		action([func, &o](Args&&... args)->RT {return func(o, std::forward<Args>(args)...); })
	{}
	template<typename O>
	Function(FUNCM<O> func, O* o)
		:
		action([func, o](Args&&... args)->RT {return func(*o, std::forward<Args>(args)...); })
	{}

	operator bool() const
	{
		return action.operator bool();
	}

	RT operator()(Args... args) const
	{
		return action(std::forward<Args>(args)...);
	}

private:
	FUNC action;
};


template<typename RT, typename... Args>
class FunctionS
{
public:
	using FUNC = std::function<RT(Args...)>;

	template<typename O>
	using FUNCM = std::function<RT(O&, Args...)>;

	//using PFUNC = PFunc<RT, Args...>;
	//template<typename O> using PFUNCM = PFuncM<RT, O, Args...>;
	//template<typename O> using PFUNCMC = PFuncMC<RT, O, Args...>;

	FunctionS(FUNC func)
		:
		action([func]()->RT {return (func(std::forward<Args>(args)...)); })
	{}
	template<typename O>
	FunctionS(FUNCM<O> func, O& o, Args&&... args)
		:
		action([func, &o, args...]()->RT {return func(o, std::forward<Args>(args)...); })
	{}
	template<typename O>
	FunctionS(FUNCM<O> func, O* o, Args&&... args)
		:
		action([func, o, args...]()->RT {return func(o, std::forward<Args>(args)...); })
	{}

	operator bool() const
	{
		return action.operator bool();
	}

	RT operator()() const
	{
		return action();
	}

private:
	std::function<RT()> action;
};
