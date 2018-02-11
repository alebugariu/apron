(* File generated from dim.idl *)

(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type t = int
and change = {
  dim : int array;
  intdim : int;
  realdim : int;
}
and perm = int array
and dimension = {
  intd: int;
  reald: int;
}

(** APRON Dimensions and related types *)
