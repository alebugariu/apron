(* File generated from box.idl *)


(** Intervals abstract domain *)

(** Type of boxes.

Boxes constrains each dimension/variable [x_i] to belong to an interval [I_i].

Abstract values which are boxes have the type [t Apron.AbstractX.t].

Managers allocated for boxes have the type [t Apron.manager.t].
*)
type t


(** Create a Box manager. *)
external manager_alloc : unit -> t Apron.Manager.t
	= "camlidl_box_box_manager_alloc"



(**
{2 Compilation information}

{3 Bytecode compilation}
To compile to bytecode, you should first generate a custom
interpreter with a command which should look like:

[ocamlc -I $APRON_PREFIX/lib -make-runtime -o myrun bigarray.cma gmp.cma apron.cma box.cma]

and then you compile and link your example [X.ml] with

[ocamlc -I $APRON_PREFIX/lib -c X.ml] and

[ocamlc -I $APRON_PREFIX/lib -use-runtime myrun -o X bigarray.cma gmp.cma apron.cma box.cma X.cmo]

{b Comments:} The C libraries related to [gmp.cma] and [apron.cma] are
automatically looked for (thanks to the auto-linking feature provided by
[ocamlc]). For [box.cma], the library [libbox.a], identic to [libboxMPQ.a], is
selected by default. The [-noautolink] option should be used to select a differetn version. See the C documentation of [Box] library for details.

With the [-noautolink] option, the generation of the custom
runtime executable should be done with

[ocamlc -I $APRON_PREFIX/lib -noautolink -make-runtime -o myrun bigarray.cma gmp.cma apron.cma box.cma -ccopt "-L$GMP_PREFIX/lib ..." -cclib "-lbox_caml -lboxMPQ -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"]

{3 Native-code compilation}
You compile and link with

[ocamlopt -I $APRON_PREFIX/lib -c X.ml] and

[ocamlopt -I $APRON_PREFIX/lib -o X bigarray.cmxa gmp.cmxa apron.cmxa box.cmxa X.cmx]

{b Comments:} Same as for bytecode compilation. With the
[-noautolink] option, the linking command becomes

[ocamlopt -I $APRON_PREFIX/lib -o X bigarray.cmxa gmp.cmxa apron.cmxa box.cmxa -ccopt "-L$GMP_PREFIX/lib ..." -cclib "-lbox_caml -lboxMPQ -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl" X.cmx]
*)
