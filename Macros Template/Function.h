#pragma once
#include <stdlib.h>
#include <functional>

template<typename Rtn, typename... Args>
using PFunc = Rtn(*)(Args...);

template<typename Rtn, typename O, typename... Args>
using PMFunc = Rtn(O::*)(Args...);

template<typename Rtn, typename O, typename... Args>
using PMFuncC = Rtn(O::*)(Args...) const;

template<typename RT>
class Function
{
public:
	template<class Func, typename... Args> 
	Function(Func func, Args&&... args)
		:
		action( [ func, args... ]()->RT{return ( RT )( func( std::move( args )... ) ); } )
	{
	}
	template<class O, typename Func, typename... Args> 
	Function(O& o, Func(O::*func), Args&&... args)
		:
		action( [ &o, func, args... ]()->RT{return ( RT )( ( o.*func )( std::move( args )... ) ); } )
	{
	}
	template<class O, typename Func, typename... Args> 
	Function(O* o, Func(O::*func), Args&&... args)
		:
		action( [ o, func, args... ]()->RT{return ( RT )( ( o->*func )( std::move( args )... ) ); } )
	{
	}

	RT operator()()
	{
		return action();
	}

private:
	std::function<RT()> action;
};

using Action = Function<void>;
