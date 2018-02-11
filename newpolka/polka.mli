(* File generated from polka.idl *)

type internal

(** Convex Polyhedra and Linear Equalities abstract domains *)

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

type equalities
(** Linear equalities.

Linear equalities are conjunctions of linear
equalities of the form [a_0*x_0 + ... + a_n*x_n + b = 0].
*)

type 'a t
(** Type of convex polyhedra/linear equalities, where ['a] is [loose], [strict] or [equalities].

Abstract values which are convex polyhedra have the type
[(loose t) Apron.Abstract0.t] or [(loose t) Apron.Abstract1.t] or
[(strict t) Apron.Abstract0.t] or [(strict t) Apron.Abstract1.t].

Abstract values which are conjunction of linear equalities have the type
[(equalities t) Apron.Abstract0.t] or [(equalities t) Apron.Abstract1.t].

Managers allocated by NewPolka have the type ['a t Apron.Manager.t].
*)



(** Create a NewPolka manager for loose convex polyhedra. *)
external manager_alloc_loose : unit -> loose t Apron.Manager.t
	= "camlidl_polka_pk_manager_alloc_loose"



(** Create a NewPolka manager for strict convex polyhedra. *)
external manager_alloc_strict : unit -> strict t Apron.Manager.t
	= "camlidl_polka_pk_manager_alloc_strict"



(** Create a NewPolka manager for conjunctions of linear equalities. *)
external manager_alloc_equalities : unit -> equalities t Apron.Manager.t
	= "camlidl_polka_pk_manager_alloc_equalities"



(** Get the internal submanager of a NewPolka manager. *)
external manager_get_internal : 'a t Apron.Manager.t -> internal
	= "camlidl_polka_manager_get_internal"



(** Various options. See the C documentation *)


external set_max_coeff_size : internal -> int -> unit
	= "camlidl_polka_pk_set_max_coeff_size"

external set_approximate_max_coeff_size : internal -> int -> unit
	= "camlidl_polka_pk_set_approximate_max_coeff_size"

external get_max_coeff_size : internal -> int
	= "camlidl_polka_pk_get_max_coeff_size"

external get_approximate_max_coeff_size : internal -> int
	= "camlidl_polka_pk_get_approximate_max_coeff_size"



(**
{2 Compilation information}

{3 Bytecode compilation}
To compile to bytecode, you should first generate a custom
interpreter with a command which should look like:

[ocamlc -I $APRON_PREFIX/lib -make-runtime -o myrun bigarray.cma gmp.cma apron.cma polka.cma -cclib "-lpolkag"]

and then you compile and link your example [X.ml] with

[ocamlc -I $APRON_PREFIX/lib -c X.ml] and

[ocamlc -I $APRON_PREFIX/lib -use-runtime myrun -o X bigarray.cma gmp.cma apron.cma polka.cma X.cmo]

{b Comments:} The C libraries related to [gmp.cma] and [apron.cma] are
automatically looked for (thanks to the auto-linking feature provided by
[ocamlc]). For [polka.cma], the library [libpolka.a], identic to
[libpolkaMPQ.a], is selected by default. The [-noautolink] option should be
used to select a differetn version. See the C documentation of [Polka] library
for details. 

With the [-noautolink] option, the generation of the custom
runtime executable should be done with

[ocamlc -I $APRON_PREFIX/lib -noautolink -make-runtime -o myrun bigarray.cma gmp.cma apron.cma polka.cma -ccopt "-L$GMP_PREFIX/lib ..." -cclib "-lpolka_caml -lpolka -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"]

{3 Native-code compilation}
You compile and link with

[ocamlopt -I $APRON_PREFIX/lib -c X.ml] and

[ocamlopt -I $APRON_PREFIX/lib -o X bigarray.cmxa gmp.cmxa apron.cmxa polka.cmxa -cclib "-lpolkag" X.cmx]

{b Comments:} Same as for bytecode compilation. With the
[-noautolink] option, the linking command becomes

[ocamlopt -I $APRON_PREFIX/lib -o X bigarray.cmxa gmp.cmxa apron.cmxa polka.cmxa -ccopt "-L$GMP_PREFIX/lib ..." -cclib "-lpolka_caml -lpolkaMPQ -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl" X.cmx]
*)
