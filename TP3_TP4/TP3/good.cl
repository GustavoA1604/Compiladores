class A {
  int_assigned : Int;

  test() : Object {
    { int_assigned <- 0; }
  };
};

Class BB__ inherits A {
};

class Main {
  obj_A : A;
  int_unassigned : Int;
  bool_unassigned : Bool;
  str_unassigned : String;
  int_assigned : Int <- 1;
  int_assigned_2 : Int <- 2;
  int_assigned_3 : Int <- 3;
  bool_assigned : Bool <- true;
  str_assigned : String <- "Hello World!";

  method_with_no_formals() : Int { int_unassigned };
  method_with_1_formal( x : Int ) : Bool { bool_unassigned };
  method_with_2_formals( x : Int, s : String ) : String {
    str_unassigned
  };

  method_expression_semicolon() : Object {
    {
      let new_variable : Int <- New Int in (new_variable + 1);
      int_assigned <- 1;
      int_assigned_3 <- int_assigned + int_assigned_2;
      int_assigned_3 <- int_assigned - int_assigned_2;
      int_assigned_3 <- int_assigned * int_assigned_3;
      int_assigned_3 <- int_assigned_2 / int_assigned;
      ~int_unassigned;
      bool_unassigned = not bool_unassigned;
      int_assigned_3 <- if 3 < 4 then (3) else (4) fi;
      bool_assigned <- false;
      while (1 <= int_assigned_3) loop int_assigned_3 = int_assigned_3 - 1 pool;
      case int_unassigned of
        int_assigned : Int => true;
      esac;
      case int_assigned_2 of
        int_assigned : Int => true;
        int_assigned_3 : Bool => false;
      esac;
      obj_A @ A.test();
    }
  };

  method_call_other( x : Int ) : Object {
    method_expression_semicolon()
  };

  main() : Object {
    { method_call_other(10); }
  };
};
