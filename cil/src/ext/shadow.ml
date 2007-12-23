open Cil

module L = List

exception Ignore

let typ_shadow4init_prefix = "__typ_ticshadow_";;
let var_shadow4init_prefix = "__var_ticshadow_";;

(* utility: get last element of a list *)
let getLastElement (l :'a list) :'a =
  L.hd(L.rev l);;

(* utility: remove last element of a list *)
let remLastElement (l: 'a list) :'a list =
  L.rev(L.tl(L.rev l));;

let isLastMemberFlexibleArray (ci:compinfo) :bool =
  0 < L.length ci.cfields && match (getLastElement ci.cfields).ftype with
    | TArray(_,None,_) -> true
    | _ -> false;;
    
(* A compound type needs a shadow for initialization if:
 * (1) it is a struct which last member is a flexible array
 * or (2) it is a struct with a member that requires a shadow
 * or (3) it is a union which first member requires a shadow
 *)
let getShadow4Init (ci:compinfo) (loc:location) :global list =
  let rec getShadow4InitRec (fil:fieldinfo list) (create_new_global:bool) :(fieldinfo list * bool * global list) =
    match fil with
    | [] -> ([], false, [])
    | fi :: tail ->
        let (tail_shadows, _, _) = getShadow4InitRec tail false in
          match fi.ftype with
          | TComp(ci, al) ->
              let shadow_required = isLastMemberFlexibleArray ci
              and n_cname = typ_shadow4init_prefix ^ ci.cname in
              let recurse =
                if shadow_required then
                  remLastElement ci.cfields	(* recurse on all fields but flexible array *)
                else if ci.cstruct then
                  ci.cfields			(* recurse on all struct-members *)
                else
                  [L.hd ci.cfields] in	(* recurse on first union-member *)
              let (n_cflds, rec_shadow_required, _) = getShadow4InitRec recurse false in
                if (shadow_required || rec_shadow_required) then
                  let n_ftype = TComp({ci with cname=n_cname; cfields=n_cflds}, al) in
                  let n_field = {fi with ftype=n_ftype} in
                    if create_new_global then
                      (n_field::tail_shadows, true, [GCompTag({ci with cname=n_cname; cfields=n_cflds}, loc)])
                    else
                      (n_field::tail_shadows, true, [])
                else
                  (fi::tail_shadows, false, [])
          | _ -> (fi::tail_shadows, false, []) in
  let fake_fi = { fcomp=ci; fname=ci.cname; ftype=TComp(ci, []); fbitfield=None; fattr=[]; floc=loc } in
  let (_, _, more_globals) = getShadow4InitRec [fake_fi] true in
    GCompTag(ci, loc) :: more_globals;;

let useShadow4Init (vi:varinfo) (ii:initinfo) (loc:location) :global list =
  let rec getShadowInitRec (oil:(offset*init) list) :((offset*init) list * bool) =
    match oil with
    | [] -> ([], false)
    | hd_oi::tl ->
        let (tl_oil, tl_uses_shadow) = getShadowInitRec tl in
          match hd_oi with
          | (o,CompoundInit(TComp(ci,_) as tp, hd_oil)) ->
              (* recurse on members of compound elements *)
              begin
                let (rec_oil, rec_uses_shadow) = getShadowInitRec hd_oil in
                let (hd_uses_shadow, nhd_oi) =
                  if (0 < L.length hd_oil) then
                    match getLastElement hd_oil with
                    | (_, CompoundInit(TArray(_,None,_),[])) ->
                        (* remove closing flexible arrays *)
                        (true, (o,CompoundInit(tp, remLastElement rec_oil)))
                    | _ -> (rec_uses_shadow, (o,CompoundInit(tp, rec_oil)))
                  else
                    (rec_uses_shadow, (o,CompoundInit(tp, rec_oil))) in
                  (nhd_oi::tl_oil, hd_uses_shadow||tl_uses_shadow)
              end
          | (o,CompoundInit(TArray(TComp(_,_),_,_) as arr, oil))
          | (o,CompoundInit(TArray(TArray(_,_,_),_,_) as arr, oil)) ->
              (* recurse on arrays of compound elements *)
              let (rec_oil, rec_uses_shadow) = getShadowInitRec oil in
                ((o,(CompoundInit(arr,rec_oil)))::tl_oil, rec_uses_shadow||tl_uses_shadow)
          | _ -> (hd_oi::tl_oil, tl_uses_shadow)
  in match ii.init with
  | Some(CompoundInit(_,_) as init) ->
      let fake_oil = [(NoOffset,init)] in
        begin
          match getShadowInitRec fake_oil with
          | ((NoOffset, CompoundInit(TComp(ci,al), oil))::[], true) ->
              let n_vname = var_shadow4init_prefix  ^ vi.vname
              and n_cname = typ_shadow4init_prefix  ^ ci.cname in
              let n_type = TComp({ci with cname=n_cname}, al) in
              let n_init = Some(CompoundInit(n_type, oil)) in
                [ GPragma(Attr("ALIAS_TMP_NAME", [ACons(n_vname,[]);ACons(vi.vname,[])]), loc) ;
                  GVar(vi, {ii with init=None}, loc) ;
                  GVar({vi with vname=n_vname; vtype=n_type}, {ii with init=n_init}, loc)
                ]
          | ((NoOffset, CompoundInit(TArray(tp,_,_), oil))::[], true) ->
              let n_vname = var_shadow4init_prefix  ^ vi.vname in
              let n_init = Some(CompoundInit(tp, oil)) in
                [ GPragma(Attr("ALIAS_TMP_NAME", [ACons(n_vname,[]);ACons(vi.vname,[])]), loc) ;
                  GVar(vi, {ii with init=None}, loc) ;
                  GVar({vi with vname=n_vname}, {ii with init=n_init}, loc)
                ]
          | _ -> [GVar(vi, ii, loc)]
        end
  | _ -> [GVar(vi, ii, loc)];;


(* Transformation Visitor: create and use shadow struct/unions (changes globals) *)
class ticShadowVisitor f = object inherit nopCilVisitor
  (* visit global types (union/struct/enum w/out typedef) *)
  method vglob = function
    | GCompTag(ci, loc) -> ChangeDoChildrenPost(getShadow4Init ci loc, (fun x->x))
    | GVar(vi, ii, loc) -> ChangeDoChildrenPost(useShadow4Init vi ii loc, (fun x->x))
    | _ -> DoChildren
end
 
let outShadow (f:file) :unit =
  try
    visitCilFile (new ticShadowVisitor f) f;
  with e ->
    Printf.printf "Exception in outTic: %s\n" (Printexc.to_string e);
    raise e

let feature:featureDescr = {
  fd_name = "Shadow";
  fd_enabled = Cilutil.doShadow;
  fd_description = "use shodow4init";
  fd_extraopt = [];
  fd_doit = (function (f:file) -> outShadow f);
  fd_post_check = true;
}

