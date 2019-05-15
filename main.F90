program fortran_c_api
  use capi_mod
  use iso_c_binding
	include 'mpif.h'
	integer(c_int) stat, num
	integer err, mstat
	call mpi_init(err)
	num = 1000
	stat = pass_by_val(num)
	call mpi_finalize(err)
end program fortran_c_api
