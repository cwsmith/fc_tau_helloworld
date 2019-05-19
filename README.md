## Install Tau on Cori

- Download the TAUv2.28.1 from tau.uoregon.edu/tau.tgz
- Unpack in your home
- Configure and install with

```bash
module unload darshan
./configure -arch=craycnl  -bfd=download -dwarf=download  -iowrapper -unwind=download -mpi; make install
```

- Create an environment script for using Tau, `envTau.sh`, with the following
  contents (adjust the path to tau bin dir):

```bash
module unload darshan
export PATH=/path/to/tau-2.28.1/craycnl/bin:$PATH 
```

## Build, Run, and Profile a C++ Example

```bash
CC -g -dynamic zdravo.cc -o zdravo
salloc -N 1 -q debug -C knl -t 00:10:00
srun -n 1 tau_exec -ebs ./zdravo
```

This produces `profile.0.0.0`.

## Build, Run, and Profile Fortran/C Example

```bash
make clean
make
salloc -N 1 -q debug -C knl -t 00:10:00
srun -n 5 tau_exec -ebs ./fc_simple 
```

This produces profile files one for each rank.

## Viewing/generating various profiles

- For non-GUI viewing see `pprof --help` and `paraprof --help` for most up to date options. To view detailed location information for merged profiles sorted by time of exclusive functions (ones that can't be further subdivided):
```bash
paraprof --pack profile.ppk
pprof -am profile.ppk
```

- For GUI, open a `.ppk` or `profile.` file in paraprof GUI application.

## Profiling options with `tau_exec`

- To see just the MPI profile (only MPI functions): `srun -n 5 tau_exec ./fc_simple`

For 5 ranks here it gives something like:

```
FUNCTION SUMMARY (mean):
---------------------------------------------------------------------------------------
%Time    Exclusive    Inclusive       #Call      #Subrs  Inclusive Name
              msec   total msec                          usec/call 
---------------------------------------------------------------------------------------
100.0       28,019       28,160           1         105   28160572 .TAU application
  0.3           77           77           1           0      77646 MPI_Finalize() 
  0.2           57           57           1           0      57283 MPI_Init() 
  0.0            6            6         100           0         66 MPI_Allreduce() 
  0.0       0.0206       0.0206           3           0          7 MPI_Comm_rank() 
```
Where we can assume that `.TAY application` refers to the whole code.

- To see functions: `srun -n 5 tau_exec -ebs -ebs_resolution=function ./fc_simple`

```
FUNCTION SUMMARY (mean):
---------------------------------------------------------------------------------------
%Time    Exclusive    Inclusive       #Call      #Subrs  Inclusive Name
              msec   total msec                          usec/call
---------------------------------------------------------------------------------------
100.0       27,977       33,508           1         105   33508939 .TAU application
 64.8       21,720       21,720       434.4           0      50000 .TAU application => [CONTEXT] .TAU application => [SAMPLE] pass_by_val [{/global/u2/a/agntt/source_code/fc_tau_helloworld/capi.cc} {10}]
 64.8       21,720       21,720       434.4           0      50000 [SAMPLE] pass_by_val [{/global/u2/a/agntt/source_code/fc_tau_helloworld/capi.cc} {10}]
 16.3        5,466        5,466           1           0    5466531 MPI_Finalize()
 12.4        4,170        4,170        83.4           0      50000 .TAU application => [CONTEXT] .TAU application => [SAMPLE] std::_Rb_tree_increment(std::_Rb_tree_node_base*) [clone .localalias.2] [{/b/tmp/peint/build-cray-gcc-20180126.202237.011353000/cray-gcc/BUILD/snos_objdir/x86_64-suse-linux/libstdc++-v3/src/c++98/../../../../../cray-gcc-7.3.0-201801270210.d61239fc6000b/libstdc++-v3/src/c++98/tree.cc} {0}]
 12.4        4,170        4,170        83.4           0      50000 [SAMPLE] std::_Rb_tree_increment(std::_Rb_tree_node_base*) [clone .localalias.2] [{/b/tmp/peint/build-cray-gcc-20180126.202237.011353000/cray-gcc/BUILD/snos_objdir/x86_64-suse-linux/libstdc++-v3/src/c++98/../../../../../cray-gcc-7.3.0-201801270210.d61239fc6000b/libstdc++-v3/src/c++98/tree.cc} {0}]
  4.1        1,370        1,370        27.4           0      50000 .TAU application => [CONTEXT] .TAU application => [SAMPLE] multimap_stats [{/global/u2/a/agntt/source_code/fc_tau_helloworld/capi.cc} {59}]
```
For some reason some functions are listed twice. This shows the line in the faile where the functions starts. `MPI_Finalize` is a bit odd due to above results. 

- To see lines: `srun -n 5 tau_exec -ebs -ebs_resolution=lines ./fc_simple`

```
FUNCTION SUMMARY (mean):
---------------------------------------------------------------------------------------
%Time    Exclusive    Inclusive       #Call      #Subrs  Inclusive Name
              msec   total msec                          usec/call
---------------------------------------------------------------------------------------
100.0       27,870       28,028           1         105   28028792 .TAU application
 68.7       19,259       19,259       385.2           0      50000 .TAU application => [CONTEXT] .TAU application => [SAMPLE] pass_by_val [{/global/u2/a/agntt/source_code/fc_tau_helloworld/capi.cc} {43}]
 68.7       19,259       19,259       385.2           0      50000 [SAMPLE] pass_by_val [{/global/u2/a/agntt/source_code/fc_tau_helloworld/capi.cc} {43}]
 10.7        3,000        3,000          60           0      50000 .TAU application => [CONTEXT] .TAU application => [SAMPLE] std::_Rb_tree_increment(std::_Rb_tree_node_base*) [clone .localalias.2] [{/b/tmp/peint/build-cray-gcc-20180126.202237.011353000/cray-gcc/BUILD/snos_objdir/x86_64-suse-linux/libstdc++-v3/src/c++98/../../../../../cray-gcc-7.3.0-201801270210.d61239fc6000b/libstdc++-v3/src/c++98/tree.cc} {65}]
 10.7        3,000        3,000          60           0      50000 [SAMPLE] std::_Rb_tree_increment(std::_Rb_tree_node_base*) [clone .localalias.2] [{/b/tmp/peint/build-cray-gcc-20180126.202237.011353000/cray-gcc/BUILD/snos_objdir/x86_64-suse-linux/libstdc++-v3/src/c++98/../../../../../cray-gcc-7.3.0-201801270210.d61239fc6000b/libstdc++-v3/src/c++98/tree.cc} {65}]
  8.4        2,350        2,350          47           0      50000 .TAU application => [CONTEXT] .TAU application => [SAMPLE] pass_by_val [{/global/u2/a/agntt/source_code/fc_tau_helloworld/capi.cc} {42}]
  8.4        2,350        2,350          47           0      50000 [SAMPLE] pass_by_val [{/global/u2/a/agntt/source_code/fc_tau_helloworld/capi.cc} {42}]
  5.1        1,420        1,420        28.4           0      50000 .TAU application => [CONTEXT] .TAU application => [SAMPLE] multimap_stats [{/global/u2/a/agntt/source_code/fc_tau_helloworld/capi.cc} {79}]
  5.1        1,420        1,420        28.4           0      50000 [SAMPLE] multimap_stats [{/global/u2/a/agntt/source_code/fc_tau_helloworld/capi.cc} {79}]
  1.9          529          529        10.6           0      50000 .TAU application => [CONTEXT] .TAU application => [SAMPLE] std::_Rb_tree_increment(std::_Rb_tree_node_base*) [clone .localalias.2] [{/b/tmp/peint/build-cray-gcc-20180126.202237.011353000/cray-gcc/BUILD/snos_objdir/x86_64-suse-linux/libstdc++-v3/src/c++98/../../../../../cray-gcc-7.3.0-201801270210.d61239fc6000b/libstdc++-v3/src/c++98/tree.cc} {71}]
  1.9          529          529        10.6           0      50000 [SAMPLE] std::_Rb_tree_increment(std::_Rb_tree_node_base*) [clone .localalias.2] [{/b/tmp/peint/build-cray-gcc-20180126.202237.011353000/cray-gcc/BUILD/snos_objdir/x86_64-suse-linux/libstdc++-v3/src/c++98/../../../../../cray-gcc-7.3.0-201801270210.d61239fc6000b/libstdc++-v3/src/c++98/tree.cc} {71}]
```

- `TAU_CALLPATH` is an option to view the function chain that called a particular function, chain length is adjusted with `TAU_CALLPATH_DEPTH`. It worked with instrumentation but doesn't seem to with `tau_exec`.

- `EBS_UNWIND` - unwinds the callstack; just the profile is not more informative but seems that call graph in the GUI is.
