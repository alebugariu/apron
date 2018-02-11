(* File generated from gmp_random.idl *)

(* This file is part of the MLGmpIDL interface, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type state

(** GMP random generation functions *)


(** {2 Random State Initialization} *)

external init_default : unit -> state
	= "camlidl_gmp_random_gmp_randinit_default"

external init_lc_2exp : Mpz.t -> int -> int -> state
	= "camlidl_gmp_random_gmp_randinit_lc_2exp"

external init_lc_2exp_size : int -> state
	= "camlidl_gmp_random_gmp_randinit_lc_2exp_size"

(** {2 Random State Seeding} *)

external seed : state -> Mpz.t -> unit
	= "camlidl_gmp_random_gmp_randseed"

external seed_ui : state -> int -> unit
	= "camlidl_gmp_random_gmp_randseed_ui"

(** {2 Random Number Functions} *)

module Mpz : sig
external urandomb : Mpz.t -> state -> int -> unit
	= "camlidl_gmp_random_mpz_urandomb"

external urandomm : Mpz.t -> state -> Mpz.t -> unit
	= "camlidl_gmp_random_mpz_urandomm"

external rrandomb : Mpz.t -> state -> int -> unit
	= "camlidl_gmp_random_mpz_rrandomb"

end
module Mpf : sig
external urandomb : Mpf.t -> state -> int -> unit
	= "camlidl_gmp_random_mpf_urandomb"

end
