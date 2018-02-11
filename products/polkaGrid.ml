(* File generated from polkaGrid.idl *)


(** Reduced product of NewPolka polyhedra and PPL grids *)

(** Type of abstract values, where ['a] is [Polka.loose] or [Polka.strict]. *)
type 'a t

external manager_alloc_loose : unit -> Polka.loose t Apron.Manager.t
	= "camlidl_polkaGrid_ap_pkgrid_manager_alloc_loose"

external manager_alloc_strict : unit -> Polka.strict t Apron.Manager.t
	= "camlidl_polkaGrid_ap_pkgrid_manager_alloc_strict"

