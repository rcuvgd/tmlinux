open Cil

module L = List

exception Ignore

(* add a pragma to file globals *)
let addPragma (name:string) (symbol:string) (apl:attrparam list) (loc:location) (file:file) :unit =
  let cfold (ap:attrparam) :attrparam =
    match ap with
    | ASizeOf(typ) when isConstant(SizeOf(typ)) ->
        begin
          match (constFold true (SizeOf(typ))) with
          | Const(CInt64(i,_,_)) -> AInt(Int64.to_int i)
          | _ -> ap;
        end
    | AAlignOf(typ) when isConstant(AlignOf(typ)) ->
        begin
          match (constFold true (AlignOf(typ))) with
          | Const(CInt64(i,_,_)) -> AInt(Int64.to_int i)
          | _ -> ap;
        end
    | _ -> ap;
  in let cf = L.map cfold apl
  in
    file.globals <- GPragma(Attr(name, ACons(symbol, [])::cf), loc) :: file.globals;;

(* this function converts type attributes into pragmas.
 * the pragmas are put in file's global scope.
 *)
let tpAttrib2Pragma (tp:typ) (file:file) :typ =
  let tpName =
    match tp with
    | TNamed(ti, _)	-> ti.tname
    | TComp(ci, _)	-> ci.cname
    | TEnum(ei,_)	-> ei.ename
    | _			-> "<unknown type name>"
  and attStrReplacement (str:string) :string  =
    match str with
    | "deprecated"	
    | "aligned"	
    | "format"		-> raise Ignore		
    | _			-> raise Ignore in
  let actAttFilter = function Attr(str, ap) ->
    try
      addPragma (attStrReplacement str) tpName ap !currentLoc file;
      false
    with
      | Ignore -> false
      | Not_found -> true in
  match tp with
  | TNamed(ti, al) ->
      ti.ttype <- setTypeAttrs ti.ttype (L.filter actAttFilter (typeAttrs ti.ttype));
      TNamed(ti, (L.filter actAttFilter al))
  | TComp(ci, al) ->
      TComp({ci with cattr=L.filter actAttFilter ci.cattr}, (L.filter actAttFilter al))
  | TFun(tp, args, b, al) ->
      TFun(typeRemoveAttributes ["volatile"] tp, args, b, (L.filter actAttFilter al))
  | TEnum(ei, al)		-> TEnum(ei, (L.filter actAttFilter al))
  | TVoid(al)			-> TVoid(L.filter actAttFilter al)
  | TInt(ikind,al)		-> TInt(ikind, (L.filter actAttFilter al))
  | TFloat(fkind, al)		-> TFloat (fkind, (L.filter actAttFilter al))
  | TPtr(typ, al)		-> TPtr(typ, (L.filter actAttFilter al))
  | TArray(typ, exp, al)	-> TArray(typ, exp, (L.filter actAttFilter al))
  | tp -> tp;;

(* this function converts variable attributes into pragmas.
 * the pragmas are put in file's global scope.
 *)
let vdecAttrib2Pragma (vi:varinfo) (file:file) :varinfo =
  let attStrReplacement (str:string) :string =
    match str with
    | "alias"
    | "unused"
    | "aligned"
    | "warn_unused_result"	-> raise Ignore		(* no tic equivalence *)
    | _				-> raise Ignore in
  let actAttFilter (Attr(str,ap):attribute) :bool = 
    try
      addPragma (attStrReplacement str) vi.vname ap vi.vdecl file;
      false
    with 
      | Ignore -> false
      | Not_found -> true
  in
    (* don't replace vi; change it, because visitCilFileSameGlobals is used *)
    vi.vattr <- L.filter actAttFilter vi.vattr;
    vi;;

(* this function converts function attributes into pragmas.
 * the pragmas are put in file's global scope.
 *)
let fdecAttrib2Pragma (vi:varinfo) (file:file) :varinfo =
  let attStrReplacement (str:string) :string =
    match str with
    | "noinline"	
    | "noreturn"	
    | "section"		
    | "used"		
    | "pure"		
    | "format"
    | "weak"
    | "alias"
    | "unused"		-> raise Ignore		(* no tic equivalence *)
    | _			-> raise Ignore in
  let actAttFilter (Attr(str,ap):attribute) :bool =
    try
      addPragma (attStrReplacement str) vi.vname ap vi.vdecl file;
      false
    with
      | Ignore -> false
      | Not_found -> true
  in
    (* don't replace vi; change it, because visitCilFileSameGlobals is used *)
    vi.vattr <- L.filter actAttFilter vi.vattr;
    vi;;

(* Transformation Visitor: changes globals *)
class rmattrVisitor f = object inherit nopCilVisitor
  method vtype tp =
    ChangeDoChildrenPost(tpAttrib2Pragma tp f, (fun x->x))

  (* visit global types (union/struct/enum w/out typedef) *)
  method vglob = function
    | GCompTag(ci,l) ->
        (* remove all member attributes *)
        let attrib_filter fname (Attr(aname,_)) =
          ignore ;
          false in
        let remove_attribs fi = {fi with fattr=L.filter (attrib_filter fi.fname) fi.fattr} in
          ci.cfields <- L.map remove_attribs ci.cfields;

        (* type attributes on whole struct/union *)
        let tp = TComp(ci, ci.cattr) in
        let al = function TComp(_,b) -> b in
          ci.cattr <- al (tpAttrib2Pragma tp f);
        DoChildren
    | GEnumTag(ei,l) ->
        (* type attributes on whole enumeration *)
        let tp = TEnum(ei, ei.eattr) in
        let al = function TEnum(_,b) -> b in
          ei.eattr <- al (tpAttrib2Pragma tp f);
        DoChildren
    | _ -> DoChildren

  method vvdec vi =
    match vi.vtype with
    | TFun(tp, args, b, al) ->
        (* remove all parameter attributes *)
        let attrib_filter pname (Attr(aname,_)) =
          ignore (*warn "attribute '%s' on function (%s) argument (%s) [removed]" aname vi.vname pname*);
          false in
        let remove_arg_attribs (pname, ptype, attlist) =
          (pname, ptype, L.filter (attrib_filter pname) attlist) in
        let remove_all_arg_attribs opt_args =
          match opt_args with
          | Some(arglist) -> Some(L.map remove_arg_attribs arglist)
          | None -> None in
        let vi_wout_arg_attribs = {vi with vtype=TFun(tp, remove_all_arg_attribs args, b, al)} in
          ChangeDoChildrenPost(fdecAttrib2Pragma vi_wout_arg_attribs f, (fun x->x))
    | _ ->
        ChangeDoChildrenPost(vdecAttrib2Pragma vi f, (fun x->x))
end

let outRmattr (f:file) :unit =
  try
    visitCilFile(*SameGlobals*) (new rmattrVisitor f) f;
  with e ->
    Printf.printf "Exception in outRmattr: %s\n" (Printexc.to_string e);
    raise e

let feature:featureDescr = {
  fd_name = "Rmattr";
  fd_enabled = Cilutil.doRmattr;
  fd_description = "Remove gcc extention __attribute__ with the exception of packed and aligned";
  fd_extraopt = [];
  fd_doit = (function (f:file) -> outRmattr f);
  fd_post_check = true;
  }

