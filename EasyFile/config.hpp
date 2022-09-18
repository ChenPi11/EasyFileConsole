#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_
#include "../CJsonObject/CJsonObject.hpp"
#include "algos.hpp"
#include "encoding.hpp"
#include <fstream>
#include <map>
namespace config
{
	std::string _parse_str(std::string p)//'"hello"' -> 'hello'
	{
		if (startwith(p, std::string("\"")) && endwith(p, std::string("\"")))
		{
			return p.substr(1,p.size()-2);
		}
		else
		{
			return p;
		}
	}
	neb::CJsonObject cfg;
	std::map<std::string,std::string> strs;
	namespace defaultstrs
	{

	}
	namespace strings
	{
		inline std::string get(std::string k)
		{
			if (strs.count(k) == 0)//k not in map
			{
				return k;
			}
			return strs[k];
		}
		inline std::wstring get(std::wstring k)
		{
			std::string s_k;//string key
			std::wstring res;
			gconvert::uni2ansi(k, s_k);
			if (strs.count(s_k) == 0)//k not in map
			{
				gconvert::utf82uni(s_k, res);
				return res;
			}
			gconvert::utf82uni(s_k, res);
			return res;
		}
	}
	void load(std::string file)
	{
		std::ifstream ifs;
		ifs.open(file);
		if(ifs.good())
		{
			std::string dt;
			ifs>>dt;
			cfg.Parse(dt);
			std::string key="langpack",val;
			if(cfg.Get(key,val))
			{
				std::ifstream lifs;
				lifs.open(val);
				if(lifs.good())
				{
					std::string line;
					while(std::getline(lifs,line).good())
					{
						auto v=split(line, "=");
						strs.emplace(v[0], _parse_str(v[1]));
					}
					lifs.close();
				}
				else
				{
					lifs.close();
					goto LIFS_ERR;
				}
			}
			else
			{
				LIFS_ERR:
				std::cerr<<"WARNING: Load lang file error.\n";
			}
		}
		else
		{
			std::cerr<<"WARNING: Load config file error.\n";
		}
		ifs.close();
	}
}

#endif