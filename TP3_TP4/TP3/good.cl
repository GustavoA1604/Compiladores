class A {
};

Class BB__ inherits A {
};

class C {
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

  method_expression_semicolon() : Int {
    {
      let new_variable : Int <- New Int in (new_variable + 1);
      int_assigned <- 1;
      int_assigned_3 <- int_assigned_1 + int_assigned_2;
      int_assigned_3 <- int_assigned_1 - int_assigned_2;
      int_assigned_3 <- int_assigned_1 * int_assigned_3;
      int_assigned_3 <- int_assigned_2 / int_assigned_1;
      ~int_assigned;
      int_assigned = not int_assigned_2;
      int_assigned_3 <- if 3 < 4 then (3) else (4) fi;
      bool_assigned <- false;
      while (1 <= int_assigned_3) loop int_assigned_3 = int_assigned_3 - 1 pool;
      case int_unassigned of
        int_assigned : Int => true;
      esac;
      case int_assigned_2 of
        int_assigned : Int => true;
        int_assigned_3 : Int => false;
      esac;
      int_assigned @ Int . test();
    }
  };

  method_call_other( x : Int ) : Int {
    method_expression_semicolon()
  };
};
