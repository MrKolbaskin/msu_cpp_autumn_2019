#pragma once


enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
    std::ostream& out;

    template <class T, class... ArgsT>
    Error process(T x, ArgsT... args){
        if constexpr (sizeof...(args) == 0){
            return save_arg(x);
        } else {
            save_arg(x);
            return process(args...);
        }
    }
    
    template<class T>
    Error process(T x){
        save_arg(x);
        return Error::NoError;
    }
    
    Error save_arg(bool x)
    {
        out << (x? "true": "false") << Separator;
        return Error::NoError;
    }

    Error save_arg(uint64_t x)
    {
        out << x << Separator;
        return Error::NoError;
    }
public:
    template <class T>
    Error save(T& object){
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args){
        return process(args...);
    }

    explicit Serializer(std::ostream& x):out(x){}
};


class Deserializer
{
    std::istream& in;

    template <class T, class... ArgsT>
    Error process(T &x, ArgsT&... args)
    {
        if (sizeof...(args) == 0){
            return get_value(x);
        }
        if (get_value(x) == Error::CorruptedArchive){
            return Error::CorruptedArchive;
        }
        return process(args...);
    }

    template<class T>
    Error process(T &x){
        return get_value(x);
    }

    Error get_value(bool &x){
        std::string val;
        in >> val;
        if (val == "true"){
            x = true;
        } else if (val == "false"){
            x = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
    
    Error get_value(uint64_t &x){
        std::string val;
        in >> val;
        if (!isdigit(val[0])){
            return Error::CorruptedArchive;
        } else {
            x = std::stoull(val);
            return Error::NoError;
        }
    }
public:
    template <class T>
    Error load(T& object){
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args){
        return process(args...);
    }

    explicit Deserializer(std::istream& x):in(x){}
};
