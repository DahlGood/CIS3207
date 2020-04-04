#include <vector>

class CBuffer {
    public:
        CBuffer(int size);
        void slide(int value);
        int snatch();
        int peep(int index);
        int size();
    private:
        std::vector<int> buff;
        unsigned int slideIndex;
        unsigned int snatchIndex;
        unsigned int currentSize;

};