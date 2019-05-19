program fortran_c_api
  use capi_mod
  use iso_c_binding
	include 'mpif.h'
	integer(c_int) stat, num
	integer err, mstat, rank
	call mpi_init(err)
	call mpi_comm_rank(MPI_COMM_WORLD,rank,err)
	num = 1000 + rank
	stat = pass_by_val(num)
	stat = multimap_stats()
	call mpi_finalize(err)
end program fortran_c_api
