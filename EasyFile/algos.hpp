#ifndef _ALGOS_HPP_
#define _ALGOS_HPP_
#include <iostream>
#include <vector>

#define _substring(s,start,end) s.substr(start,end-start)
template <typename T> std::vector<std::basic_string<T>> split(const std::basic_string<T>& v, const std::basic_string<T>& sep)//nice
{
	std::vector<std::basic_string<T>> res;
	size_t last{};
	size_t f{};
	if (sep.size() == 0)return { v };//empty separator
	if (v.find(sep) == std::basic_string<T>::npos)
	{
		return { v };//separator not in string
	}
	try
	{
		while ((f = v.find(sep, (last == 0 ? 0 : last + sep.size()))) != std::basic_string<T>::npos)
		{
			res.push_back(_substring(v, (last == 0 ? 0 : last + sep.size()), f));
			last = f;
		}
		res.push_back(v.substr(last + sep.size()));
	}
	catch (...)
	{
		return { v };//unknown bugs
	}
	return res;//success return
}
template <typename T> std::vector<std::basic_string<T>> split(const T* _v, const T* _sep)//C input
{
	std::basic_string<T> v = _v, sep = _sep;
	std::vector<std::basic_string<T>> res;
	size_t last{};
	size_t f{};
	if (sep.size() == 0)return { v };//empty separator
	if (v.find(sep) == std::basic_string<T>::npos)
	{
		return { v };//separator not in string
	}
	try
	{
		while ((f = v.find(sep, (last == 0 ? 0 : last + sep.size()))) != std::basic_string<T>::npos)
		{
			res.push_back(_substring(v, (last == 0 ? 0 : last + sep.size()), f));
			last = f;
		}
		res.push_back(v.substr(last + sep.size()));
	}
	catch (...)
	{
		return { v };//unknown bugs
	}
	return res;//success return
}
template <typename T> std::vector<std::basic_string<T>> split(const std::basic_string<T>& v, const T* sep)
{
	return split(v, std::basic_string<T>(sep));
}
template <typename T> std::vector<std::basic_string<T>> split(const T* v, const std::basic_string<T>& sep)
{
	return split(std::basic_string<T>(v), sep);
}

template<typename T>
constexpr auto endwith(std::basic_string<T> s, std::basic_string<T> c) { return (s.find_last_of(c) + c.size() == s.size()); }
template<typename T>
constexpr auto startwith(std::basic_string<T> s, std::basic_string<T> c) { return (s.find_first_of(c)==0); }
#endif