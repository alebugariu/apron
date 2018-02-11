(* File generated from oct.idl *)

type internal

(*
 This file is part of the APRON Library, released under LGPL license.
 Please read the COPYING file packaged in the distribution.
*)

 
(** Octagon abstract domain. *)
 


 
type t
(** Type of octagons.

Octagons are defined by conjunctions of inequalities of the form
[+/-x_i +/- x_j >= 0].

Abstract values which are octagons have the type [t Apron.AbstractX.t].

Managers allocated for octagons have the type [t Apron.manager.t].
*)

 
(** Allocate a new manager to manipulate octagons. *)
external manager_alloc : unit -> t Apron.Manager.t
	= "camlidl_oct_oct_manager_alloc"

(** No internal parameters for now... *)
external manager_get_internal : t Apron.Manager.t -> internal
	= "camlidl_oct_manager_get_internal"

(** Approximate a set of generators to an abstract value, with best precision. *)
external of_generator_array : t Apron.Manager.t -> int -> int -> Apron.Generator0.t array -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_of_generator_array"

(** Widening with scalar thresholds. *)
external widening_thresholds : t Apron.Manager.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t -> Apron.Scalar.t array -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_widening_thresholds"

(** Standard narrowing. *)
external narrowing : t Apron.Manager.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_narrowing"

(** Perturbation. *)
external add_epsilon : t Apron.Manager.t -> t Apron.Abstract0.t -> Apron.Scalar.t -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_add_epsilon"

(** Perturbation. *)
external add_epsilon_bin : t Apron.Manager.t -> t Apron.Abstract0.t -> t Apron.Abstract0.t -> Apron.Scalar.t -> t Apron.Abstract0.t
	= "camlidl_oct_ap_abstract0_oct_add_epsilon_bin"

(** Algorithms. *)
val pre_widening : int


(**
{2 Compilation information}

{3 Bytecode compilation}
To compile to bytecode, you should first generate a custom interpreter with a
command which should look like:

[ocamlc -I $APRON_PREFIX/lib -make-runtime -o myrun bigarray.cma gmp.cma apron.cma oct.cma]

and then you compile and link your example [X.ml] with

[ocamlc -I $APRON_PREFIX/lib -c X.ml] and

[ocamlc -I $APRON_PREFIX/lib -use-runtime myrun -o X bigarray.cma gmp.cma apron.cma oct.cma X.cmo]

{b Comments:} The C libraries related to [gmp.cma] and [apron.cma] are
automatically looked for (thanks to the auto-linking feature provided by
[ocamlc]).  For [oct.cma], the library [liboct.a], identic to [liboctMPQ.a], is
selected by default. The [-noautolink] option should be used to select a
differetn version. See the C documentation of [Oct] library for details.

With the [-noautolink] option, the generation of the custom runtime executable
should be done with

[ocamlc -I $APRON_PREFIX/lib -noautolink -make-runtime -o myrun bigarray.cma gmp.cma apron.cma oct.cma -ccopt "-L$GMP_PREFIX/lib ..." -cclib "-loct_caml -loctMPQ -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"]

{3 Native-code compilation}
You compile and link with

[ocamlopt -I $APRON_PREFIX/lib -c X.ml] and

[ocamlopt -I $APRON_PREFIX/lib -o X bigarray.cmxa gmp.cmxa apron.cmxa oct.cmxa X.cmx]

{b Comments:} Same as for bytecode compilation. With the
[-noautolink] option, the linking command becomes

[ocamlopt -I $APRON_PREFIX/lib -o X bigarray.cmxa gmp.cmxa apron.cmxa oct.cmxa -ccopt "-L$GMP_PREFIX/lib ..." -cclib "-loct_caml -loctMPQ -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl" X.cmx]
*)
