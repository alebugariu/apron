(* File generated from ppl.idl *)


(*
 This file is part of the APRON Library, released under GPL license.
 Please read the COPYING file packaged in the distribution.
*)

(** Convex Polyhedra and Linear Congruences abstract domains (PPL wrapper) *)

(** This module is a wrapper around the Parma Polyhedra Library. *)


type loose
type strict
(** Two flavors for convex polyhedra: loose or strict.

Loose polyhedra cannot have strict inequality constraints like [x>0].
They are algorithmically more efficient
(less generators, simpler normalization).

Convex polyhedra are defined by the conjunction of a set of linear
constraints of the form [a_0*x_0 + ... + a_n*x_n + b >= 0] or
[a_0*x_0 + ... + a_n*x_n + b > 0]
where [a_0, ..., a_n, b, c] are constants and [x_0, ..., x_n] variables.

*)

type grid
(** Linear congruences.


      Linear congruences are defined by the conjunction of equality constraints
modulo a rational number, of the form [a_0*x_0 + ... + a_n*x_n = b mod c],
where [a_0, ..., a_n, b, c] are constants and [x_0, ..., x_n] variables.
*)

type 'a t
(** Type of convex polyhedra/linear congruences, where ['a] is [loose], [strict] or [grid].

Abstract values which are convex polyhedra have the type
[loose t Apron.AbstractX.t] or [strict t Apron.AbstractX.t].

Abstract values which are conjunction of linear congruences equalities have the type
[grid t Apron.AbstractX.t].

Managers allocated by PPL have the type ['a t Apron.Manager.t].
*)



(** Allocate a PPL manager for loose convex polyhedra. *)
external manager_alloc_loose : unit -> loose t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_loose"



(** Allocate a PPL manager for strict convex polyhedra. *)
external manager_alloc_strict : unit -> strict t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_strict"



(** Allocate a new manager for linear congruences (grids) *)
external manager_alloc_grid : unit -> grid t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_grid"



(**
{2 Compilation information}

{3 Bytecode compilation}
To compile to bytecode, you should first generate a custom
interpreter with a command which should look like:

[ocamlc -I $APRON_PREFIX/lib -make-runtime -o myrun -cc "g++" bigarray.cma gmp.cma apron.cma ppl.cma]

and then you compile and link your example [X.ml] with

[ocamlc -I $APRON_PREFIX/lib -c X.ml] and

[ocamlc -I $APRON_PREFIX/lib -use-runtime myrun -o X bigarray.cma gmp.cma apron.cma ppl.cma X.cmo]

{b Comments:}

Do not forget the [-cc "g++"] option: PPL is a C++ library which requires
a C++ linker.

The C libraries related to [gmp.cma], [apron.cma] and [ppl.cma]
are automatically looked for (thanks to the auto-linking feature
provided by [ocamlc]).  Be aware that PPL requires the C++ wrapper
[libgmpxx.a] library on top of GMP library, which should thus be
installed.  With the [-noautolink] option, the generation of the
custom runtime executable should be done with

[ocamlc -I $APRON_PREFIX/lib -noautolink -make-runtime -o myrun -cc "g++" 
bigarray.cma gmp.cma apron.cma ppl.cma -ccopt "-L$GMP_PREFIX/lib ..." 
-cclib "-lap_ppl_caml -lap_ppl -lppl -lgmpxx -lapron_caml -lapron -lgmp_caml 
-lmpfr -lgmp -lbigarray -lcamlidl"]

{3 Native-code compilation}
You compile and link with

[ocamlopt -I $APRON_PREFIX/lib -c X.ml] and

[ocamlopt -I $APRON_PREFIX/lib -o X -cc "g++" bigarray.cmxa gmp.cmxa apron.cmxa ppl.cmxa X.cmx]

{b Comments:} Same as for bytecode compilation. Do not forget the [-cc "g++"] option. With the [-noautolink] option, the linking command becomes

[ocamlopt -I $APRON_PREFIX/lib -o X -cc "g++" bigarray.cmxa gmp.cmxa apron.cmxa ppl.cmxa -ccopt "-L$GMP_PREFIX/lib ..." -cclib "-lap_ppl_caml -lap_ppl -lppl -lgmpxx -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl" X.cmx]
*)
