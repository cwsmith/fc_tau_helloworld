## Install Tau

- Download the TAUv2.28.1 from tau.uoregon.edu/tau.tgz
- Unpack in your home
- Configure and install with

```
module unload darshan
./configure -arch=craycnl  -bfd=download -dwarf=download  -iowrapper -unwind=download -mpi; make install
```

- Create an environment script for using Tau, `envTau.sh`, with the following
  contents (adjust the path to tau bin dir):

```
module unload darshan
export PATH=/path/to/tau-2.28.1/craycnl/bin:$PATH 
```

## Build, Run, and Profile Fortran/C Example

```
make clean
make
salloc -N 1 -q debug -C knl -t 00:10:00
srun -n 5 tau_exec -ebs ./fc_simple 
```

---> no profile produced, you should see all 5 ranks printing a number to the screen though

