#include <iostream>

class LinearAllocator
{
    char* begin;
    size_t maxSz;
    char* current;
public:
    LinearAllocator(size_t& maxSize): maxSz(maxSize)
    {
        if ((begin = (char*) malloc(maxSize)) == nullptr){
            throw std::bad_alloc();
        }
        current = begin;
    }

    ~LinearAllocator(){
        if (begin != nullptr){
            free(begin);
        }
    }
    char* alloc(size_t& size)
    {
        if (current - begin + size > maxSz){
            return nullptr;
        }
        char* tmp = current;
        current += size;
        return tmp;
    }

    void reset()
    {
        current = begin;
    }
};

int main(int argc, char **argv)
{
    try
    {
        size_t Maxsz = (size_t)atoll(argv[1]);
        if (Maxsz == 0){
            std::cout << "Wrong input" << std::endl;
            return 1;
        }
        LinearAllocator al(Maxsz);
        size_t sz = 0;
        char* tmp;
        
        for (int i = 2; i < argc; ++i){
            if (!strcmp(argv[i], "alloc")){
                ++i;
                sz = (size_t)atoll(argv[i]);
                if (sz == 0){
                    std::cout << "Wrong input" << std::endl;
                    return 1;
                }
                tmp = al.alloc(sz);
                if (tmp == nullptr){
                    std::cout << "Bad alloc" << std::endl;
                    return 1;
                }
                continue;
            }
            if (!strcmp(argv[i], "reset")){
                al.reset();
            } else {
                std::cout << "Wrong input" << std::endl;
                return 1;
            }
         }

    }
    catch (std::bad_alloc& x){
        std::cout << x.what() << std::endl;
        return 1;
    }

    return 0;
}
