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

external manager_alloc_loose : unit -> loose t Apron.Manager.t
	= "camlidl_polka_pk_manager_alloc_loose"

external manager_alloc_strict : unit -> strict t Apron.Manager.t
	= "camlidl_polka_pk_manager_alloc_strict"

external manager_alloc_equalities : unit -> equalities t Apron.Manager.t
	= "camlidl_polka_pk_manager_alloc_equalities"

external manager_get_internal : 'a t Apron.Manager.t -> internal
	= "camlidl_polka_manager_get_internal"

external set_max_coeff_size : internal -> int -> unit
	= "camlidl_polka_pk_set_max_coeff_size"

external set_approximate_max_coeff_size : internal -> int -> unit
	= "camlidl_polka_pk_set_approximate_max_coeff_size"

external get_max_coeff_size : internal -> int
	= "camlidl_polka_pk_get_max_coeff_size"

external get_approximate_max_coeff_size : internal -> int
	= "camlidl_polka_pk_get_approximate_max_coeff_size"

