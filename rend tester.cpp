#include <DataFame/RandGen.h>

#include <cassert>
#include <iostream>

using namespace hmdf;

int main(int, char *[]) {

    {
        RandGenParams<long> p;

        p.min_value = 0;
        p.max_value = 1000000;

        gen_uniform_int_dist<long>(1024, p);
    }

    {
        RandGenParams<double> p;

        p.min_value = 0;
        p.max_value = 2.0;
        p.seed = 23;

        gen_uniform_real_dist<double>(1024, p);
    }

    {
        gen_bernouilli_dist(1024);
    }

    {
        RandGenParams<int> p;

        p.t_dist = 1000;

        gen_binominal_dist<int>(1024, p);
    }

    {
        RandGenParams<int> p;

        p.t_dist = 1000;

        gen_negative_binominal_dist<int>(1024, p);
    }

    <
}