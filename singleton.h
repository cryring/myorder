#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
public:
    static T* instance()
    {
        static T instance;
        return &instance;
    }

protected:
    Singleton(){}

    ~Singleton(){}
};

#endif // SINGLETON_H
