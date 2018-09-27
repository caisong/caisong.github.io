#include <cstdio>

using namespace std;

struct SampleStruct
{
    char field_a;
    char field_b;
    int field_c;
    int field_d;
};

int main()
{
    int value = 0x12345678;
    char prefix = *(char*) &value;

    char str1[] = "123456789";
    char *str2 = "123456789";

    printf("%s\n", prefix == 0x12? "Big Endian":"Little Endian");

    SampleStruct sample1;
    printf("struct at stack memory address in order:\nFiled A:%p\n"
    "Field B%p\nField C%p\nField D%p\n", &sample1.field_a, &sample1.field_b, &sample1.field_c, &sample1.field_d);

    SampleStruct *sample2 = new SampleStruct();
    printf("struct at headp memory address in order:\nFiled A:%p\n"
    "Field B%p\nField C%p\nField D%p\n", &sample1->field_a, &sample1->field_b, &sample1->field_c, &sample1->field_d);

}