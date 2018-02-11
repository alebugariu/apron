(* File generated from box.idl *)


(** Intervals abstract domain *)

(** Type of boxes.

Boxes constrains each dimension/variable [x_i] to belong to an interval [I_i].

Abstract values which are boxes have the type [t Apron.AbstractX.t].

Managers allocated for boxes have the type [t Apron.manager.t].
*)
type t

external manager_alloc : unit -> t Apron.Manager.t
	= "camlidl_box_box_manager_alloc"

