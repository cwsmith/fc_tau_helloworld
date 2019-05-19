module capi_mod
  interface
     integer(c_int) function pass_by_val(num) bind(c, name='pass_by_val')
       use iso_c_binding
       implicit none
       integer(c_int), value :: num
     end function pass_by_val
     integer(c_int) function multimap_stats() bind(c, name='multimap_stats')
       use iso_c_binding
       implicit none
     end function multimap_stats
  end interface
contains
end module capi_mod
