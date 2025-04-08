#pragma once

template <typename ArgType, typename RetType>
class Function
{
protected:
    RetType (*function)(ArgType);
    int counter;

public:
    Function(){
        counter = 0;
    }
    void set_function(RetType (*funct)(ArgType))
    {
        function = funct;
    }

    void reset_counter()
    {
        counter = 0;
    }

    int get_counter()
    {
        return counter;
    }

    RetType get_value(ArgType x)
    {
        counter += 1;
        return function(x);
    }
};
