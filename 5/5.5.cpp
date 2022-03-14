#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true,std::memory_order_relaxed);
    y.store(true,std::memory_order_relaxed);
}

void read_y_then_x()
{
    while(!y.load(std::memory_order_relaxed));
    if(x.load(std::memory_order_relaxed))
        ++z;
}

int main(int argc, char **argv)
{
    int loops = 100000;
    int counts_0 = 0, counts_1 = 0, counts_2 = 0;
    if(argc == 2) {
        loops = std::atoi(argv[1]);
    }

    for(int i = 0; i < loops; i++) {
        x=false;
        y=false;
        z=0;
        std::thread a(write_x_then_y);
        std::thread b(read_y_then_x);
        a.join();
        b.join();

        if(z == 0) {
            counts_0 += 1;
        } else if(z == 1) {
            counts_1 += 1;
        } else {
            counts_2 += 1;
        }
    }

    printf("z=0: %d, z=1: %d, z=2: %d\n", counts_0, counts_1, counts_2);
    
    return 0;
}