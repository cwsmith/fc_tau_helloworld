#ifndef CAPI_H_
#define CAPI_H_
extern "C"
{
int pass_by_val(int);
int multimap_stats();
void reduce(int);
}
#endif
