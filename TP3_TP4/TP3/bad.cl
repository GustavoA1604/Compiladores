
(*
 *  execute "coolc bad.cl" to see the error messages that the coolc parser
 *  generates
 *
 *  execute "myparser bad.cl" to see the error messages that your parser
 *  generates
 *)

(* no error *)
class A {
};

(* error:  b is not a type identifier *)
Class b inherits A {
};

(* error:  a is not a type identifier *)
Class C inherits a {
};

(* error:  keyword inherits is misspelled *)
Class D inherts A {
  good_attribute : Int;
};

(* error:  closing brace is missing *)
Class E inherits A {
  good_attribute : Int;
;

(* errors in features *)
Class F {
(* no error *)
  good_attribute : Int;
(* error: wrong attribute definitions *)
  bad_attribute_1 : int;
  bad_attribute_2 : Int = 3;
  bad_attribute_3 : int <- Int;

(* error: missing closing parenthesis *)
  bad_method_1 ( : Int { 1 };
(* error: wrong colon *)
  good_method_1 () : Int { 1 };
  bad_method_2 () = Int { 1 };
(* error: wrong TYPEID *)
	bad_method_3 () : int { 1 };
(* error: missing closing braces *)
	bad_method_4 () : Int { 1 ; 
(* error: wrong formal list *)
  bad_method_5 ( Int ) : Int { 1 };
  bad_method_6 ( x : Int, ) : Int { 1 };
  bad_method_7 ( x : Int, y ) : Int { 1 };
  good_method_2 ( x : Int ) : Int { 1 };
};

(* errors in let expressions *)
class G {
  good_method () : Int {
    {
      let x : Int in function_name();
      let x = Bool in funciton_name();
      let Int : Int in function_name();
      let x : int in function_name();
      let x : Int = 2 in function_name();
      let x : Int <- in function_name();
      let x : Int <- Int in function_name();
    }
  };
};

(* errors in block {...} expressions *)
class H {
  good_method () : Int {
    if true then {
      bad expression 1;
      bad_OBJECTID == 2;
      expr @ . missing_typeid();
      1 + 1;
      wrong_function_call_1(1, );
      wrong_function_call_2(;
      if missing_fi then 1 else 2;
      while missing_loop 1 pool;
      isvoid; (* this one is not shown by coolc *)
      new int; (* this one is not shown by coolc *)
      1 +;
      1 + * 2;
      ( missing_parenthesis;
      True; (* this one is not shown by coolc *)
      good_attribute <- 1;
    } else 1 fi
  };
};

