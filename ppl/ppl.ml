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

external manager_alloc_loose : unit -> loose t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_loose"

external manager_alloc_strict : unit -> strict t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_strict"

external manager_alloc_grid : unit -> grid t Apron.Manager.t
	= "camlidl_ppl_ap_ppl_manager_alloc_grid"

