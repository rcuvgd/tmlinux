open Cil

module L = List

exception Ignore

(* utility: get last element of a list *)
let getLastElement (l :'a list) :'a =
  L.hd(L.rev l);;

(* this function will be executed for each complex type (struct
 * or union). all members which type is zero length arrays will
 * be removed. last member is ignored (may be a flexible array).
 *) 
let remZeroLenArrays (ci:compinfo) :compinfo =
  if (0 < L.length ci.cfields) then
    begin
      let last = getLastElement ci.cfields in
      let zeroLenArrayFilter (fi:fieldinfo) :bool =
        match fi.ftype with
        | _ when fi==last -> true (* ignore last member *)
        | TArray(btp, None, att) ->
            ignore (*warn "flexible array: %s.%s [removed]" ci.cname fi.fname*);
            false
        | TArray(btp, (Some exp), att) when isZero (constFold true exp) ->
            ignore (*warn "0-length array: %s.%s [removed]" ci.cname fi.fname*);
            false
        | _ -> true;
      in
        ci.cfields <- L.filter zeroLenArrayFilter ci.cfields;
    end;
  ci;;

(* this function will be executed on each type in the program.
 * type conversions from gcc to tic:
 *
 *	type array[0]			=>	type array[]
 *	struct{}			=>	struct{char __dummy;}
 *	struct{type array[]; ...}	=>	struct{...}
 *	union{type array[]; ...}	=>	union{...}
 *)
let tpConvert (tp:typ) (loc:location) :typ =
  match tp with
  (* type array_decl[0] => type array_decl[] *)
  | TArray(btp, (Some exp), att) when isZero (constFold true exp)
      -> TArray(btp, None, att)
  (* struct{} => struct{char __dummy;} *)
  | TComp(ci, att) when ci.cstruct && (L.length ci.cfields = 0)
      -> ci.cfields <- [{fcomp=ci;fname="__dummy";ftype=TInt(IChar,[]);fattr=[];floc=loc;fbitfield=None}];
         TComp(ci, att)
  (* remove non-closing zero-length-arrays from unions/structs *)
  | TComp(ci, att)
      -> TComp(remZeroLenArrays(ci), att)
  (* othrwise: leave untouched *)
  | _ -> tp;;
  
 
(* Transformation Visitor: does not change globals *)
class tpcvtVisitor f = object inherit nopCilVisitor
  (* convert types *)
  method vtype tp =
    ChangeDoChildrenPost(tpConvert tp !currentLoc, (fun x->x))

end

let outTpcvt (f:file) :unit =
  try
    visitCilFileSameGlobals (new tpcvtVisitor f) f;
  with e ->
    Printf.printf "Exception in outTcs: %s\n" (Printexc.to_string e);
    raise e
    
let feature:featureDescr = {
  fd_name = "Tpcvt";
  fd_enabled = Cilutil.doTpcvt;
  fd_description = "output C acceptable Type by the TMCC Compiler";
  fd_extraopt = [];
  fd_doit = (function (f:file) -> outTpcvt f);
  fd_post_check = true;
  }

