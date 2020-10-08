#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>

/**
 * @brief Tool for IO, STL, ...
 * will be included by many
 *
 * @file Utils.h
 * @author Bowen Zhang
 * @date 2020/10/03
 */

using std::cout;
using std::cin;
using std::string;
using std::domain_error;
using std::istringstream;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::steady_clock;
using std::chrono::milliseconds;


namespace SP
{
    class IO {
    public:
        /**
         * @brief Get integer (the index of excercise) from user
         */
        static int getInteger(const std::string& prompt = "请输入要测试的练习序号：",
                       const std::string& reprompt = "请重试（必须输入整数）！\n")
        {
            while (1)
            {
                cout << prompt;
                string line;
                if (!getline(cin, line)) throw domain_error("Failed to get line from cin.");

                istringstream iss(line);
                int i; char a;
                if (iss >> i && !(iss >> a))
                {
                    return i;
                }
                cout << reprompt;
            }
            return INT_MIN;
        }

        /**
         * @brief Ask continue or not
         */
        static bool doContinue()
        {
            cout << "继续？[Y/n] ";
            string line;

            while (true)
            {
                if (!getline(cin, line)) throw domain_error("Failed to get line from cin.");
                istringstream iss(line);
                char a; char remain;

                if (iss >> a && !(iss >> remain)) {
                    if (std::tolower(a) == 'y')
                    {
                        return true;
                    }
                    else if (std::tolower(a) == 'n')
                    {
                        return false;
                    }
                }
                std::cerr << "请输入 Y/y 或 N/n: ";
            }
        }

        /**
         * @brief print function
         */
        static void print(const char* fmt)
        {
            cout << fmt;
        }

        static void println(const char* fmt)
        {
            IO::print(fmt);
            IO::print("\n");
        }

        template<typename T, typename ... Targs>
        static void print(const char* fmt, T value, Targs... Fargs)
        {
            for (; *fmt != '\0'; fmt++)
            {
                if (*fmt == '%') {
                    cout << value;
                    IO::print(fmt+1, Fargs...);
                    return;
                }
                cout << *fmt;
            }
        }

        template<typename T, typename ... Targs>
        static void println(const char* fmt, T value, Targs... Fargs)
        {
            IO::print(fmt, value, Fargs...);
            IO::print("\n");
        }
    };

    class STL
    {
    public:
        /**
         * @brief Clear up vector of pointers
         */
        template<typename T>
        static void ClearUp(std::vector<T>& vec)
        {
            std::for_each(vec.begin(), vec.end(), [](T& p){ delete p; p = nullptr; });
            vec.clear();
        }
        /**
         * @brief Measure function runtime
         */
        template<typename Tfunc, typename ...Targs>
        static long long FunctionRunTime(Tfunc f, Targs... args)
        {
            auto t1 = steady_clock::now();
            f(args...);
            auto t2 = steady_clock::now();
            return duration_cast<milliseconds>(t2-t1).count();
        }

        static string GetFilePath(const std::string& sFileName)
        {
            return string(getenv("RES_PATH")) + sFileName;
        }
    };
}

#endif // UTILS_H
