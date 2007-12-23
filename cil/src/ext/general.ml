open Cil

module L = List

exception Ignore

(* cast void pointers to char pointers in arithmetic operations *)
let enableVoidPointerArithmetic (exp:exp) :exp =
  match exp with
  | BinOp(PlusPI  as op, e1, e2, (TPtr(TVoid(_),_) as tp))
  | BinOp(IndexPI as op, e1, e2, (TPtr(TVoid(_),_) as tp))
  | BinOp(MinusPI as op, e1, e2, (TPtr(TVoid(_),_) as tp)) ->
      let nt = TPtr(TInt(IChar,[]),[]) in
        BinOp(op, mkCast e1 nt, e2, tp)
  | BinOp(MinusPP  as  op,  e1,  e2,  tp)  when  isVoidPtrType  (typeOf  e1) ->  
      let nt = TPtr(TInt(IChar,[]),[]) in
        BinOp(op, mkCast e1 nt, mkCast e2 nt, tp)
  | SizeOf(TVoid(_)) -> one
  (* attempts to remove __alignof__: assuming tic will not change alignments *)
  | AlignOf(_) -> constFold true exp
  | AlignOfE(_) -> constFold true exp
  | _ -> exp;;
  
(* Transformation Visitor: changes globals *)
class ticGeneralVisitor f = object inherit nopCilVisitor
  (* visit global types (union/struct/enum w/out typedef) *)
  method vexpr exp =
    ChangeDoChildrenPost(enableVoidPointerArithmetic exp, (fun x->x))
end

let outTic (f:file) :unit =
  try
    visitCilFile (new ticGeneralVisitor f) f;
  with e ->
    Printf.printf "Exception in outTic: %s\n" (Printexc.to_string e);
    raise e

let feature:featureDescr = {
  fd_name = "General";
  fd_enabled = Cilutil.doGeneral;
  fd_description = "cast void pointers to char pointers in arithmetic operations";
  fd_extraopt = [];
  fd_doit = (function (f:file) -> outTic f);
  fd_post_check = true;
  }

