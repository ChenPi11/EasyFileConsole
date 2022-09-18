#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <exception>
#include <chrono>
#include "config.hpp"
long long gettime_stamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
long long start_time = gettime_stamp();
#ifdef __linux__
std::ofstream nullofs = std::ofstream("/dev/null",std::ios::out);
#else
std::ofstream nullofs = std::ofstream("nul",std::ios::out);
#endif
std::string replace(std::string str, std::string from, std::string to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
#define getexc(e) e.what()
#ifndef NULL
#define NULL 0
#endif

class Logger
{
protected:
    class _ProgessBar
    {
    public:
        std::ostream* out;
        int max = 10;
        int now = 0;
        bool flush = false;
        _ProgessBar(std::ostream& out, int max, bool flush)
        {
            this->out = &out;
            this->max = max;
            this->flush = flush;
            update(0);
        }
        void setProgress(int now)
        {
            this->now = now;
            if (this->now > this->max)
            {
                suc();
            }
            else
            {
                update(0);
            }
        }
        void back(int back = 1)
        {
            now -= back;
            if (now < 0)
                now = 0;
            update(0);
        }
        void update(int add = 1)
        {
            if (now >= max)
            {
                return;
            }
            if (now + add >= max)
            {
                now = max;
            }
            else
            {
                now += add;
            }
            int prs = int(now * 100 / max);
            std::string str = "\r[" + std::string(prs / 10, '=') + std::string(10 - prs / 10, ' ') + "] " + std::to_string(prs) + "% " + std::to_string(now) + "/" + std::to_string(max);
            if (prs >= 100)
            {
                suc();
            }
            else
            {
                std::cout << str;
            }
            if (flush)
            {
                out->flush();
            }
        }
        void suc(std::string msg = "")
        {
            std::string _str = "\r[==========] 100% " + std::to_string(now) + "/" + std::to_string(max) + ":" + msg;
            std::cerr<<_str;
            if (flush)
            {
                out->flush();
            }
        }
        void error(std::string msg = "")
        {
            std::string _str = "\r[          ] -% " + std::to_string(now) + "/" + std::to_string(max) + ":" + msg;
            std::cout<<_str;
            if (flush)
            {
                out->flush();
            }
        }
        void end()
        {
            std::cout<<std::endl;
        }
        ~_ProgessBar()
        {
            end();
        }
    };
private:
    std::ostream& console;
    std::ofstream& file;
public:
    std::string format = "[{lvl} {time}] {msg}";
    bool _flush = false;
    Logger(std::ofstream& file = nullofs, std::ostream& console = std::cerr) :console(console), file(file) {}
    void open(std::string _file)
    {
        file.close();
        std::ofstream ofs;
        ofs.open(_file, std::ios::trunc | std::ios::out);
        if (ofs.good())
        {
            file.swap(ofs);
        }
    }
    ~Logger()
    {
        if (file.is_open() && file.good())
        {
            file.close();
        }
    }
    void close()
    {
        if (file.is_open() && file.good())
        {
            file.close();
        }
    }
    void flush()
    {
        console.flush();
        if (file.is_open() && file.good())
        {
            file.flush();
        }
    }
    void write(std::string msg)
    {
        if (file.is_open() && file.good())
        {
            file << msg << "\n";
        }
        console << msg << "\n";
        if (_flush)
        {
            flush();
        }
    }
    void log(std::string lvl, std::string msg)
    {
        std::string stime = std::to_string(gettime_stamp() - start_time);
        std::string sformat = replace(format, "{lvl}", lvl);
        sformat = replace(sformat, "{time}", stime);
        sformat = replace(sformat, "{msg}", msg);
        write(sformat);
    }
#define _LOGGER_DEFINE_LEVEL(level,name) void level (std::string msg){log(name,msg);}
    _LOGGER_DEFINE_LEVEL(info,"INFO")
    _LOGGER_DEFINE_LEVEL(warn,"WARN")
    _LOGGER_DEFINE_LEVEL(error,"ERROR")
    _LOGGER_DEFINE_LEVEL(debug,"DEBUG")
    _LOGGER_DEFINE_LEVEL(fatal,"FATAL")
    _LOGGER_DEFINE_LEVEL(exception,"EXCEPTION")
    _LOGGER_DEFINE_LEVEL(fail,"FAIL")
    _LOGGER_DEFINE_LEVEL(suc,"SUC")
    _LOGGER_DEFINE_LEVEL(suc_critical,"SUCC")
    _LOGGER_DEFINE_LEVEL(critical,"CRITICAL")
    void exception()
    {
        try
        {
            std::rethrow_exception(std::current_exception());
        }
        catch (std::exception& e)
        {
            exception(getexc(e));
        }
    }
};
Logger Log;
#endif