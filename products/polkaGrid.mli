(* File generated from polkaGrid.idl *)


(** Reduced product of NewPolka polyhedra and PPL grids *)

(** Type of abstract values, where ['a] is [Polka.loose] or [Polka.strict]. *)
type 'a t


(** Create a PolkaGrid manager with loose convex polyhedra. *)
external manager_alloc_loose : unit -> Polka.loose t Apron.Manager.t
	= "camlidl_polkaGrid_ap_pkgrid_manager_alloc_loose"


(** Create a PolkaGrid manager with strict convex polyhedra. *)
external manager_alloc_strict : unit -> Polka.strict t Apron.Manager.t
	= "camlidl_polkaGrid_ap_pkgrid_manager_alloc_strict"



(**
{2 Compilation information}

{3 Bytecode compilation}
To compile to bytecode, you should first generate a custom
interpreter with a command which should look like:

[ocamlc -I $APRON_PREFIX/lib -make-runtime -o myrun -cc "g++" bigarray.cma gmp.cma apron.cma polkaGrid.cma]

and then you compile and link your example [X.ml] with

[ocamlc -I $APRON_PREFIX/lib -c X.ml] and

[ocamlc -I $APRON_PREFIX/lib -use-runtime myrun -o X bigarray.cma gmp.cma apron.cma polkaGrid.cma X.cmo]

{b Comments:} The C libraries related to [gmp.cma], [apron.cma] and
[polkaGrid.cma]
are automatically looked for (thanks to the auto-linking feature
provided by [ocamlc]).

With the [-noautolink] option, the generation of the custom
runtime executable should be done with

[ocamlc -I $APRON_PREFIX/lib -noautolink -make-runtime -o myrun bigarray.cma gmp.cma apron.cma polkaGrid.cma -ccopt "-L$GMP_PREFIX/lib ..." -cclib "-lpolkaGrid_caml -lap_pkgrid -lpolka -lap_ppl -lppl -lgmpxx -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"]

{3 Native-code compilation}
You compile and link with

[ocamlopt -I $APRON_PREFIX/lib -c X.ml] and

[ocamlopt -I $APRON_PREFIX/lib -o X bigarray.cmxa gmp.cmxa apron.cmxa polkaGrid.cmxa X.cmx]

{b Comments:} Same as for bytecode compilation. With the
[-noautolink] option, the linking command becomes

[ocamlopt -I $APRON_PREFIX/lib -o X bigarray.cmxa gmp.cmxa apron.cmxa polka.cmxa ppl.cmxa -cc "g++" -ccopt "-L$GMP_PREFIX/lib ..." -cclib "-lpolkaGrid_caml -lap_pkgrid -lpolka -lap_ppl -lppl -lgmpxx -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl" X.cmx]
*)