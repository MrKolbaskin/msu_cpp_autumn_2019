#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

size_t to_num(std::string& x)
{
	size_t res;
	std::stringstream ss;
	ss << x;
	ss >> res;
	return res;
}

template <class T>
std::string to_string1(T&& x){
	std::stringstream ss;
	ss << x;
	return ss.str();
}


template <class... argsT>
std::string format(const std::string& str, argsT&&... obj)
{
	std::vector<std::string> args{to_string1(std::forward<argsT>(obj))...};
	std::ostringstream res;

	for (size_t i = 0; i < str.length(); ++i){
		if (str[i] == '{'){
			i++;
			if (str[i] == '}'){
				throw std::runtime_error("No_index");
			}
			size_t a;
			std::string tmp_num;
			while(str[i] != '}' && i < str.length()){
				if (std::isdigit(str[i])){
					tmp_num += str[i];
					++i;
				} else {
					throw std::runtime_error("Wrong_index");
				}
			}
			if (i == str.length()){
				throw std::runtime_error("Wrong_format");
			}
			a = to_num(tmp_num);
			if (a < args.size()){
				res << args[a];
			} else {
				throw std::runtime_error("Big_index");
			}
		} else {
			if (str[i] == '}'){
				throw std::runtime_error("Wrong_format");
			}
			res << str[i];
		}
		
	}
	return res.str();
}